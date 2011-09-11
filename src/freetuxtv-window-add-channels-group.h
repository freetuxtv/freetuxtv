/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
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

#ifndef _FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_H_
#define _FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_H_

#include <glib-object.h>

#include <gtk/gtk.h>

#include "gtk-builder-window.h"
#include "freetuxtv-app.h"
#include "gtk-progress-dialog.h"

#include "freetuxtv-db-sync.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP             (freetuxtv_window_add_channels_group_get_type ())
#define FREETUXTV_WINDOW_ADD_CHANNELS_GROUP(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP, FreetuxTVWindowAddChannelsGroup))
#define FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP, FreetuxTVWindowAddChannelsGroupClass))
#define FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP))
#define FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP))
#define FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP, FreetuxTVWindowAddChannelsGroupClass))

typedef struct _FreetuxTVWindowAddChannelsGroupClass FreetuxTVWindowAddChannelsGroupClass;
typedef struct _FreetuxTVWindowAddChannelsGroup FreetuxTVWindowAddChannelsGroup;

struct _FreetuxTVWindowAddChannelsGroupClass
{
	GtkBuilderWindowClass parent_class;

	void (*channels_group_added) (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data);

	void (*channels_added) (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data);
};

struct _FreetuxTVWindowAddChannelsGroup
{
	GtkBuilderWindow parent_instance;
};

enum FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW {
	FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_EXISTING		= 1 << 0,
	FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_CUSTOM		= 1 << 1,
	FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_FAVOURITES	= 1 << 2,
	
	FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_ALL			=
		FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_EXISTING |
		FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_CUSTOM |
		FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_FAVOURITES
};

GType freetuxtv_window_add_channels_group_get_type (void) G_GNUC_CONST;

FreetuxTVWindowAddChannelsGroup*
freetuxtv_window_add_channels_group_new (GtkWindow *parent, FreetuxTVApp* app, GError** error);

void
freetuxtv_window_add_channels_group_set_allowed_type (
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup,
    int allowedType);
 
FreetuxTVApp*
freetuxtv_window_add_channels_group_get_app(FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup);

G_END_DECLS

#endif /* _FREEUXTV_WINDOW_ADD_CHANNELS_GROUP_H_ */
