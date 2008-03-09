/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <gdk/gdkx.h>

#include "freetuxtv-player.h"

G_DEFINE_TYPE (FreetuxTVPlayer, freetuxtv_player, GTK_TYPE_DRAWING_AREA);

static void 
freetuxtv_player_on_vlc_exception (FreetuxTVPlayer *self, 
								   libvlc_exception_t *excp);

GtkWidget *
freetuxtv_player_new ()
{
	FreetuxTVPlayer * player = NULL;
	player = gtk_type_new (freetuxtv_player_get_type ());
	gtk_widget_show (GTK_WIDGET(player));
	return GTK_WIDGET(player);
	
}

void
freetuxtv_player_play (FreetuxTVPlayer *self, gchar *uri)
{
	libvlc_exception_t _vlcexcep;

	libvlc_exception_init (&_vlcexcep);

	// Création de l'instance VLC si elle n'existe pas
	if(self->vlcinstance == NULL){
		self->vlcinstance=libvlc_new(0,NULL,&_vlcexcep);
		freetuxtv_player_on_vlc_exception (self, &_vlcexcep);
		XID xid = gdk_x11_drawable_get_xid(GTK_WIDGET(self)->window);
		libvlc_video_set_parent(self->vlcinstance, xid, &_vlcexcep);
		freetuxtv_player_on_vlc_exception (self, &_vlcexcep);
	}
	
	// Arret de la chaine en cour de lecture
	if (libvlc_playlist_isplaying (self->vlcinstance,&_vlcexcep)) {
		freetuxtv_player_on_vlc_exception (self, &_vlcexcep);
		libvlc_playlist_stop (self->vlcinstance,&_vlcexcep);
		freetuxtv_player_on_vlc_exception (self, &_vlcexcep);
	}
	
	// Effacement de la playlist
	if ( libvlc_playlist_items_count (self->vlcinstance,&_vlcexcep) ){
		libvlc_playlist_clear (self->vlcinstance,&_vlcexcep);
		freetuxtv_player_on_vlc_exception (self, &_vlcexcep);
	}
	
	// Ajout de l'URL dans la playlist
	libvlc_playlist_add (self->vlcinstance, uri , NULL, &_vlcexcep);
	freetuxtv_player_on_vlc_exception (self, &_vlcexcep);

	// Lecture de la chaine
	if ( libvlc_playlist_items_count (self->vlcinstance,&_vlcexcep) ){
		libvlc_playlist_play (self->vlcinstance, -1, 0, NULL, &_vlcexcep);
		freetuxtv_player_on_vlc_exception (self, &_vlcexcep);
	}

}

static void 
freetuxtv_player_on_vlc_exception (FreetuxTVPlayer *self, 
								   libvlc_exception_t *excp)
{
	if(libvlc_exception_raised(excp)){
	    g_print("VLC : Error %s\n",libvlc_exception_get_message(excp));
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
