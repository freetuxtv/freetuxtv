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

#ifndef FREETUXTV_TV_CHANNELS_LIST_H
#define FREETUXTV_TV_CHANNELS_LIST_H

#include "freetuxtv-app.h"
#include "freetuxtv-db-sync.h"

typedef void (*SynchronizeProgressCB)(gchar* szTVChannelName, void* user_data);

void
tvchannels_list_synchronize (FreetuxTVApp *app, DBSync *dbsync,
    SynchronizeProgressCB funcCB, void* user_data,
    GError** error);

gchar*
tvchannels_list_get_tvchannel_logo_path_for_channel(FreetuxTVApp *app, 
    FreetuxTVChannelInfos* pChannelInfos,
    gboolean bNoneIcon);

gchar*
tvchannels_list_get_tvchannel_logo_path_for_recording(FreetuxTVApp *app, 
    FreetuxTVRecordingInfos* pRecordingInfos,
    gboolean bNoneIcon);

#endif /* FREETUXTV_TV_CHANNELS_LIST_H */
