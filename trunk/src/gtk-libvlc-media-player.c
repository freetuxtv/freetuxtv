/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include "gtk-libvlc-media-player.h"

#define GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(o)       (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_LIBVLC_MEDIA_PLAYER, GtkLibVLCMediaPlayerPrivate))
typedef struct _GtkLibVLCMediaPlayerPrivate GtkLibVLCMediaPlayerPrivate;

struct _GtkLibVLCMediaPlayerPrivate
{
	gboolean initialized;

	GtkTreePath *current_media;
	
	gboolean is_media_parsed;
	gboolean is_recording;
	gboolean play_next_at_end;
};

#define LIBVLC_MAX_VOLUME_POWER 2.0 // Represent a percentage (2.0 = 200%)

static void
gtk_libvlc_media_player_class_init(GtkLibVLCMediaPlayerClass *klass);
static void
gtk_libvlc_media_player_init(GtkLibVLCMediaPlayer *object);
static void
gtk_libvlc_media_player_size_request(GtkWidget *widget, GtkRequisition *requisition);
static void
gtk_libvlc_media_player_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static void
gtk_libvlc_media_player_realize(GtkWidget *widget);
static void
gtk_libvlc_media_player_dispose (GObject *object);
static void
gtk_libvlc_media_player_destroy(GtkObject *object);

static void
gtk_libvlc_media_player_initialize(GtkLibVLCMediaPlayer *self);
static void
gtk_libvlc_media_player_play_media(GtkLibVLCMediaPlayer *self, GtkLibVLCMedia *media, gchar **options);
static void
gtk_libvlc_media_player_set_current_path(GtkLibVLCMediaPlayer *self, GtkTreePath *path);

static void
on_vlc_exception(GtkLibVLCMediaPlayer *self, libvlc_exception_t * ex);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	
#else
static void 
on_vlc_event(const libvlc_event_t * event, void * user_data);
#endif

G_DEFINE_TYPE (GtkLibVLCMediaPlayer, gtk_libvlc_media_player, GTK_TYPE_WIDGET);

GtkWidget *
gtk_libvlc_media_player_new (GtkLibVLCInstance* libvlc_instance)
{
	g_return_if_fail(libvlc_instance != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_INSTANCE(libvlc_instance));
	
	GtkLibVLCMediaPlayer *self = NULL;
	self = gtk_type_new (GTK_TYPE_LIBVLC_MEDIA_PLAYER);

	self->libvlc_instance = libvlc_instance;
	g_object_ref(G_OBJECT(self->libvlc_instance));

	self->media_list = gtk_tree_store_new(GTK_LIBVLC_MODEL_NB_COLUMN, GTK_TYPE_LIBVLC_MEDIA);
	
	return GTK_WIDGET(self);
}

void
gtk_libvlc_media_player_add_media (GtkLibVLCMediaPlayer *self, GtkLibVLCMedia *media)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	// Add the media in the media list
	GtkTreeIter iter;
	gtk_tree_store_append (self->media_list, &iter, NULL);
	gtk_tree_store_set (GTK_TREE_STORE(self->media_list), &iter,
			    GTK_LIBVLC_MODEL_MEDIA_COLUMN, media, -1);
}

GtkLibVLCMedia*
gtk_libvlc_media_player_get_current_media (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);

	return gtk_libvlc_media_player_get_media_from_path(self, priv->current_media);
}

GtkLibVLCMedia*
gtk_libvlc_media_player_get_media_from_path (GtkLibVLCMediaPlayer *self, GtkTreePath *path)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(path != NULL);
	
	// Get the media from the path
	GtkLibVLCMedia *media;
	GtkTreeIter iter;
	if(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter, path)){
		gtk_tree_model_get (GTK_TREE_MODEL(self->media_list),
				    &iter, 0, &media, -1);
		return media;
	}

	return NULL;
}

void
gtk_libvlc_media_player_clear_media_list(GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	gtk_tree_store_clear (self->media_list);
	
	gtk_libvlc_media_player_set_current_path(self, NULL);
}

