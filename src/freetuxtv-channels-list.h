/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
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
	GtkWidget *filter_widget;
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

FreetuxTVChannelsList *
freetuxtv_channels_list_get_from_widget (GtkWidget *self);

G_END_DECLS

#endif /* FREETUXTV_CHANNELS_LIST_H */
