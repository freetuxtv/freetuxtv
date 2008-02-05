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

#ifndef FREETUXTV_CHANNEL_H
#define FREETUXTV_CHANNEL_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-player.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_CHANNEL            (freetuxtv_channel_get_type ())
#define FREETUXTV_CHANNEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNEL, FreetuxTVChannel))
#define FREETUXTV_CHANNEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNEL, FreetuxTVChannelClass))
#define FREETUXTV_IS_CHANNEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNEL))
#define FREETUXTV_IS_CHANNEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNEL))
#define FREETUXTV_CHANNEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNEL, FreetuxTVChannelClass))

typedef struct _FreetuxTVChannel FreetuxTVChannel;
typedef struct _FreetuxTVChannelClass FreetuxTVChannelClass;

struct _FreetuxTVChannel
{
	GtkButton parent;

	char *name;
	char *uri;
	GtkWidget *logo;
	FreetuxTVPlayer *player;
};

struct _FreetuxTVChannelClass
{
	GtkButtonClass parent_class;
};

GType
freetuxtv_channel_get_type (void);

GtkWidget *
freetuxtv_channel_new (gchar *name, gchar *uri);

void
freetuxtv_channel_set_logo (FreetuxTVChannel *self, gchar *file);

void
freetuxtv_channel_set_player (FreetuxTVChannel *self, FreetuxTVPlayer *player);

void
freetuxtv_channel_play (FreetuxTVChannel *self);

G_END_DECLS

#endif /* FREETUXTV_CHANNEL_H */
