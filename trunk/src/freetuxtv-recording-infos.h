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

#ifndef _FREETUXTV_RECORDING_INFOS_H_
#define _FREETUXTV_RECORDING_INFOS_H_

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum {
	FREETUXTV_RECORDING_STATUS_NOTSET = 0,
	FREETUXTV_RECORDING_STATUS_WAITING,
	FREETUXTV_RECORDING_STATUS_PROCESSING,
	FREETUXTV_RECORDING_STATUS_FINISHED,
	FREETUXTV_RECORDING_STATUS_SKIPPED,
	FREETUXTV_RECORDING_STATUS_ERROR,
} FREETUXTV_RECORDING_STATUS;

#define FREETUXTV_TYPE_RECORDING_INFOS             (freetuxtv_recording_infos_get_type ())
#define FREETUXTV_RECORDING_INFOS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_RECORDING_INFOS, FreetuxTVRecordingInfos))
#define FREETUXTV_RECORDING_INFOS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_RECORDING_INFOS, FreetuxTVRecordingInfosClass))
#define FREETUXTV_IS_RECORDING_INFOS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_RECORDING_INFOS))
#define FREETUXTV_IS_RECORDING_INFOS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_RECORDING_INFOS))
#define FREETUXTV_RECORDING_INFOS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_RECORDING_INFOS, FreetuxTVRecordingInfosClass))

typedef struct _FreetuxTVRecordingInfosClass FreetuxTVRecordingInfosClass;
typedef struct _FreetuxTVRecordingInfos FreetuxTVRecordingInfos;

struct _FreetuxTVRecordingInfosClass
{
	GObjectClass parent_class;
};

struct _FreetuxTVRecordingInfos
{
	GObject parent_instance;

	int id;
	gchar* szTitle;

	FREETUXTV_RECORDING_STATUS status;

	gint64 beginTime;
	gint64 endTime;

	gchar* szFileName;

	int channel_id;
};

GType freetuxtv_recording_infos_get_type (void) G_GNUC_CONST;

FreetuxTVRecordingInfos*
freetuxtv_recording_infos_new(gchar *title, gint64 beginTime, gint64 endTime, int channel_id);

void
freetuxtv_recording_infos_set_id(FreetuxTVRecordingInfos* self, int id);

void
freetuxtv_recording_infos_set_status(FreetuxTVRecordingInfos* self, FREETUXTV_RECORDING_STATUS status);

void
freetuxtv_recording_infos_set_filename(FreetuxTVRecordingInfos* self, const gchar* szFileName);

gboolean
freetuxtv_recording_infos_has_time (FreetuxTVRecordingInfos* self, gint64 refTime);

gboolean
freetuxtv_recording_infos_is_time_greater (FreetuxTVRecordingInfos* self, gint64 refTime);

G_END_DECLS

#endif /* _FREETUXTV_RECORDING_INFOS_H_ */
