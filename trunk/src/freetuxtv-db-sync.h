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

#ifndef FREETUXTV_DB_SYNC_H
#define FREETUXTV_DB_SYNC_H

#include <glib.h>
#include <sqlite3.h>

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-channel-infos.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-tv-channel-infos.h"
#include "freetuxtv-recording-infos.h"

typedef struct _DBSync DBSync;
struct _DBSync
{
	struct sqlite3 *db_link;
};

#define FREETUXTV_DBSYNC_ERROR freetuxtv_dbsync_error_quark ()
typedef enum
{
	FREETUXTV_DBSYNC_ERROR_OPEN,
	FREETUXTV_DBSYNC_ERROR_EXEC_QUERY
} FreetuxTVDBSyncError;

GQuark
freetuxtv_dbsync_error_quark ();

// Database manipulation function

void
dbsync_open_db (DBSync *dbsync, GError** error);

void
dbsync_close_db (DBSync *dbsync);

gboolean
dbsync_db_exists (DBSync *dbsync);

void
dbsync_create_db (DBSync *dbsync, gchar* szScriptFilename, GError** error);

void
dbsync_update_db (DBSync *dbsync, gchar* szScriptFilename, GError** error);

// Channels groups manipulation function

void
dbsync_select_channels_groups (DBSync *dbsync,
    FreetuxTVApp *app,
    int (*callback)(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error);

void
dbsync_select_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    FreetuxTVApp *app,
    int (*callback)(FreetuxTVApp *app, 
	    FreetuxTVChannelInfos* pChannelInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error);

void
dbsync_update_channels_group_last_update (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_delete_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_delete_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_start_update_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_end_update_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

// Channels manipulation function

void
dbsync_add_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    gboolean update,
    GError** error);

void
dbsync_delete_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    GError** error);

int
dbsync_get_channel_id_by_name (DBSync *dbsync,
    const gchar* szChannelName,
    GError** error);

void
dbsync_update_channel_deinterlace_mode (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    gchar *mode,
    GError** error);

void
dbsync_add_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_update_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_switch_position_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfosA,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfosB,
    GError** error);

void
dbsync_switch_position_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfosA,
    FreetuxTVChannelInfos* pChannelInfosB,
    GError** error);

// TVChannels manipulation function

void
dbsync_delete_tvchannels (DBSync *dbsync, GError** error);

void
dbsync_add_tvchannel (DBSync *dbsync, FreetuxTVTvChannelInfos* pTvChannelInfos,
    GError** error);

// Recordings manipulation function

void
dbsync_add_recording (DBSync *dbsync, FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error);

void
dbsync_select_recordings (DBSync *dbsync,
    FreetuxTVApp *app,
    int (*callback)(FreetuxTVApp *app, 
	    FreetuxTVRecordingInfos* pRecordingInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error);

void
dbsync_update_recording (DBSync *dbsync,
    FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error);

void
dbsync_delete_recording (DBSync *dbsync,
    FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error);

#endif /* FREETUXTV_DB_SYNC_H */
