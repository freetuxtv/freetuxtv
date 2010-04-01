/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtk-libvlc-media-player.h"

#include <gdk/gdkx.h>

typedef struct _GtkLibvlcMediaPlayerPrivate GtkLibvlcMediaPlayerPrivate;
struct _GtkLibvlcMediaPlayerPrivate
{
	gboolean initialized;

	GtkTreePath *current_media;
	gchar **current_options;
	
	gboolean is_media_parsed;
	gboolean play_next_at_end;
	gboolean loop;
};

#define GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_LIBVLC_MEDIA_PLAYER, GtkLibvlcMediaPlayerPrivate))


G_DEFINE_TYPE (GtkLibvlcMediaPlayer, gtk_libvlc_media_player, GTK_TYPE_WIDGET);

static void
on_vlc_exception(GtkLibvlcMediaPlayer *self, libvlc_exception_t * ex);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
#else
static void 
on_vlc_event(const libvlc_event_t *event, void *user_data);
#endif

static void
gtk_libvlc_media_player_init (GtkLibvlcMediaPlayer *object)
{
	object->libvlc_instance = NULL;
	
	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(object);
	
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
#else
	object->libvlc_mediaplayer = NULL;
	priv->is_media_parsed = FALSE;
#endif

	object->media_list = NULL;
	
	priv->initialized = FALSE;
	priv->play_next_at_end = TRUE;
	priv->loop = FALSE;

	priv->current_options = NULL;
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
	GtkLibvlcMediaPlayer *libvlcmp = GTK_LIBVLC_MEDIA_PLAYER (widget);
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
gtk_libvlc_media_player_finalize (GObject *object)
{
	GtkLibvlcMediaPlayer *self;

	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	g_return_if_fail(object != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(object));
	 
	self = GTK_LIBVLC_MEDIA_PLAYER(object);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
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

	if(priv->current_options != NULL){
		g_strfreev (priv->current_options);
		priv->current_options = NULL;
	}

	// Free the media list
	gtk_libvlc_media_player_clear_media_list (self);

	G_OBJECT_CLASS (gtk_libvlc_media_player_parent_class)->finalize (object);
}

static void
gtk_libvlc_media_player_class_init (GtkLibvlcMediaPlayerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass* parent_class = GTK_WIDGET_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkLibvlcMediaPlayerPrivate));

	object_class->finalize = gtk_libvlc_media_player_finalize;
	
	parent_class->realize = gtk_libvlc_media_player_realize;
	parent_class->size_request = gtk_libvlc_media_player_size_request;
	parent_class->size_allocate = gtk_libvlc_media_player_size_allocate;
}

static void
on_vlc_exception(GtkLibvlcMediaPlayer *self, libvlc_exception_t * ex)
{	
	if (libvlc_exception_raised (ex)){
		/*
#if LIBVLC_VERSION_MAJOR == 1 && ((LIBVLC_VERSION_MINOR == 0 && LIBVLC_VERSION_REVISION >= 2) || LIBVLC_VERSION_MINOR > 0)
		g_printerr("libvlc-gtk error: %s\n", libvlc_errmsg());
#else
		*/
		g_printerr("libvlc-gtk error: %s\n", libvlc_exception_get_message(ex));
		/*
#endif
		*/
	}
}

