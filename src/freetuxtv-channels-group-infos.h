/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CHANNELS_GROUP_INFOS_H
#define FREETUXTV_CHANNELS_GROUP_INFOS_H

#include <glib-object.h>

#define FREETUXTV_TYPE_CHANNELS_GROUP_INFOS                  (freetuxtv_channels_group_infos_get_type ())
#define FREETUXTV_CHANNELS_GROUP_INFOS(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelsGroupInfos))
#define FREETUXTV_IS_CHANNELS_GROUP_INFOS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS))
#define FREETUXTV_CHANNELS_GROUP_INFOS_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelGroupsInfosClass))
#define FREETUXTV_IS_CHANNELS_GROUP_INFOS_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS))
#define FREETUXTV_CHANNELS_GROUP_INFOS_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelGroupsInfosClass))

typedef struct _FreetuxTVChannelsGroupInfos        FreetuxTVChannelsGroupInfos;
typedef struct _FreetuxTVChannelsGroupInfosClass   FreetuxTVChannelsGroupInfosClass;

struct _FreetuxTVChannelsGroupInfos
{
	GObject parent_instance;

	int id;
	int rank;
	char *name;
	char *uri;
};

struct _FreetuxTVChannelsGroupInfosClass
{
	GObjectClass parent_class;
};

FreetuxTVChannelsGroupInfos*
freetuxtv_channels_group_infos_new(int id, int rank, char *name, char *uri);

#endif /* FREETUXTV_CHANNELS_GROUP_INFOS_H */

