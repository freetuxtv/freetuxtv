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

#ifndef _FREETUXTV_CELLRENDERER_RECORDINGSLIST_H_
#define _FREETUXTV_CELLRENDERER_RECORDINGSLIST_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define FREETUXTV_TYPE_CELLRENDERER_RECORDINGSLIST             (freetuxtv_cellrenderer_recordingslist_get_type ())
#define FREETUXTV_CELLRENDERER_RECORDINGSLIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CELLRENDERER_RECORDINGSLIST, FreetuxTVCellRendererRecordingsList))
#define FREETUXTV_CELLRENDERER_RECORDINGSLIST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CELLRENDERER_RECORDINGSLIST, FreetuxTVCellRendererRecordingsListClass))
#define FREETUXTV_IS_CELLRENDERER_RECORDINGSLIST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CELLRENDERER_RECORDINGSLIST))
#define FREETUXTV_IS_CELLRENDERER_RECORDINGSLIST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CELLRENDERER_RECORDINGSLIST))
#define FREETUXTV_CELLRENDERER_RECORDINGSLIST_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CELLRENDERER_RECORDINGSLIST, FreetuxTVCellRendererRecordingsListClass))

typedef struct _FreetuxTVCellRendererRecordingsListClass FreetuxTVCellRendererRecordingsListClass;
typedef struct _FreetuxTVCellRendererRecordingsList FreetuxTVCellRendererRecordingsList;

struct _FreetuxTVCellRendererRecordingsListClass
{
	GtkCellRendererClass parent_class;
};

struct _FreetuxTVCellRendererRecordingsList
{
	GtkCellRenderer parent_instance;
	
	gchar *szTitle;
	gchar *szLogoPath;
	gint64 beginTime;
	gint64 endTime;
	gint status;
};

GType freetuxtv_cellrenderer_recordingslist_get_type (void) G_GNUC_CONST;

GtkCellRenderer *
freetuxtv_cellrenderer_recordingslist_new (void);

G_END_DECLS

#endif /* _FREETUXTV_CELLRENDERER_RECORDINGSLIST_H_ */