void
gtk_libvlc_media_player_play (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);
	
	// Play the current media or the first if no current media
	GtkTreePath *path = priv->current_media;
	if(path == NULL){
		path = gtk_tree_path_new_first();
		gtk_libvlc_media_player_set_current_path(self, path);
		gtk_tree_path_free(path);
	}
	gtk_libvlc_media_player_play_media_at_path(self, priv->current_media);
}

void
gtk_libvlc_media_player_play_media_at_path (GtkLibVLCMediaPlayer *self, GtkTreePath *path)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	g_return_if_fail(path != NULL);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);
	
	gtk_libvlc_media_player_set_current_path(self, path);
	
	GtkLibVLCMedia *media;
	media = gtk_libvlc_media_player_get_media_from_path (self, priv->current_media);
	
	GtkTreeIter iter;
	g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter, priv->current_media) == TRUE);
	if(gtk_tree_model_iter_has_child(GTK_TREE_MODEL(self->media_list), &iter)){
		priv->is_media_parsed = TRUE;
	}else{
		priv->is_media_parsed = FALSE;
	}

	// Play the media
	gtk_libvlc_media_player_play_media (self, media, NULL);
}

void
gtk_libvlc_media_player_play_next (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);
	
	GtkTreePath* path;
	path = priv->current_media;
	if(path != NULL){
		GtkTreeIter iter1;
		GtkTreeIter iter2;
		g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter1, path) == TRUE);
		
		if(gtk_tree_model_iter_has_child(GTK_TREE_MODEL(self->media_list), &iter1)){
			gtk_tree_model_iter_children(GTK_TREE_MODEL(self->media_list), &iter2, &iter1);
			path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter2);
		}else{
			if(gtk_tree_model_iter_next (GTK_TREE_MODEL(self->media_list), &iter1)){
				path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
			}else{
				if(gtk_tree_model_iter_parent (GTK_TREE_MODEL(self->media_list), &iter2, &iter1)){
					if(gtk_tree_model_iter_next (GTK_TREE_MODEL(self->media_list), &iter2)){
						path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter2);
					}else{
						gtk_tree_model_get_iter_first(GTK_TREE_MODEL(self->media_list), &iter1);
						path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
					}
				}
			}
		}

		// Play the next media
		gtk_libvlc_media_player_play_media_at_path(self, path);
	}else{
		// Play the first media
		gtk_libvlc_media_player_play(self);	
	}	
}

void
gtk_libvlc_media_player_stop (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);
	
	// Stop the current playing
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	if (libvlc_playlist_isplaying (libvlc_instance, &_vlcexcep)) {
		on_vlc_exception (self, &_vlcexcep);
		libvlc_playlist_stop (libvlc_instance, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}
#else
	if(self->libvlc_mediaplayer != NULL){
		libvlc_media_player_stop (self->libvlc_mediaplayer, &_vlcexcep);
		on_vlc_exception(self, &_vlcexcep);
	}
#endif

	priv->is_recording = FALSE;
}

void
gtk_libvlc_media_player_set_volume (GtkLibVLCMediaPlayer *self, gdouble volume)
{
	
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(volume >= 0 && volume <= LIBVLC_MAX_VOLUME_POWER);
	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	libvlc_audio_set_volume (libvlc_instance, 
				 (gint)(volume*100), &_vlcexcep);    
	on_vlc_exception (self, &_vlcexcep);
}

gdouble
gtk_libvlc_media_player_get_volume (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	gint volume;
	volume = libvlc_audio_get_volume (libvlc_instance, &_vlcexcep);    
	on_vlc_exception (self, &_vlcexcep);
	return (gdouble)volume;
}

void
gtk_libvlc_media_player_set_fullscreen (GtkLibVLCMediaPlayer *self, gboolean fullscreen)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
        if(libvlc_instance != NULL){
                if (libvlc_playlist_isplaying (libvlc_instance, &_vlcexcep)) {
                        input_t = libvlc_playlist_get_input(libvlc_instance,
                                                            &_vlcexcep);
                        on_vlc_exception (self, &_vlcexcep);
                        libvlc_set_fullscreen(input_t, fullscreen, &_vlcexcep);
                        on_vlc_exception (self, &_vlcexcep);
			libvlc_input_free(input_t);
                }
        }
#else
        if(self->libvlc_mediaplayer != NULL){
                libvlc_set_fullscreen (self->libvlc_mediaplayer, fullscreen, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
        }
#endif

}

