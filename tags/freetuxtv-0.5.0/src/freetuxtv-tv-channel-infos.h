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

#ifndef _FREETUXTV_TV_CHANNEL_INFOS_H_
#define _FREETUXTV_TV_CHANNEL_INFOS_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define FREETUXTV_TYPE_TV_CHANNEL_INFOS             (freetuxtv_tv_channel_infos_get_type ())
#define FREETUXTV_TV_CHANNEL_INFOS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_TV_CHANNEL_INFOS, FreetuxtvTvChannelInfos))
#define FREETUXTV_TV_CHANNEL_INFOS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_TV_CHANNEL_INFOS, FreetuxtvTvChannelInfosClass))
#define FREETUXTV_IS_TV_CHANNEL_INFOS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_TV_CHANNEL_INFOS))
#define FREETUXTV_IS_TV_CHANNEL_INFOS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_TV_CHANNEL_INFOS))
#define FREETUXTV_TV_CHANNEL_INFOS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_TV_CHANNEL_INFOS, FreetuxtvTvChannelInfosClass))

typedef struct _FreetuxTVTvChannelInfosClass FreetuxTVTvChannelInfosClass;
typedef struct _FreetuxTVTvChannelInfos FreetuxTVTvChannelInfos;

struct _FreetuxTVTvChannelInfosClass
{
	GObjectClass parent_class;
};

struct _FreetuxTVTvChannelInfos
{
	GObject parent_instance;
};

GType freetuxtv_tv_channel_infos_get_type (void) G_GNUC_CONST;

FreetuxTVTvChannelInfos*
freetuxtv_tv_channel_infos_new(gchar* name);

void
freetuxtv_tv_channel_infos_set_id(FreetuxTVTvChannelInfos* tv_channel_infos, gint id);

gint
freetuxtv_tv_channel_infos_get_id(FreetuxTVTvChannelInfos* tv_channel_infos);

void
freetuxtv_tv_channel_infos_set_name(FreetuxTVTvChannelInfos* tv_channel_infos, gchar* name);

const gchar*
freetuxtv_tv_channel_infos_get_name(FreetuxTVTvChannelInfos* tv_channel_infos);

void
freetuxtv_tv_channel_infos_set_logo_filename(FreetuxTVTvChannelInfos* tv_channel_infos, gchar* logo_filename);

const gchar*
freetuxtv_tv_channel_infos_get_logo_filename(FreetuxTVTvChannelInfos* tv_channel_infos);

void
freetuxtv_tv_channel_infos_add_label(FreetuxTVTvChannelInfos* tv_channel_infos, gchar *label);

GList*
freetuxtv_tv_channel_infos_get_labels(FreetuxTVTvChannelInfos* tv_channel_infos);

G_END_DECLS

#endif /* _FREETUXTV_TV_CHANNEL_INFOS_H_ */