static void
gtk_libvlc_media_player_initialize(GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
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
gtk_libvlc_media_player_play_media(GtkLibvlcMediaPlayer *self, GtkLibvlcMedia *media, gchar **options)
{
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibvlcMediaPlayerPrivate *priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	int nb_options;
	int nb_m_options = 0; // Number of media options
	int nb_mp_options = 0; // Number of media player options
	gchar** list_options = NULL;
	int i;

	gtk_libvlc_media_player_stop (self);

	// g_print("\n\nPlaying : %s\n\n", media->mrl);
	// Delete options attached to the media player
	if(priv->current_options != options){
		if(priv->current_options != NULL){
			g_strfreev (priv->current_options);
			priv->current_options = NULL;
		}
		if(options == NULL){
			priv->current_options = NULL;
		}else{
			priv->current_options = g_strdupv (options);
		}
	}

	// Merge options in only one tab
	gchar** media_options;
	media_options = (gchar**)gtk_libvlc_media_get_options(media);
	if(media_options != NULL){
		nb_m_options += g_strv_length(media_options);
	}
	if(priv->current_options != NULL){
		nb_mp_options += g_strv_length(priv->current_options);
	}
	nb_options = nb_m_options + nb_mp_options;
	if(nb_options > 0){
		list_options = (gchar**)g_malloc((nb_options+1) * sizeof(gchar*));
		list_options[nb_options] = NULL;
		int step = 0;
		// We put first the media option
		for(i=0; i<nb_m_options; i++){
			list_options[step+i] = g_strdup(media_options[i]);
		}
		step += nb_m_options;
		// After we put the media player option
		for(i=0; i<nb_mp_options; i++){
			list_options[step+i] = g_strdup(priv->current_options[i]);
		}
	}


	// Play the media
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	if(libvlc_playlist_items_count (libvlc_instance, &_vlcexcep) > 0){
		libvlc_playlist_delete_item(libvlc_instance, 0, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_playlist_clear(libvlc_instance, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}
	if(list_options != NULL){
		/*
		int i;
		for(i=0; i<nb_options; i++){
			g_print("option[%d] = %s\n", i, list_options[i]);
		}
		*/
		libvlc_playlist_add_extended(libvlc_instance, media->mrl, NULL,
					     nb_options, (const char**)list_options, &_vlcexcep);
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

	if(list_options != NULL){
		int i=0;
		for(i=0; i<nb_options; i++){
			libvlc_media_add_option(m, g_strdup(list_options[i]), &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
			//g_print("option_copy[%d] = %s\n", i, list_options[i]);
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

	// Free the options tab
	if(list_options){
		g_strfreev(list_options);
		list_options=NULL;
	}
}

static void
gtk_libvlc_media_player_set_current_path(GtkLibvlcMediaPlayer *self, GtkTreePath *path)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
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

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	
#else

static gboolean
idle_play_next_function(gpointer ptrdata){
	GtkLibvlcMediaPlayer *self;
	g_return_if_fail(ptrdata != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(ptrdata));
	
	self = GTK_LIBVLC_MEDIA_PLAYER(ptrdata);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	gtk_libvlc_media_player_play_next (self, priv->current_options);
	
    return FALSE;
}

static void 
on_vlc_event(const libvlc_event_t *event, void *user_data)
{
	//g_print("event %s\n", libvlc_event_type_name (event->type));
	
	GtkLibvlcMediaPlayer *self;
	g_return_if_fail(user_data != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(user_data));
	 
	self = GTK_LIBVLC_MEDIA_PLAYER(user_data);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	if(event->type == libvlc_MediaSubItemAdded){		
		
		libvlc_media_t *new_m;
		new_m = event->u.media_subitem_added.new_child;
		
		if(new_m != NULL){
			gchar* mrl = libvlc_media_get_mrl(new_m,
							  &_vlcexcep);
			GtkLibvlcMedia *new_media;
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

	if(event->type == libvlc_MediaPlayerEndReached){
		if(priv->play_next_at_end == TRUE){
			g_idle_add (idle_play_next_function, (gpointer)self);
		}
	}
}
#endif


GtkWidget *
gtk_libvlc_media_player_new (GtkLibvlcInstance* libvlc_instance)
{
	g_return_if_fail(libvlc_instance != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_INSTANCE(libvlc_instance));
	
	GtkLibvlcMediaPlayer *self = NULL;
	self = gtk_type_new (GTK_TYPE_LIBVLC_MEDIA_PLAYER);

	self->libvlc_instance = libvlc_instance;
	g_object_ref(G_OBJECT(self->libvlc_instance));

	self->media_list = gtk_tree_store_new(GTK_LIBVLC_MODEL_NB_COLUMN, GTK_TYPE_LIBVLC_MEDIA);
	
	return GTK_WIDGET(self);
}

void
gtk_libvlc_media_player_add_media (GtkLibvlcMediaPlayer *self, GtkLibvlcMedia *media)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
	// Add the media in the media list
	GtkTreeIter iter;
	gtk_tree_store_append (self->media_list, &iter, NULL);
	gtk_tree_store_set (GTK_TREE_STORE(self->media_list), &iter,
			    GTK_LIBVLC_MODEL_MEDIA_COLUMN, media, -1);
}

GtkLibvlcMedia*
gtk_libvlc_media_player_get_current_media (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	return gtk_libvlc_media_player_get_media_from_path(self, priv->current_media);
}

GtkLibvlcMedia*
gtk_libvlc_media_player_get_media_from_path (GtkLibvlcMediaPlayer *self, GtkTreePath *path)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(path != NULL);
	
	// Get the media from the path
	GtkLibvlcMedia *media;
	GtkTreeIter iter;
	if(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter, path)){
		gtk_tree_model_get (GTK_TREE_MODEL(self->media_list),
				    &iter, 0, &media, -1);
		return media;
	}

	return NULL;
}

void
gtk_libvlc_media_player_clear_media_list(GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	gtk_tree_store_clear (self->media_list);
	
	gtk_libvlc_media_player_set_current_path(self, NULL);
}

void
gtk_libvlc_media_player_play (GtkLibvlcMediaPlayer *self, gchar **options)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

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
	gtk_libvlc_media_player_play_media_at_path(self, priv->current_media, options);
}

void
gtk_libvlc_media_player_play_media_at_path (GtkLibvlcMediaPlayer *self, GtkTreePath *path, gchar **options)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	g_return_if_fail(path != NULL);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);
	
	gtk_libvlc_media_player_set_current_path(self, path);
	
	GtkLibvlcMedia *media;
	media = gtk_libvlc_media_player_get_media_from_path (self, priv->current_media);
	
	GtkTreeIter iter;
	g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter, priv->current_media) == TRUE);
	if(gtk_tree_model_iter_has_child(GTK_TREE_MODEL(self->media_list), &iter)){
		priv->is_media_parsed = TRUE;
	}else{
		priv->is_media_parsed = FALSE;
	}
	
	// Play the media
	gtk_libvlc_media_player_play_media (self, media, options);
}

void
gtk_libvlc_media_player_play_next (GtkLibvlcMediaPlayer *self, gchar **options)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);
	
	GtkTreePath* path = NULL;
	if(priv->current_media != NULL){
		
		GtkTreeIter iter1;
		GtkTreeIter iter2;
		g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter1, priv->current_media) == TRUE);
		if(gtk_tree_model_iter_has_child(GTK_TREE_MODEL(self->media_list), &iter1)){
			gtk_tree_model_iter_children(GTK_TREE_MODEL(self->media_list), &iter2, &iter1);
			path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter2);
		}else{
			if(gtk_tree_model_iter_next (GTK_TREE_MODEL(self->media_list), &iter1)){
				path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
			}else{
				gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter1, priv->current_media);
				if(gtk_tree_model_iter_parent (GTK_TREE_MODEL(self->media_list), &iter2, &iter1)){
					if(gtk_tree_model_iter_next (GTK_TREE_MODEL(self->media_list), &iter2)){
						path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter2);
					}else{
						if(priv->loop){
							gtk_tree_model_get_iter_first(GTK_TREE_MODEL(self->media_list), &iter1);
							path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
						}
					}
				}else{
					if(priv->loop){
						gtk_tree_model_get_iter_first(GTK_TREE_MODEL(self->media_list), &iter1);
						path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
					}
				}
			}
		}

		// Play the next media
		if(path != NULL){
			gtk_libvlc_media_player_play_media_at_path(self, path, options);
		}
	}else{
		// Play the first media
		if(priv->loop){
			gtk_libvlc_media_player_play(self, options);
		}
	}	
}

