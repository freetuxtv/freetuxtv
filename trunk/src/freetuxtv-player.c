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

extern FreetuxTVApp *app;

static void 
on_vlc_exception (FreetuxTVPlayer *self, 
		  libvlc_exception_t *excp);

GtkWidget *
freetuxtv_player_new ()
{
	FreetuxTVPlayer *self = NULL;
	self = gtk_type_new (freetuxtv_player_get_type ());

	/* Widget contenant la vidéo */
	GdkColor color;
	color.pixel = 0;
	color.red   = 0x55 * 0x100;
	color.green = 0x55 * 0x100;
	color.blue  = 0x55 * 0x100;
	gtk_widget_modify_bg(GTK_WIDGET(self), GTK_STATE_NORMAL, &color);
	
	gtk_widget_show (GTK_WIDGET(self));
	
	return GTK_WIDGET(self);
	
}

void
freetuxtv_player_play (FreetuxTVPlayer *self, gchar *uri)
{
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	// Création de l'instance VLC si elle n'existe pas
	if(self->vlcinstance == NULL){
		self->vlcinstance=libvlc_new(0,NULL,&_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		XID xid = gdk_x11_drawable_get_xid(GTK_WIDGET(self)->window);
		libvlc_video_set_parent(self->vlcinstance, xid, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		libvlc_audio_set_volume (self->vlcinstance, 
                                         80.0, &_vlcexcep);    
                on_vlc_exception (self, &_vlcexcep);
	}
	
	// Arret de la chaine en cour de lecture
	if (libvlc_playlist_isplaying (self->vlcinstance,&_vlcexcep)) {
		on_vlc_exception (self, &_vlcexcep);
		libvlc_playlist_stop (self->vlcinstance,&_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}
	
	// Effacement de la playlist
	if ( libvlc_playlist_items_count (self->vlcinstance,&_vlcexcep) ){
		libvlc_playlist_clear (self->vlcinstance,&_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}
	
	// Ajout de l'URL dans la playlist
	libvlc_playlist_add (self->vlcinstance, uri , NULL, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);

	// Lecture de la chaine
	if ( libvlc_playlist_items_count (self->vlcinstance, &_vlcexcep) ){
		libvlc_playlist_play (self->vlcinstance, -1, 0, 
				      NULL, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}

	/*
	libvlc_log_t *log;
 	log = libvlc_log_open (self->vlcinstance, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);

	libvlc_log_iterator_t *logiter;
 	logiter = libvlc_log_get_iterator (log, &_vlcexcep);

	int logc = libvlc_log_iterator_has_next (logiter, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	while(logc == 1){
		struct libvlc_log_message_t logmsg;
		logmsg.sizeof_msg = sizeof(logmsg);
		libvlc_log_iterator_next (logiter, &logmsg, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
		g_printerr("FreetuxTV : Log VLC -> %s\n", logmsg.psz_message);
		logc = libvlc_log_iterator_has_next (logiter, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}

	libvlc_log_clear (log, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_log_iterator_free (logiter, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_log_close (log, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	*/
}

void
freetuxtv_player_set_volume (FreetuxTVPlayer *self, gdouble value)
{
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);
        if(self->vlcinstance != NULL){
                libvlc_audio_set_volume (self->vlcinstance, 
                                         value, &_vlcexcep);    
                on_vlc_exception (self, &_vlcexcep);
        }
}

void
freetuxtv_player_stop (FreetuxTVPlayer *self)
{
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);
        if(self->vlcinstance != NULL){
		// Arret de la chaine en cours de lecture
		if (libvlc_playlist_isplaying (self->vlcinstance, &_vlcexcep)) {
			on_vlc_exception (self, &_vlcexcep);
			libvlc_playlist_stop (self->vlcinstance, &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
		}
        }
}


void
freetuxtv_player_fullscreen (FreetuxTVPlayer *self)
{
	libvlc_input_t *input_t;
	libvlc_exception_t _vlcexcep;
        libvlc_exception_init (&_vlcexcep);
        if(self->vlcinstance != NULL){
		// Mode plein écran si la chaine est en cours de lecture
		if (libvlc_playlist_isplaying (self->vlcinstance, &_vlcexcep)) {
			input_t = libvlc_playlist_get_input(self->vlcinstance,
							    &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
			libvlc_set_fullscreen(input_t, 1, &_vlcexcep);
			on_vlc_exception (self, &_vlcexcep);
		}
        }
}

static void 
on_vlc_exception (FreetuxTVPlayer *self, 
		  libvlc_exception_t *excp)
{
	if(libvlc_exception_raised(excp)){
		g_printerr("VLC : Error %s\n", 
			   libvlc_exception_get_message(excp));
	}
}

static void
freetuxtv_player_init (FreetuxTVPlayer *object)
{
	object->vlcinstance=NULL;
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
