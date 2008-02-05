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

#ifndef FREETUXTV_PLAYER_H
#define FREETUXTV_PLAYER_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include <vlc/libvlc.h>

G_BEGIN_DECLS

#define FREETUXTV_TYPE_PLAYER            (freetuxtv_player_get_type ())
#define FREETUXTV_PLAYER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_PLAYER, FreetuxTVPlayer))
#define FREETUXTV_PLAYER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_PLAYER, FreetuxTVPlayerClass))
#define FREETUXTV_IS_PLAYER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_PLAYER))
#define FREETUXTV_IS_PLAYER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_PLAYER))
#define FREETUXTV_PLAYER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_PLAYER, FreetuxTVPlayerClass))

typedef struct _FreetuxTVPlayer FreetuxTVPlayer;
typedef struct _FreetuxTVPlayerClass FreetuxTVPlayerClass;

struct _FreetuxTVPlayer
{
	GtkDrawingArea parent;

	libvlc_instance_t *vlcinstance;
};

struct _FreetuxTVPlayerClass
{
	GtkDrawingAreaClass parent_class;
};

GType
freetuxtv_player_get_type (void);

GtkWidget *
freetuxtv_player_new ();

void
freetuxtv_player_play (FreetuxTVPlayer *self, gchar *uri);

G_END_DECLS

#endif /* FREETUXTV_PLAYER_H */