void
gtk_libvlc_media_player_pause (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_playlist_pause (libvlc_instance, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
#else
	libvlc_media_player_pause (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
#endif
	
}

gboolean
gtk_libvlc_media_player_can_pause (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);

	gboolean ret = FALSE;

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	ret = TRUE;
#else
	ret = libvlc_media_player_can_pause (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
#endif

	return ret;

}

void
gtk_libvlc_media_player_stop (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
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
}

void
gtk_libvlc_media_player_set_volume (GtkLibvlcMediaPlayer *self, gdouble volume)
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
gtk_libvlc_media_player_get_volume (GtkLibvlcMediaPlayer *self)
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
	return (gdouble)volume/100;
}

void
gtk_libvlc_media_player_set_fullscreen (GtkLibvlcMediaPlayer *self, gboolean fullscreen)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8	
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
	if (libvlc_playlist_isplaying (libvlc_instance, &_vlcexcep)) {
		input_t = libvlc_playlist_get_input(libvlc_instance,
						    &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_set_fullscreen(input_t, fullscreen, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_input_free(input_t);
	}
#else
        if(self->libvlc_mediaplayer != NULL){
                libvlc_set_fullscreen (self->libvlc_mediaplayer, fullscreen, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
        }
#endif

}

gboolean
gtk_libvlc_media_player_is_playing (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	int res;

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);
	
	res = libvlc_playlist_isplaying(libvlc_instance, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	return (res == 1);
#elif LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
	return (gtk_libvlc_media_player_get_state(self)==GTK_LIBVLC_STATE_PLAYING);
#else
	res = libvlc_media_player_is_playing (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
	return (res == 1);
#endif
}

GtkLibvlcState
gtk_libvlc_media_player_get_state (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self);
	g_return_if_fail(priv->initialized == TRUE);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	GtkLibvlcState gtkstate = GTK_LIBVLC_STATE_NOTHING_SPECIAL;

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
gtk_libvlc_media_player_state_tostring (GtkLibvlcState state)
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

glong
gtk_libvlc_media_player_get_length(GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));	
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	glong length = 0;

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
					    &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	length = libvlc_input_get_length(input_t, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_input_free(input_t);
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
	if(media != NULL){
		length = libvlc_media_player_get_length (self->libvlc_mediaplayer, &_vlcexcep);
		on_vlc_exception(self, &_vlcexcep);
	}
#endif
	return length;
}

glong
gtk_libvlc_media_player_get_time(GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));	
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	glong time = 0;

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
					    &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	time = libvlc_input_get_time(input_t, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_input_free(input_t);
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
	if(media != NULL){
		time = libvlc_media_player_get_time (self->libvlc_mediaplayer, &_vlcexcep);
		on_vlc_exception(self, &_vlcexcep);
	}
#endif
	return time;
}

void
gtk_libvlc_media_player_set_time(GtkLibvlcMediaPlayer *self, glong time)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));	
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
					    &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_input_set_time(input_t, time, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_input_free(input_t);

#else
	libvlc_media_player_set_time (self->libvlc_mediaplayer, time, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
#endif	
}

gfloat
gtk_libvlc_media_player_get_position(GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));	
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	gfloat pos = 0.0;

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
					    &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	pos = libvlc_input_get_position(input_t, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_input_free(input_t);
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
	if(media != NULL){
		pos = libvlc_media_player_get_position (self->libvlc_mediaplayer, &_vlcexcep);
		on_vlc_exception(self, &_vlcexcep);
	}
#endif
	return pos;
	
}

