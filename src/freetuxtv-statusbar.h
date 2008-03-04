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

#ifndef FREETUXTV_STATUSBAR_H
#define FREETUXTV_STATUSBAR_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-channel.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_STATUSBAR            (freetuxtv_statusbar_get_type ())
#define FREETUXTV_STATUSBAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_STATUSBAR, FreetuxTVStatusbar))
#define FREETUXTV_STATUSBAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_STATUSBAR, FreetuxTVStatusbarClass))
#define FREETUXTV_IS_STATUSBAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_STATUSBAR))
#define FREETUXTV_IS_STATUSBAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_STATUSBAR))
#define FREETUXTV_STATUSBAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_STATUSBAR, FreetuxTVStatusbarClass))

typedef struct _FreetuxTVStatusbar FreetuxTVStatusbar;
typedef struct _FreetuxTVStatusbarClass FreetuxTVStatusbarClass;

struct _FreetuxTVStatusbar
{
	GtkStatusbar parent;
};

struct _FreetuxTVStatusbarClass
{
	GtkStatusbarClass parent_class;
};

GType
freetuxtv_statusbar_get_type (void);

GtkWidget *
freetuxtv_statusbar_new ();

void freetuxtv_statusbar_update_channel_info (FreetuxTVStatusbar *self, 
					      FreetuxTVChannel *channel);

G_END_DECLS

#endif /* FREETUXTV_STATUSBAR_H */
