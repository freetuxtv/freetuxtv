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

#include "freetuxtv-app.h"
#include "freetuxtv-channels-group-infos.h"

void
channels_list_init (FreetuxTVApp *app);

void
channels_list_apply_filter (FreetuxTVApp *app);

void
channels_list_load_channels (FreetuxTVApp *app);

void
channels_list_load_channels_group (FreetuxTVApp *app, 
				   FreetuxTVChannelsGroupInfos* channels_group_infos);

void
channels_list_add_channels_group (FreetuxTVApp *app, 
				  FreetuxTVChannelsGroupInfos* channels_group_infos);

void
channels_list_update_channels_group (FreetuxTVApp *app, 
				     FreetuxTVChannelsGroupInfos* channels_group_infos);

void
channels_list_delete_channels_channels_group (FreetuxTVApp *app, 
					      FreetuxTVChannelsGroupInfos* channels_group_infos);

void
channels_list_delete_channels_group (FreetuxTVApp *app, 
				     FreetuxTVChannelsGroupInfos* channels_group_infos);

gboolean
channels_list_get_prev_channel (FreetuxTVApp *app, 
				FreetuxTVChannelInfos* channel_infos,
				FreetuxTVChannelInfos** prev_channel_infos);

gboolean
channels_list_get_next_channel (FreetuxTVApp *app, 
				FreetuxTVChannelInfos* channel_infos,
				FreetuxTVChannelInfos** next_channel_infos);

#endif /* FREETUXTV_CHANNELS_LIST_H */
