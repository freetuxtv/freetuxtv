/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
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

typedef enum
{
  FREETUXTV_CHANNELS_GROUP_COLLAPSED,
  FREETUXTV_CHANNELS_GROUP_EXPANDED,
} FreetuxTVChannelsGroupState;



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
	gchar *uri;
	
	gchar *filter;
	FreetuxTVChannelsGroupState state;

	GtkWidget *channels_widget;
	GtkWidget *arrow;
	GtkWidget *popup_menu;
};

struct _FreetuxTVChannelsGroupClass
{
	GtkVBoxClass parent_class;

	void (* menu_delete_group) (FreetuxTVChannelsGroup *channels_group);
	void (* menu_delete_channels) (FreetuxTVChannelsGroup *channels_group);
	void (* menu_refresh_group) (FreetuxTVChannelsGroup *channels_group);	
};

GType
freetuxtv_channels_group_get_type (void);

GtkWidget *
freetuxtv_channels_group_new (gchar *id, gchar *name, gchar *uri);

void
freetuxtv_channels_group_set_collapsed (FreetuxTVChannelsGroup *self,
					FreetuxTVChannelsGroupState state);

void
freetuxtv_channels_group_change_collapsed (FreetuxTVChannelsGroup *self);

int
freetuxtv_channels_group_set_filter (FreetuxTVChannelsGroup *self, 
				     gchar *filter);

gchar *
freetuxtv_channels_group_get_filter (FreetuxTVChannelsGroup *self);

void
freetuxtv_channels_group_add_channel (FreetuxTVChannelsGroup *self,
				      FreetuxTVChannel *channel);

void
freetuxtv_channels_group_delete_channels (FreetuxTVChannelsGroup *self);

FreetuxTVChannelsGroup *
freetuxtv_channels_group_get_from_channel (FreetuxTVChannel *channel);

G_END_DECLS

#endif /* FREETUXTV_CHANNELS_GROUP_H */
