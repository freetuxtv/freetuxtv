/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * FreetuxTV is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * FreetuxTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glade; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef FREETUXTV_RECORDINGS_LIST_H
#define FREETUXTV_RECORDINGS_LIST_H

#include <gtk/gtk.h>

#include "freetuxtv-app.h"

#include "freetuxtv-db-sync.h"

void
recordings_list_init(FreetuxTVApp *app);

void
recordings_list_load_recordings(FreetuxTVApp *app, DBSync* dbsync, GError** error);

void
recordings_list_refresh(FreetuxTVApp *app);

void
recordings_list_updatestatus(FreetuxTVApp *app, DBSync* dbsync, GError** error);

void
recordings_list_getrecordings_toprocess(FreetuxTVApp *app, GList** ppListRecording, GError** error);

void
recordings_list_add_recording(FreetuxTVApp *app, DBSync* dbsync, FreetuxTVRecordingInfos* pRecordingInfos, GError** error);

#endif /* FREETUXTV_RECORDINGS_LIST_H */
