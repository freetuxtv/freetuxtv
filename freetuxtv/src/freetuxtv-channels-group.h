/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CHANNELS_GROUP_H
#define FREETUXTV_CHANNELS_GROUP_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-channel.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_CHANNELS_GROUP            (freetuxtv_channels_group_get_type ())
#define FREETUXTV_CHANNELS_GROUP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNELS_GROUP, FreetuxTVChannelsGroup))
#define FREETUXTV_CHANNELS_GROUP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNELS_GROUP, FreetuxTVChannelsGroupClass))
#define FREETUXTV_IS_CHANNELS_GROUP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNELS_GROUP))
#define FREETUXTV_IS_CHANNELS_GROUP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNELS_GROUP))
#define FREETUXTV_CHANNELS_GROUP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNELS_GROUP, FreetuxTVChannelsGroupClass))

typedef struct _FreetuxTVChannelsGroup FreetuxTVChannelsGroup;
typedef struct _FreetuxTVChannelsGroupClass FreetuxTVChannelsGroupClass;

struct _FreetuxTVChannelsGroup
{
	GtkVBox parent;

	gchar *id;
	gchar *name;
	GtkWidget *channels_widget;

	gchar collapsed;
	GtkWidget *arrow;
	
};

struct _FreetuxTVChannelsGroupClass
{
	GtkVBoxClass parent_class;
};

GType
freetuxtv_channels_group_get_type (void);

GtkWidget *
freetuxtv_channels_group_new (gchar *id, gchar *name);

void
freetuxtv_channels_group_set_collasped (FreetuxTVChannelsGroup *self,
										gchar mode);

void
freetuxtv_channels_group_change_collasped (FreetuxTVChannelsGroup *self);

int
freetuxtv_channels_group_apply_filter (FreetuxTVChannelsGroup *self, 
									   gchar *filter);

int
freetuxtv_channels_group_update_from_db (FreetuxTVChannelsGroup *self);

void
freetuxtv_channels_group_add_channel (FreetuxTVChannelsGroup *self,
									  FreetuxTVChannel *channel);

FreetuxTVChannelsGroup *
freetuxtv_channels_group_get_from_widget (GtkWidget *self);

G_END_DECLS

#endif /* FREETUXTV_CHANNELS_GROUP_H */