void
gtk_libvlc_media_player_record_current (GtkLibVLCMediaPlayer *self, gchar* output_filename)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	g_return_if_fail(output_filename != NULL);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);	

	// Set options for the record
	gchar *options[2];
	options[0] = g_strconcat(":sout=#duplicate{dst=display,dst=std{access=file,mux=ts,dst='",
				 output_filename, "'}}", NULL);
	options[1] = NULL;
	//gchar *options[] = {":vout-filter=transform", ":transform-type=90", NULL};

	GtkLibVLCMedia* media;
	media =	gtk_libvlc_media_player_get_current_media (self);
	gtk_libvlc_media_player_play_media (self, media, options);	

	g_free(options[0]);

	priv->is_recording = TRUE;

}

gboolean
gtk_libvlc_media_player_is_recording (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);

	return priv->is_recording;
}

GtkLibVLCState
gtk_libvlc_media_player_get_state (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));	
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	GtkLibVLCState gtkstate = GTK_LIBVLC_STATE_NOTHING_SPECIAL;

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
        if(libvlc_instance != NULL){
                if (libvlc_playlist_isplaying (libvlc_instance, &_vlcexcep)) {
                        input_t = libvlc_playlist_get_input(libvlc_instance,
                                                            &_vlcexcep);
                        on_vlc_exception (self, &_vlcexcep);
			int state = libvlc_input_get_state(input_t, &_vlcexcep);
                        on_vlc_exception (self, &_vlcexcep);
			switch(state){
			case 0 :
				gtkstate = GTK_LIBVLC_STATE_NOTHING_SPECIAL;
				break;
			case 1 :
				gtkstate = GTK_LIBVLC_STATE_OPENING;
				break;
			case 2 :
				gtkstate = GTK_LIBVLC_STATE_BUFFERING;	
				break;
			case 3 :
				gtkstate = GTK_LIBVLC_STATE_PLAYING;
				break;
			case 4 :
				gtkstate = GTK_LIBVLC_STATE_PAUSED;
				break;
			case 5 :
				gtkstate = GTK_LIBVLC_STATE_STOPPED;
				break;
			case 6 :
				gtkstate = GTK_LIBVLC_STATE_ENDED;
				break;
			case 7 :
				gtkstate = GTK_LIBVLC_STATE_ERROR;
				break;
			}
			libvlc_input_free(input_t);
		}
        }
#else
	g_return_if_fail(self->libvlc_mediaplayer != NULL);

	libvlc_state_t state;
	state = libvlc_media_player_get_state (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
	
	switch(state){
	case libvlc_NothingSpecial :
		gtkstate = GTK_LIBVLC_STATE_NOTHING_SPECIAL;
		break;
	case libvlc_Opening :
		gtkstate = GTK_LIBVLC_STATE_OPENING;
		break;
	case libvlc_Buffering :
		gtkstate = GTK_LIBVLC_STATE_BUFFERING;	
		break;
	case libvlc_Playing :
		gtkstate = GTK_LIBVLC_STATE_PLAYING;
		break;
	case libvlc_Paused :
		gtkstate = GTK_LIBVLC_STATE_PAUSED;
		break;
	case libvlc_Stopped :
		gtkstate = GTK_LIBVLC_STATE_STOPPED;
		break;
	case libvlc_Ended :
		gtkstate = GTK_LIBVLC_STATE_ENDED;
		break;
	case libvlc_Error:
		gtkstate = GTK_LIBVLC_STATE_ERROR;
		break;
	}
#endif
	return gtkstate;

}

const gchar*
gtk_libvlc_media_player_state_tostring (GtkLibVLCState state)
{
	switch(state){
	case GTK_LIBVLC_STATE_NOTHING_SPECIAL :
		return "GTK_LIBVLC_STATE_NOTHING_SPECIAL";
	case GTK_LIBVLC_STATE_OPENING :
		return "GTK_LIBVLC_STATE_OPENING";
	case GTK_LIBVLC_STATE_BUFFERING :
		return "GTK_LIBVLC_STATE_BUFFERING";
	case GTK_LIBVLC_STATE_PLAYING :
		return "GTK_LIBVLC_STATE_PLAYING";
	case GTK_LIBVLC_STATE_PAUSED :
		return "GTK_LIBVLC_STATE_PAUSED";
	case GTK_LIBVLC_STATE_STOPPED :
		return "GTK_LIBVLC_STATE_STOPPED";
	case GTK_LIBVLC_STATE_ENDED :
		return "GTK_LIBVLC_STATE_ENDED";
	case GTK_LIBVLC_STATE_ERROR:
		return "GTK_LIBVLC_STATE_ERROR";
	}
}

