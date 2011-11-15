/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2011 <eric.beuque@gmail.com>
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

#include "freetuxtv-recording-infos.h"



G_DEFINE_TYPE (FreetuxTVRecordingInfos, freetuxtv_recording_infos, G_TYPE_OBJECT);

static void
freetuxtv_recording_infos_init (FreetuxTVRecordingInfos *object)
{

	object->id = -1;
	object->szTitle = NULL;
	object->status = FREETUXTV_RECORDING_STATUS_NOTSET;
	object->szFileName = NULL;
	object->channel_id = -1;
	object->beginTime = 0;
	object->endTime = 0;
}

static void
freetuxtv_recording_infos_finalize (GObject *object)
{
	FreetuxTVRecordingInfos *self;
	self = FREETUXTV_RECORDING_INFOS(object);

	G_OBJECT_CLASS (freetuxtv_recording_infos_parent_class)->finalize (object);

	if(self->szTitle){
		g_free(self->szTitle);
		self->szTitle = NULL;
	}
	if(self->szFileName){
		g_free(self->szFileName);
		self->szFileName = NULL;
	}
}

static void
freetuxtv_recording_infos_class_init (FreetuxTVRecordingInfosClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	object_class->finalize = freetuxtv_recording_infos_finalize;
}

FreetuxTVRecordingInfos*
freetuxtv_recording_infos_new(gchar *title, gint64 beginTime, gint64 endTime, int channel_id)
{
	FreetuxTVRecordingInfos *pRecordingInfos;
	pRecordingInfos = g_object_new (FREETUXTV_TYPE_RECORDING_INFOS, NULL);

	pRecordingInfos->szTitle = g_strdup(title);
	pRecordingInfos->channel_id = channel_id;
	pRecordingInfos->beginTime = beginTime;
	pRecordingInfos->endTime = endTime;

	return pRecordingInfos;
}

void
freetuxtv_recording_infos_set_id(FreetuxTVRecordingInfos* self, int id)
{
	self->id = id;
}

void
freetuxtv_recording_infos_set_status(FreetuxTVRecordingInfos* self, FREETUXTV_RECORDING_STATUS status)
{
	self->status = status;
}

void
freetuxtv_recording_infos_set_filename(FreetuxTVRecordingInfos* self, const gchar* szFileName)
{
	if(self->szFileName){
		g_free(self->szFileName);
		self->szFileName = NULL;
	}
	self->szFileName = g_strdup(szFileName);
}

gboolean
freetuxtv_recording_infos_has_time (FreetuxTVRecordingInfos* self, gint64 refTime)
{
	if(self->beginTime <= refTime && refTime <= self->endTime){
		return TRUE;
	}
	return FALSE;
}

gboolean
freetuxtv_recording_infos_is_time_greater (FreetuxTVRecordingInfos* self, gint64 refTime)
{
	if(refTime > self->endTime){
		return TRUE;
	}
	return FALSE;
}