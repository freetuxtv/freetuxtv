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

#ifndef FREETUXTV_WINDOW_RECORDING_H
#define FREETUXTV_WINDOW_RECORDING_H

#include <gtk/gtk.h>

#include "gtk-builder-dialog.h"
#include "freetuxtv-app.h"
#include "freetuxtv-recording-infos.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_WINDOW_RECORDING             (freetuxtv_window_recording_get_type ())
#define FREETUXTV_WINDOW_RECORDING(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_WINDOW_RECORDING, FreetuxTVWindowRecording))
#define FREETUXTV_WINDOW_RECORDING_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_WINDOW_RECORDING, FreetuxTVWindowRecordingClass))
#define FREETUXTV_IS_WINDOW_RECORDING(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_WINDOW_RECORDING))
#define FREETUXTV_IS_WINDOW_RECORDING_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_WINDOW_RECORDING))
#define FREETUXTV_WINDOW_RECORDING_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_WINDOW_RECORDING, FreetuxTVWindowRecordingClass))

typedef struct _FreetuxTVWindowRecordingClass FreetuxTVWindowRecordingClass;
typedef struct _FreetuxTVWindowRecording FreetuxTVWindowRecording;

struct _FreetuxTVWindowRecordingClass
{
	GtkBuilderDialogClass parent_class;
};

struct _FreetuxTVWindowRecording
{
	GtkBuilderDialog parent_instance;
};

GType freetuxtv_window_channel_properties_get_type (void) G_GNUC_CONST;

FreetuxTVWindowRecording*
freetuxtv_window_recording_new (GtkWindow *parent, FreetuxTVApp* app, FreetuxTVChannelInfos* pChannelInfos);

FreetuxTVRecordingInfos*
freetuxtv_window_recording_get_recording_infos(FreetuxTVWindowRecording* pWindowRecording);

G_END_DECLS

#endif /* FREETUXTV_WINDOW_RECORDING_H */
