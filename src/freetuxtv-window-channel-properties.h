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

#ifndef _FREETUXTV_WINDOW_CHANNEL_PROPERTIES_H_
#define _FREETUXTV_WINDOW_CHANNEL_PROPERTIES_H_

#include <glib-object.h>

#include "gtk-builder-dialog.h"
#include "freetuxtv-app.h"
#include "freetuxtv-channel-infos.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES             (freetuxtv_window_channel_properties_get_type ())
#define FREETUXTV_WINDOW_CHANNEL_PROPERTIES(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES, FreetuxTVWindowChannelProperties))
#define FREETUXTV_WINDOW_CHANNEL_PROPERTIES_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES, FreetuxTVWindowChannelPropertiesClass))
#define FREETUXTV_IS_WINDOW_CHANNEL_PROPERTIES(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES))
#define FREETUXTV_IS_WINDOW_CHANNEL_PROPERTIES_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES))
#define FREETUXTV_WINDOW_CHANNEL_PROPERTIES_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES, FreetuxTVWindowChannelPropertiesClass))

typedef struct _FreetuxTVWindowChannelPropertiesClass FreetuxTVWindowChannelPropertiesClass;
typedef struct _FreetuxTVWindowChannelProperties FreetuxTVWindowChannelProperties;

struct _FreetuxTVWindowChannelPropertiesClass
{
	GtkBuilderDialogClass parent_class;
};

struct _FreetuxTVWindowChannelProperties
{
	GtkBuilderDialog parent_instance;
};

GType freetuxtv_window_channel_properties_get_type (void) G_GNUC_CONST;

FreetuxTVWindowChannelProperties*
freetuxtv_window_channel_properties_new (GtkWindow *parent, FreetuxTVApp* app, FreetuxTVChannelInfos* pChannelInfos);

G_END_DECLS

#endif /* _FREETUXTV_WINDOW_CHANNEL_PROPERTIES_H_ */