void
gtk_libvlc_media_player_set_play_next_at_end (GtkLibVLCMediaPlayer *self, gboolean b)
{	
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);

	priv->play_next_at_end = b;	
}

GtkLibVLCInstance*
gtk_libvlc_media_player_get_instance (GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	return self->libvlc_instance;
}


static void
gtk_libvlc_media_player_class_init (GtkLibVLCMediaPlayerClass *klass)
{
	GtkWidgetClass *widget_class;
	GtkObjectClass *object_class;
	GObjectClass *gobject_class;	

	widget_class = GTK_WIDGET_CLASS (klass);
	object_class = GTK_OBJECT_CLASS (klass);
	gobject_class = G_OBJECT_CLASS (klass);
	
	widget_class->realize = gtk_libvlc_media_player_realize;
	widget_class->size_request = gtk_libvlc_media_player_size_request;
	widget_class->size_allocate = gtk_libvlc_media_player_size_allocate;
	
	object_class->destroy = gtk_libvlc_media_player_destroy;

	gobject_class->dispose = gtk_libvlc_media_player_dispose;

	g_type_class_add_private (gobject_class, sizeof (GtkLibVLCMediaPlayerPrivate));
}

static void
gtk_libvlc_media_player_init (GtkLibVLCMediaPlayer *object)
{
	object->libvlc_instance = NULL;
	
	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(object);
	
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
#else
	object->libvlc_mediaplayer = NULL;
	priv->is_media_parsed = FALSE;
#endif

	object->media_list = NULL;
	
	priv->initialized = FALSE;
	priv->is_recording = FALSE;
	priv->play_next_at_end = TRUE;	
}

static void
gtk_libvlc_media_player_size_request(GtkWidget *widget, GtkRequisition *requisition)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(widget));
	g_return_if_fail(requisition != NULL);

	requisition->width = 240;
	requisition->height = 160;
}

static void
gtk_libvlc_media_player_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(widget));
	g_return_if_fail(allocation != NULL);
	
	widget->allocation = *allocation;

	if (GTK_WIDGET_REALIZED(widget)) {
		gdk_window_move_resize(widget->window,
				       allocation->x, allocation->y,
				       allocation->width, allocation->height);
	}
}

static void
gtk_libvlc_media_player_realize(GtkWidget *widget)
{
	GtkLibVLCMediaPlayer *libvlcmp = GTK_LIBVLC_MEDIA_PLAYER (widget);
	GdkWindowAttr attributes;
	guint attributes_mask;

	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(widget));
	
	if (GTK_WIDGET_NO_WINDOW (widget)){
		GTK_WIDGET_CLASS (gtk_libvlc_media_player_parent_class)->realize (widget);
	}else{
		GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);
		
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.x = widget->allocation.x;
		attributes.y = widget->allocation.y;
		attributes.width = widget->allocation.width;
		attributes.height = widget->allocation.height;		
		attributes.wclass = GDK_INPUT_OUTPUT;
		attributes.visual = gtk_widget_get_visual (widget);
		attributes.colormap = gtk_widget_get_colormap (widget);		
		attributes.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;
		
		attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
		
		widget->window = gdk_window_new(gtk_widget_get_parent_window (widget),
						& attributes, attributes_mask);
		
		gdk_window_set_user_data(widget->window, libvlcmp);
		
		widget->style = gtk_style_attach(widget->style, widget->window);
		gtk_style_set_background(widget->style, widget->window, GTK_STATE_ACTIVE);
	}
}

