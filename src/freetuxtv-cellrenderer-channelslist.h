/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * FreetuxTV is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * FreetuxTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glade; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef FREETUXTV_CELLRENDERER_CHANNELSLIST_H
#define FREETUXTV_CELLRENDERER_CHANNELSLIST_H

#include <gtk/gtk.h>

#define FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST             (freetuxtv_cellrenderer_channelslist_get_type())
#define FREETUXTV_CELLRENDERER_CHANNELSLIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST, FreetuxTVCellRendererChannelsList))
#define FREETUXTV_IS_CELLRENDERER_CHANNELSLISTS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST))
#define FREETUXTV_CELLRENDERER_CHANNELSLIST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST, FreetuxTVCellRendererChannelsListClass))

#define FREETUXTV_IS_CELLRENDERER_CHANNELSLIST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST))
#define CUSTOM_CELL_RENDERER_PROGRESS_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj),  FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST, FreetuxTVCellRendererChannelsListClass))

typedef struct _FreetuxTVCellRendererChannelsList FreetuxTVCellRendererChannelsList;
typedef struct _FreetuxTVCellRendererChannelsListClass FreetuxTVCellRendererChannelsListClass;

typedef enum {
	CELLRENDERER_TYPE_CHANNELS_GROUP,
	CELLRENDERER_TYPE_CHANNEL
} FreetuxTVCellRendererChannelsListType;

struct _FreetuxTVCellRendererChannelsList
{
	GtkCellRenderer parent;

	FreetuxTVCellRendererChannelsListType type;
	gchar *name;
	gchar *logo;
	gboolean is_playing;
};

struct _FreetuxTVCellRendererChannelsListClass
{
	GtkCellRendererClass  parent_class;
};

GtkCellRenderer *
freetuxtv_cellrenderer_channelslist_new (void);


#define FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST_TYPE (freetuxtv_cellrenderer_channelslist_type_get_type())

#endif /* FREETUXTV_CELLRENDERER_CHANNELSLIST_H */
