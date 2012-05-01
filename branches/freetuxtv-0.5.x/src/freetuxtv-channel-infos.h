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

#ifndef _FREETUXTV_CHANNEL_INFOS_H_
#define _FREETUXTV_CHANNEL_INFOS_H_

#include <glib-object.h>

#include "freetuxtv-channels-group-infos.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_CHANNEL_INFOS             (freetuxtv_channel_infos_get_type ())
#define FREETUXTV_CHANNEL_INFOS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfos))
#define FREETUXTV_CHANNEL_INFOS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfosClass))
#define FREETUXTV_IS_CHANNEL_INFOS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNEL_INFOS))
#define FREETUXTV_IS_CHANNEL_INFOS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNEL_INFOS))
#define FREETUXTV_CHANNEL_INFOS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfosClass))

typedef struct _FreetuxTVChannelInfosClass FreetuxTVChannelInfosClass;
typedef struct _FreetuxTVChannelInfos FreetuxTVChannelInfos;

struct _FreetuxTVChannelInfosClass
{
	GObjectClass parent_class;
};

struct _FreetuxTVChannelInfos
{
	GObject parent_instance;

	int id;
	int position;
	gchar *name;
	gchar *url;

	gchar *logo_name;

	gchar **vlc_options;

	gchar *deinterlace_mode;

	FreetuxTVChannelsGroupInfos *channels_group;
};

GType freetuxtv_channel_infos_get_type (void) G_GNUC_CONST;


FreetuxTVChannelInfos*
freetuxtv_channel_infos_new(gchar *name, gchar *url);

void
freetuxtv_channel_infos_set_id(FreetuxTVChannelInfos* self, int id);

void
freetuxtv_channel_infos_set_position(FreetuxTVChannelInfos* self, int position);

void
freetuxtv_channel_infos_set_logo(FreetuxTVChannelInfos* self,
                                 gchar *logo_name);

void
freetuxtv_channel_infos_set_vlcoptions(FreetuxTVChannelInfos* self,
                                       gchar **options);

void
freetuxtv_channel_infos_set_deinterlace_mode(FreetuxTVChannelInfos* self,
                                             gchar *mode);

void
freetuxtv_channel_infos_set_channels_group(FreetuxTVChannelInfos* self,
                                           FreetuxTVChannelsGroupInfos *group);

G_END_DECLS

#endif /* _FREETUXTV_CHANNEL_INFOS_H_ */