void
gtk_libvlc_media_player_set_position(GtkLibvlcMediaPlayer *self, gfloat position)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	g_return_if_fail(position>=0.0 && position<=1.0);	
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = self->libvlc_instance->libvlc_instance;
	g_return_if_fail(libvlc_instance != NULL);

        libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
					    &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_input_set_position(input_t, position, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_input_free(input_t);

#else
	libvlc_media_player_set_position (self->libvlc_mediaplayer, position, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
#endif	
}

gboolean
gtk_libvlc_media_player_is_seekable (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));	
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	gboolean ret = FALSE;

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	ret = TRUE;
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (self->libvlc_mediaplayer, &_vlcexcep);
	on_vlc_exception(self, &_vlcexcep);
	if(media != NULL){
		ret = libvlc_media_player_is_seekable (self->libvlc_mediaplayer, &_vlcexcep);
		on_vlc_exception(self, &_vlcexcep);
	}
#endif
	return ret;
}

void
gtk_libvlc_media_player_set_play_next_at_end (GtkLibvlcMediaPlayer *self, gboolean b)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	priv->play_next_at_end = b;	
}

void
gtk_libvlc_media_player_set_loop (GtkLibvlcMediaPlayer *self, gboolean b)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	priv->loop = b;	
}

GtkLibvlcInstance*
gtk_libvlc_media_player_get_instance (GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	return self->libvlc_instance;
}
