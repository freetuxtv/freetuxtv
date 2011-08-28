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

#ifndef FREETUXTV_CHANNELS_LIST_H
#define FREETUXTV_CHANNELS_LIST_H

#include <gtk/gtk.h>

#include "freetuxtv-app.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-db-sync.h"

#define FREETUXTV_LIBM3UPARSE_ERROR freetuxtv_libm3uparse_error_quark ()
typedef enum
{
	FREETUXTV_LIBM3UPARSE_ERROR_PARSE,
} FreetuxTVLibM3UParseError;

GQuark
freetuxtv_libm3uparse_error_quark ();

// Callback function
typedef void (*DBOnChannelsGroupAddedFunc) (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error);

typedef void (*DBOnChannelsAddedFunc) (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error);


void
channels_list_init (FreetuxTVApp *app);

void
channels_list_load_channels (FreetuxTVApp *app, DBSync* dbsync, GError** error);

void
channels_list_db_add_channels_group (
    FreetuxTVApp *app, DBSync* dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    DBOnChannelsGroupAddedFunc funcGroupAdded,
    DBOnChannelsAddedFunc funcChannelsAdded,
    gpointer user_data,
    GError** error
    );

void
channels_list_update_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
                                     FreetuxTVChannelsGroupInfos* channels_group_infos);

void
channels_list_db_refresh_channels_group (
    FreetuxTVApp *app, DBSync* dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error
    );

void
channels_list_reload_channels_of_channels_group (
    FreetuxTVApp *app, DBSync* dbsync,
    GtkTreePath *path_group,
    GError** error);

void
channels_list_refresh_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
                                      DBSync* dbsync, GError** error);

void
channels_list_delete_channels_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
                                              DBSync* dbsync, GError** error);

void
channels_list_delete_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
                                     DBSync* dbsync, GError** error);

void
channels_list_switch_channels_group (FreetuxTVApp *app,
                                     GtkTreePath *pPathGroupSrc, GtkTreePath *pPathGroupDest,
                                     DBSync* dbsync, GError** error);

void
channels_list_switch_channel (FreetuxTVApp *app,
                              GtkTreePath *pPathChannelSrc, GtkTreePath *pPathChannelDest,
                              DBSync* dbsync, GError** error);

void
channels_list_ui_add_channels_group (
    FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GtkTreePath** ppChannelsGroupTreePath
    );

gboolean
channels_list_get_prev_channel (FreetuxTVApp *app,
                                GtkTreePath **path_prev_channel);

gboolean
channels_list_get_next_channel (FreetuxTVApp *app,
                                GtkTreePath ** path_next_channel);

gint
channels_list_get_channels_count (FreetuxTVApp *app);

void
channels_list_set_playing(FreetuxTVApp *app, GtkTreePath *path_channel);

FreetuxTVChannelInfos*
channels_list_get_channel(FreetuxTVApp *app, GtkTreePath* path_channel);

FreetuxTVChannelInfos*
channels_list_get_channel_by_id(FreetuxTVApp *app, int channel_id);

FreetuxTVChannelsGroupInfos*
channels_list_get_group(FreetuxTVApp *app, GtkTreePath* path_group);

#endif /* FREETUXTV_CHANNELS_LIST_H */
