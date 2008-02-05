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

#include <gtk/gtk.h>

#include "freetuxtv-player.h"

typedef struct _FreetuxTVChannel FreetuxTVChannel;

struct _FreetuxTVChannel
{
	GtkWidget *widget;
	char *name;
	char *uri;
	GtkWidget *logo;
	FreetuxTVPlayer *player;
};

FreetuxTVChannel *
freetuxtv_channel_new (gchar *name, gchar *uri);

void
freetuxtv_channel_set_logo (FreetuxTVChannel *self, gchar *file);

void
freetuxtv_channel_set_player (FreetuxTVChannel *self, FreetuxTVPlayer *player);

void
freetuxtv_channel_play (FreetuxTVChannel *self);
