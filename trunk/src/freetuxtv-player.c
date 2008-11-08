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
		  libvlc_exception_t *ex);

GtkWidget *
freetuxtv_player_new ()
{
	FreetuxTVPlayer *self = NULL;
	self = gtk_type_new (freetuxtv_player_get_type ());
	self->volume = 70.00;

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
	libvlc_media_t *m;

	char *config_file;
	
	config_file = g_build_filename (g_get_user_config_dir(),
					"FreetuxTV/vlcrc", NULL);
	const char * const vlc_args[] = {
		"--config", config_file /* Use alternative VLC's config */,
		"--save-config"};
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

	// Création de l'instance VLC si elle n'existe pas
	if(self->libvlc.inst == NULL){		
		self->libvlc.inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
	}
	
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
	
	// Ajout de l'URL dans le player
	m = libvlc_media_new (self->libvlc.inst, uri, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_media_player_set_media (self->libvlc.mp, m, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);
	libvlc_media_release (m);

	// Lecture de la chaine
	libvlc_media_player_play (self->libvlc.mp, &_vlcexcep);
	on_vlc_exception (self, &_vlcexcep);

	g_free(config_file);

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
		// Arret de la chaine en cours de lecture
		libvlc_media_player_stop (self->libvlc.mp, &_vlcexcep);
		on_vlc_exception (self, &_vlcexcep);
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