static void
gtk_libvlc_media_player_dispose (GObject *object)
{
	GtkLibVLCMediaPlayer *self;

	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	g_return_if_fail(object != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(object));
	 
	self = GTK_LIBVLC_MEDIA_PLAYER(object);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	priv->play_next_at_end = FALSE;

	// Release the media player
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
	if(self->libvlc_mediaplayer != NULL){
		// Detach events on the media player
		libvlc_event_manager_t *em;
		em = libvlc_media_player_event_manager (self->libvlc_mediaplayer, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerNothingSpecial, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerOpening, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerBuffering, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerPlaying, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerPaused, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerStopped, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerEndReached, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerEncounteredError, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		
		// Stop the current media
		libvlc_media_player_stop (self->libvlc_mediaplayer, &_vlcexcep);
		on_vlc_exception(self, &_vlcexcep);

		libvlc_media_player_release (self->libvlc_mediaplayer);
		self->libvlc_mediaplayer = NULL;
	}
#endif

	// Free the media list
	gtk_libvlc_media_player_clear_media_list (self);

	G_OBJECT_CLASS (gtk_libvlc_media_player_parent_class)->dispose (object);
	
}

static void
gtk_libvlc_media_player_destroy(GtkObject *object)
{
	 GtkLibVLCMediaPlayer *self;
	 
	 g_return_if_fail(object != NULL);
	 g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(object));
	 
	 self = GTK_LIBVLC_MEDIA_PLAYER(object);	 
	 
	 GTK_OBJECT_CLASS (gtk_libvlc_media_player_parent_class)->destroy (object);
}

