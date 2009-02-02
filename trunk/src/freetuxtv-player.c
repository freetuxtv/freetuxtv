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
	}
	g_free(config_file);
  
	gtk_widget_show (GTK_WIDGET(self));
	
	return GTK_WIDGET(self);
}

void
freetuxtv_player_play (FreetuxTVPlayer *self, FreetuxTVChannelInfos *channel_infos)
{
	libvlc_media_t *m;

	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

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
		}		
        }
}

void
freetuxtv_player_record_current (FreetuxTVPlayer *self, gchar** out_filename)
{
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

		*out_filename = g_strconcat("/tmp/", channel_infos->name, ".mpg", NULL);

		g_print ("FreetuxTV : Recording channel \"%s\" -> %s (in %s)\n",
			 channel_infos->name, channel_infos->url, *out_filename);

		gchar *options = g_strconcat(":sout=#duplicate{dst=display,dst=std{access=file,mux=ts,dst='",
					     *out_filename, "'}}", NULL);

		libvlc_media_add_option(m, options, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_media_player_set_media (self->libvlc.mp, m, &_vlcexcep);
		libvlc_media_player_play (self->libvlc.mp, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);

		g_free(options);
        }
}

void
freetuxtv_player_fullscreen (FreetuxTVPlayer *self)
{
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);
        if(self->libvlc.mp != NULL){
		libvlc_set_fullscreen (self->libvlc.mp, 1, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
        }
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

static void
freetuxtv_player_init (FreetuxTVPlayer *object)
{
	object->libvlc.inst=NULL;
	object->libvlc.mp=NULL;
	object->volume=70.00;
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
