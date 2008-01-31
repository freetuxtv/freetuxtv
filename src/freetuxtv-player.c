/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with brasero.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include <malloc.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include "freetuxtv-player.h"

FreetuxTVPlayerClass *
freetuxtv_player_new ()
{
	FreetuxTVPlayerClass * self;
	self = (FreetuxTVPlayerClass *)malloc(sizeof(FreetuxTVPlayerClass *));
	
	/* Creation du widget */
	self->widget = gtk_drawing_area_new ();
 
	return self;
	
}

void
freetuxtv_player_init (FreetuxTVPlayerClass *self)
{
	/* Création d'une instance VLC */
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	self->vlcinstance=libvlc_new(0,NULL,&_vlcexcep);
	XID xid = gdk_x11_drawable_get_xid(self->widget->window);
	libvlc_video_set_parent(self->vlcinstance, xid, &_vlcexcep);	
}

void
freetuxtv_player_play (FreetuxTVPlayerClass *self, gchar *uri)
{
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
	libvlc_playlist_clear (self->vlcinstance,&_vlcexcep);
	libvlc_playlist_add (self->vlcinstance,uri,NULL,&_vlcexcep);
	libvlc_playlist_play (self->vlcinstance , -1, 0, NULL, &_vlcexcep);
}
