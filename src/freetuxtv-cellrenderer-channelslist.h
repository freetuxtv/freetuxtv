/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
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
