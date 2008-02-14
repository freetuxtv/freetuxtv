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

#ifndef FREETUXTV_CHANNELS_LIST_H
#define FREETUXTV_CHANNELS_LIST_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-channels-group.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_CHANNELS_LIST            (freetuxtv_channels_list_get_type ())
#define FREETUXTV_CHANNELS_LIST(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNELS_LIST, FreetuxTVChannelsList))
#define FREETUXTV_CHANNELS_LIST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNELS_LIST, FreetuxTVChannelsListClass))
#define FREETUXTV_IS_CHANNELS_LIST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNELS_LIST))
#define FREETUXTV_IS_CHANNELS_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNELS_LIST))
#define FREETUXTV_CHANNELS_LIST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNELS_LIST, FreetuxTVChannelsListClass))

typedef struct _FreetuxTVChannelsList FreetuxTVChannelsList;
typedef struct _FreetuxTVChannelsListClass FreetuxTVChannelsListClass;

struct _FreetuxTVChannelsList
{
	GtkVBox parent;

	GtkWidget *channelsgroups_widget;
};

struct _FreetuxTVChannelsListClass
{
	GtkVBoxClass parent_class;
};

GType
freetuxtv_channels_list_get_type (void);

GtkWidget *
freetuxtv_channels_list_new ();

void
freetuxtv_channels_list_add_channels_group (FreetuxTVChannelsList *self,
				     FreetuxTVChannelsGroup *channels_group);

int
freetuxtv_channels_list_update_from_db (FreetuxTVChannelsList *self);

G_END_DECLS

#endif /* FREETUXTV_CHANNELS_LIST_H */
