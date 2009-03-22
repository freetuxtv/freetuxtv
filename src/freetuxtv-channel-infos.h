/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CHANNEL_INFOS_H
#define FREETUXTV_CHANNEL_INFOS_H

#include <glib-object.h>

#define FREETUXTV_TYPE_CHANNEL_INFOS                  (freetuxtv_channel_infos_get_type ())
#define FREETUXTV_CHANNEL_INFOS(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfos))
#define FREETUXTV_IS_CHANNEL_INFOS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNEL_INFOS))
#define FREETUXTV_CHANNEL_INFOS_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfosClass))
#define FREETUXTV_IS_CHANNEL_INFOS_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNEL_INFOS))
#define FREETUXTV_CHANNEL_INFOS_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfosClass))

#include "freetuxtv-channels-group-infos.h"

typedef struct _FreetuxTVChannelInfos        FreetuxTVChannelInfos;
typedef struct _FreetuxTVChannelInfosClass   FreetuxTVChannelInfosClass;

struct _FreetuxTVChannelInfos
{
	GObject parent_instance;

	int id;
	int rank;
	char *name;
	char *url;
	char *redirect_url;

	char *logo_name;

	FreetuxTVChannelsGroupInfos *group;
};

struct _FreetuxTVChannelInfosClass
{
	GObjectClass parent_class;
	
};

FreetuxTVChannelInfos*
freetuxtv_channel_infos_new(int id, int rank, char *name, char *url);

void
freetuxtv_channel_infos_set_logo(FreetuxTVChannelInfos* self,
				 char *logo_name);

void
freetuxtv_channel_infos_set_group(FreetuxTVChannelInfos* self,
				  FreetuxTVChannelsGroupInfos *group);

#endif /* FREETUXTV_CHANNEL_INFOS_H */

