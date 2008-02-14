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

#ifndef FREETUXTV_MAIN_WINDOW_H
#define FREETUXTV_MAIN_WINDOW_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-channels-list.h"
#include "freetuxtv-player.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_MAIN_WINDOW            (freetuxtv_main_window_get_type ())
#define FREETUXTV_MAIN_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_MAIN_WINDOW, FreetuxTVMainWindow))
#define FREETUXTV_MAIN_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_MAIN_WINDOW, FreetuxTVMainWindowClass))
#define FREETUXTV_IS_MAIN_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_MAIN_WINDOW))
#define FREETUXTV_IS_MAIN_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_MAIN_WINDOW))
#define FREETUXTV_MAIN_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_MAIN_WINDOW, FreetuxTVMainWindowClass))

typedef struct _FreetuxTVMainWindow FreetuxTVMainWindow;
typedef struct _FreetuxTVMainWindowClass FreetuxTVMainWindowClass;

struct _FreetuxTVMainWindow
{
	GtkWindow parent;

	FreetuxTVPlayer *player;
	FreetuxTVChannelsList *channelslist;
};

struct _FreetuxTVMainWindowClass
{
	GtkWindowClass parent_class;
};

GType
freetuxtv_main_window_get_type (void);

GtkWidget *
freetuxtv_main_window_new ();

FreetuxTVMainWindow *
freetuxtv_channel_get_main_window (GtkWidget *self);

G_END_DECLS

#endif /* FREETUXTV_MAIN_WINDOW_H */
