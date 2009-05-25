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
#include <glade/glade.h>

#include "freetuxtv-app.h"
#include "freetuxtv-player.h"

G_DEFINE_TYPE (FreetuxTVPlayer, freetuxtv_player, GTK_TYPE_DRAWING_AREA);

static void 
on_vlc_exception (FreetuxTVPlayer *self, 
		  libvlc_exception_t *ex);


#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
typedef struct _IdlePlayParam {
	FreetuxTVPlayer *self;
	FreetuxTVChannelInfos *channel_infos;
} IdlePlayParam;

static gboolean libvlc_idle_play_function(gpointer ptrdata){
	IdlePlayParam *play_param = (IdlePlayParam*)ptrdata;
	freetuxtv_player_play (play_param->self, play_param->channel_infos);
	return FALSE;
}

static void 
event(const libvlc_event_t * event, void * user_data);
#endif

GtkWidget *
freetuxtv_player_new ()
{
	FreetuxTVPlayer *self = NULL;
	self = gtk_type_new (freetuxtv_player_get_type ());
	self->volume = 70.00;

	// Widget contenant la vidéo
	GdkColor color;
	color.pixel = 0;
	color.red   = 0x55 * 0x100;
	color.green = 0x55 * 0x100;
	color.blue  = 0x55 * 0x100;
	gtk_widget_modify_bg(GTK_WIDGET(self), GTK_STATE_NORMAL, &color);
	
	// Création de l'instance VLC si elle n'existe pas
	char *config_file;
	config_file = g_build_filename (g_get_user_config_dir(),
					"FreetuxTV/vlcrc", NULL);
	const char * const vlc_args[] = {
		"--config", config_file /* Use alternative VLC's config */,
		"--save-config", "--verbose=0"};
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	if(self->libvlc.inst == NULL){		
		self->libvlc.inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
		// Création de l'instance VLC si elle n'existe pas
                XID xid = gdk_x11_drawable_get_xid(GTK_WIDGET(self)->window);
                libvlc_video_set_parent(self->libvlc.inst, xid, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
                libvlc_audio_set_volume (self->libvlc.inst, 
                                         self->volume, &_vlcexcep);    
                on_vlc_exception (self, &_vlcexcep);		
#endif

	}
	g_free(config_file);
  
	gtk_widget_show (GTK_WIDGET(self));
	
	return GTK_WIDGET(self);
}

void
freetuxtv_player_play (FreetuxTVPlayer *self, FreetuxTVChannelInfos *channel_infos)
{
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
	libvlc_media_t *m;

	// Création du mediaplayer s'il n'existe pas
	if(self->libvlc.mp == NULL){
		XID xid = gdk_x11_drawable_get_xid(GTK_WIDGET(self)->window);

		self->libvlc.mp = libvlc_media_player_new (self->libvlc.inst,
							   &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_media_player_set_drawable (self->libvlc.mp, xid,
						  &_vlcexcep);
		
		libvlc_audio_set_volume (self->libvlc.inst, 
					 self->volume, &_vlcexcep);    
                on_vlc_exception (self, &_vlcexcep);
	}

	g_object_ref(channel_infos);

	// Arrete la chaine en cours s'il y en a une
	freetuxtv_player_stop(self);

	// Ajout de l'URL dans le player
	if(channel_infos->redirect_url != NULL){
		g_print ("FreetuxTV : Launching redirected channel \"%s\" -> %s\n",
			 channel_infos->name, channel_infos->url);
		m = libvlc_media_new (self->libvlc.inst, channel_infos->redirect_url, &_vlcexcep);
	}else{
		g_print ("FreetuxTV : Launching channel \"%s\" -> %s\n",
			 channel_infos->name, channel_infos->url);
		m = libvlc_media_new (self->libvlc.inst, channel_infos->url, &_vlcexcep);
	}
	on_vlc_exception (self, &_vlcexcep);
	libvlc_media_set_user_data (m, channel_infos, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
		
	libvlc_media_player_set_media (self->libvlc.mp, m, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_media_release (m);
	
	// Evenement sur le media
	libvlc_event_manager_t *em;
	m = libvlc_media_player_get_media(self->libvlc.mp, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
 	em = libvlc_media_event_manager (m, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	// Pour gerer si le media est une playlist
	libvlc_event_attach (em, libvlc_MediaSubItemAdded, event, self, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	
	// Lecture de la chaine
	libvlc_media_player_play (self->libvlc.mp, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
#endif

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8        
        // Arret de la chaine en cour de lecture
        if (libvlc_playlist_isplaying (self->libvlc.inst,&_vlcexcep)) {
                on_vlc_exception (self, &_vlcexcep);
                libvlc_playlist_stop (self->libvlc.inst,&_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
        }
        
        // Effacement de la playlist
        if ( libvlc_playlist_items_count (self->libvlc.inst,&_vlcexcep) ){
                libvlc_playlist_clear (self->libvlc.inst,&_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
        }
        
        // Ajout de l'URL dans la playlist
	g_print ("FreetuxTV : Launching channel \"%s\" -> %s\n",
		 channel_infos->name, channel_infos->url);
        libvlc_playlist_add (self->libvlc.inst, channel_infos->url, NULL, &_vlcexcep);
        on_vlc_exception (self, &_vlcexcep);

        // Lecture de la chaine
        if ( libvlc_playlist_items_count (self->libvlc.inst, &_vlcexcep) ){
                libvlc_playlist_play (self->libvlc.inst, -1, 0, 
                                      NULL, &_vlcexcep);
                on_vlc_exception (self, &_vlcexcep);
        }
#endif

}

void
freetuxtv_player_set_volume (FreetuxTVPlayer *self, gdouble value)
{
	libvlc_exception_t _vlcexcep;
        
	self->volume = value;
	
	libvlc_exception_init (&_vlcexcep);
	if(self->libvlc.inst != NULL){
                libvlc_audio_set_volume (self->libvlc.inst, 
                                         (gint)value, &_vlcexcep);    
                on_vlc_exception (self, &_vlcexcep);
        }
}

gdouble
freetuxtv_player_get_volume (FreetuxTVPlayer *self)
{
	return self->volume;
}

void
freetuxtv_player_stop (FreetuxTVPlayer *self)
{
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);
	
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
        if(self->libvlc.mp != NULL){
		libvlc_media_t *m;
		m = libvlc_media_player_get_media(self->libvlc.mp, &_vlcexcep);
		if(m != NULL){
			// Libere la memoire
			libvlc_media_t *current_m;
			current_m = libvlc_media_player_get_media(self->libvlc.mp, &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
			FreetuxTVChannelInfos *channel_infos;
			channel_infos = libvlc_media_get_user_data (current_m, &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
			g_print ("FreetuxTV : Stopping current channel \"%s\" -> %s\n",
				 channel_infos->name, channel_infos->url);			
			g_object_unref(channel_infos);
			
			// Arret de la chaine en cours de lecture
			libvlc_media_player_stop (self->libvlc.mp, &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);

			self->is_recording = FALSE;
		}
        }
#endif

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
        if(self->libvlc.inst != NULL){
                // Arret de la chaine en cours de lecture
                if (libvlc_playlist_isplaying (self->libvlc.inst, &_vlcexcep)) {
                        on_vlc_exception (self, &_vlcexcep);
                        libvlc_playlist_stop (self->libvlc.inst, &_vlcexcep);
                        on_vlc_exception (self, &_vlcexcep);
                }
        }
#endif

}

void
freetuxtv_player_record_current (FreetuxTVPlayer *self, gchar* directory_record, gchar** out_filename)
{

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);
        if(self->libvlc.mp != NULL){
		// Enregistrement de la chaine en cours
		libvlc_media_t *m;
		m = libvlc_media_player_get_media (self->libvlc.mp, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		FreetuxTVChannelInfos *channel_infos;
		channel_infos = libvlc_media_get_user_data (m, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);

		GTimeVal now;
		g_get_current_time(&now);
		*out_filename = g_strconcat(directory_record, "/", channel_infos->name, " - ",
					    g_time_val_to_iso8601(&now), ".mpg", NULL);

		g_print ("FreetuxTV : Recording channel \"%s\" -> %s (in %s)\n",
			 channel_infos->name, channel_infos->url, *out_filename);

		gchar *options = g_strconcat(":sout=#duplicate{dst=display,dst=std{access=file,mux=ts,dst='",
					     *out_filename, "'}}", NULL);

		libvlc_media_add_option(m, options, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_media_player_set_media (self->libvlc.mp, m, &_vlcexcep);
		libvlc_media_player_play (self->libvlc.mp, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		self->is_recording = TRUE;
		g_free(options);
        }
#endif
}

void
freetuxtv_player_fullscreen (FreetuxTVPlayer *self)
{
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
        if(self->libvlc.mp != NULL){
		libvlc_set_fullscreen (self->libvlc.mp, 1, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
        }
#endif

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	libvlc_input_t *input_t;
        if(self->vlcinstance != NULL){
                if (libvlc_playlist_isplaying (self->libvlc.inst, &_vlcexcep)) {
                        input_t = libvlc_playlist_get_input(self->libvlc.inst,
                                                            &_vlcexcep);
                        on_vlc_exception (self, &_vlcexcep);
                        libvlc_set_fullscreen(input_t, 1, &_vlcexcep);
                        on_vlc_exception (self, &_vlcexcep);
                }
        }
#endif

}

gboolean
freetuxtv_player_is_playing(FreetuxTVPlayer *self)
{
	gboolean result = FALSE;
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
	libvlc_state_t state;
	state = libvlc_media_player_get_state (self->libvlc.mp, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	if(state == libvlc_Opening){
		result = TRUE;
	}
	if(state == libvlc_Buffering){
		result = TRUE;
	}
	if(state == libvlc_Playing){
		result = TRUE;
	}
#endif

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	int is_playing;
	is_playing = libvlc_playlist_isplaying (self->libvlc.inst, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	if(is_playing){
		result = TRUE;		
	}
#endif

	return result;
}

gboolean
freetuxtv_player_is_recording(FreetuxTVPlayer *self)
{
	return self->is_recording;
}

static void 
on_vlc_exception (FreetuxTVPlayer *self, 
		  libvlc_exception_t *ex)
{
	if(libvlc_exception_raised(ex)){
		g_printerr("VLC : Error %s\n", 
			   libvlc_exception_get_message(ex));
	}
}

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
static void 
event(const libvlc_event_t * event, void * user_data){
	FreetuxTVPlayer *self = (FreetuxTVPlayer*) user_data;
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	
	FreetuxTVChannelInfos *channel_infos;
	libvlc_media_t *current_m;
	libvlc_media_t *new_m;
	current_m = libvlc_media_player_get_media(self->libvlc.mp, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	channel_infos = libvlc_media_get_user_data (current_m, &_vlcexcep);

	libvlc_media_list_t *ml;
 	ml = libvlc_media_subitems (current_m, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	if(ml != NULL){
		int mlcount = libvlc_media_list_count(ml, &_vlcexcep);
		if(mlcount > 0){
			// Lit le premier element de la media_list
			new_m = libvlc_media_list_item_at_index(ml, 0, &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
			channel_infos->redirect_url = libvlc_media_get_mrl(new_m,
									   &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
			
			IdlePlayParam *play_param = g_new0(IdlePlayParam, 1);
			play_param->self = self;
			play_param->channel_infos = channel_infos;
			// Necessaire pour envoyer une action dans le MainLoop de GTK
			g_idle_add (libvlc_idle_play_function, play_param);
		}	
	}
	
}
#endif

static void
freetuxtv_player_init (FreetuxTVPlayer *object)
{
	object->libvlc.inst = NULL;
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 9
	object->libvlc.mp = NULL;
#endif
	object->volume = 70.00;
	object->is_recording = FALSE;
}

static void
freetuxtv_player_finalize (GObject *object)
{
	
}

static void
freetuxtv_player_class_init (FreetuxTVPlayerClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_player_finalize;
}