static void
gtk_libvlc_media_player_initialize(GtkLibVLCMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	if(priv->initialized == FALSE){

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
                XID xid = gdk_x11_drawable_get_xid(GTK_WIDGET(self)->window);
                libvlc_video_set_parent(libvlc_instance, xid, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
#else
		XID xid = gdk_x11_drawable_get_xid(GTK_WIDGET(self)->window);
			
		self->libvlc_mediaplayer = libvlc_media_player_new (libvlc_instance, &_vlcexcep);
		on_vlc_exception(self, &_vlcexcep);
		
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
		libvlc_media_player_set_drawable (self->libvlc_mediaplayer, xid,
						  &_vlcexcep);
#else
		libvlc_media_player_set_xwindow (self->libvlc_mediaplayer, xid,
						 &_vlcexcep);
#endif
		on_vlc_exception(self, &_vlcexcep);
		
		// Attach events on the media player
		libvlc_event_manager_t *em;
		em = libvlc_media_player_event_manager (self->libvlc_mediaplayer, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerNothingSpecial, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerOpening, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerBuffering, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerPlaying, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerPaused, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerStopped, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerEndReached, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerEncounteredError, on_vlc_event, self, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
		//libvlc_event_attach (em, libvlc_MediaPlayerPositionChanged, on_vlc_event, self, &_vlcexcep);
                //on_vlc_exception (self, &_vlcexcep);
#endif
		priv->initialized = TRUE;	
	}
}

static void
gtk_libvlc_media_player_play_media(GtkLibVLCMediaPlayer *self, GtkLibVLCMedia *media, gchar **options)
{
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibVLCMediaPlayerPrivate *priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	gtk_libvlc_media_player_stop (self);

	//g_print("\n\nPlaying : %s\n\n", media->mrl);
	// Play the media
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	if(libvlc_playlist_items_count (libvlc_instance, &_vlcexcep) > 0){
		libvlc_playlist_delete_item(libvlc_instance, 0, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_playlist_clear(libvlc_instance, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}
	if(options != NULL){
		int i;
		for(i=0; i<g_strv_length(options); i++){

		}
		libvlc_playlist_add_extended(libvlc_instance, media->mrl, NULL,
					     g_strv_length(options), (const char**)options, &_vlcexcep);
	}else{
		libvlc_playlist_add (libvlc_instance, media->mrl, NULL, &_vlcexcep);		
	}
	on_vlc_exception (self, &_vlcexcep);

	if (libvlc_playlist_items_count (libvlc_instance, &_vlcexcep) > 0){;
                libvlc_playlist_play (libvlc_instance, -1, 0, 
                                      NULL, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
        }
#else
	
	libvlc_media_t *m;
	m = libvlc_media_new (libvlc_instance, media->mrl, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);

	if(options != NULL){
		int i=0;
		for(i=0; i<g_strv_length(options); i++){
			libvlc_media_add_option(m, g_strdup(options[i]), &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
		}
	}	
		
	libvlc_media_player_set_media (self->libvlc_mediaplayer, m, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
	libvlc_media_release (m);
	
	// Event on the media
        libvlc_event_manager_t *em;
        m = libvlc_media_player_get_media(self->libvlc_mediaplayer, &_vlcexcep);
        on_vlc_exception (self, &_vlcexcep);
        em = libvlc_media_event_manager (m, &_vlcexcep);
        on_vlc_exception (self, &_vlcexcep);
        libvlc_event_attach (em, libvlc_MediaSubItemAdded, on_vlc_event, self, &_vlcexcep);
        on_vlc_exception (self, &_vlcexcep);
	
	libvlc_media_player_play (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
#endif	
}

static void
gtk_libvlc_media_player_set_current_path(GtkLibVLCMediaPlayer *self, GtkTreePath *path)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	if(priv->current_media != path){
		if(priv->current_media != NULL){
			gtk_tree_path_free(priv->current_media);	
		}
		if(path != NULL){
			priv->current_media = gtk_tree_path_copy(path);
		}else{
			priv->current_media = NULL;
		}
	}
}

static void
on_vlc_exception(GtkLibVLCMediaPlayer *self, libvlc_exception_t * ex)
{	
	if (libvlc_exception_raised (ex)){
		//#if LIBVLC_VERSION_MAJOR == 1 && ((LIBVLC_VERSION_MINOR == 0 && LIBVLC_VERSION_REVISION >= 2) || LIBVLC_VERSION_MINOR > 0)
		// g_printerr("libvlc-gtk error: %s\n", libvlc_errmsg());
		//#else
		//g_printerr("libvlc-gtk error: %s\n", libvlc_exception_get_message(ex));
		//#endif
	}
}

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	
#else

static gboolean
idle_play_next_function(gpointer ptrdata){
	GtkLibVLCMediaPlayer *self;
	g_return_if_fail(ptrdata != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(ptrdata));
	
	self = GTK_LIBVLC_MEDIA_PLAYER(ptrdata);
	gtk_libvlc_media_player_play_next (self);
	
        return FALSE;
}

static void 
on_vlc_event(const libvlc_event_t *event, void *user_data)
{
	// g_print("event %s\n", libvlc_event_type_name (event->type));
	
	GtkLibVLCMediaPlayer *self;
	g_return_if_fail(user_data != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(user_data));
	 
	self = GTK_LIBVLC_MEDIA_PLAYER(user_data);

	GtkLibVLCMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_GET_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	if(event->type == libvlc_MediaSubItemAdded){		
		
		libvlc_media_t *current_m;
		libvlc_media_t *new_m;
		current_m = libvlc_media_player_get_media(self->libvlc_mediaplayer, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		
		libvlc_media_list_t *ml;
		ml = libvlc_media_subitems (current_m, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		if(ml != NULL){
			int mlcount = libvlc_media_list_count(ml, &_vlcexcep);
			if(mlcount > 0){
				// Read last element in the playlist
				new_m = libvlc_media_list_item_at_index(ml, mlcount-1, &_vlcexcep);
				on_vlc_exception (self, &_vlcexcep);
				gchar* mrl = libvlc_media_get_mrl(new_m,
								  &_vlcexcep);
				on_vlc_exception (self, &_vlcexcep);
				GtkLibVLCMedia *new_media;
				new_media = gtk_libvlc_media_new(mrl);
				
				GtkTreeIter iter1;
				GtkTreeIter iter2;
				
				gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter1, priv->current_media);
				
				if(priv->is_media_parsed == FALSE){
					gtk_tree_store_append (self->media_list, &iter2, &iter1);
					gtk_tree_store_set (GTK_TREE_STORE(self->media_list), &iter2,
							    GTK_LIBVLC_MODEL_MEDIA_COLUMN, new_media, -1);
				}
			}
		}
	}	

	if(event->type == libvlc_MediaPlayerEndReached){
		if(priv->play_next_at_end == TRUE){
			g_idle_add (idle_play_next_function, (gpointer)self);
		}
	}
}
#endif
