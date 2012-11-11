/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2012 <eric.beuque@gmail.com>
 * 
freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_H_
#define _FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_H_

#include <glib-object.h>

#include "gtk-builder-window.h"
#include "freetuxtv-app.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE             (freetuxtv_window_tv_channels_database_get_type ())
#define FREETUXTV_WINDOW_TV_CHANNELS_DATABASE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE, FreetuxTVWindowTVChannelsDatabase))
#define FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE, FreetuxTVWindowTVChannelsDatabaseClass))
#define FREETUXTV_IS_WINDOW_TV_CHANNELS_DATABASE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE))
#define FREETUXTV_IS_WINDOW_TV_CHANNELS_DATABASE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE))
#define FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE, FreetuxTVWindowTVChannelsDatabaseClass))

typedef struct _FreetuxTVWindowTVChannelsDatabaseClass FreetuxTVWindowTVChannelsDatabaseClass;
typedef struct _FreetuxTVWindowTVChannelsDatabase FreetuxTVWindowTVChannelsDatabase;



struct _FreetuxTVWindowTVChannelsDatabaseClass
{
	GtkBuilderWindowClass parent_class;
};

struct _FreetuxTVWindowTVChannelsDatabase
{
	GtkBuilderWindow parent_instance;
};

GType freetuxtv_window_tv_channels_database_get_type (void) G_GNUC_CONST;

FreetuxTVWindowTVChannelsDatabase*
freetuxtv_window_tv_channels_database_new (GtkWindow *parent, FreetuxTVApp* app);

G_END_DECLS

#endif /* _FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_H_ */
