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

#ifndef _FREETUXTV_CHANNELS_GROUP_INFOS_H_
#define _FREETUXTV_CHANNELS_GROUP_INFOS_H_

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum {
	FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST = 0,
	FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES
} FREETUXTV_CHANNELSGROUP_TYPEGROUP;

#define FREETUXTV_TYPE_CHANNELS_GROUP_INFOS             (freetuxtv_channels_group_infos_get_type ())
#define FREETUXTV_CHANNELS_GROUP_INFOS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelsGroupInfos))
#define FREETUXTV_CHANNELS_GROUP_INFOS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelsGroupInfosClass))
#define FREETUXTV_IS_CHANNELS_GROUP_INFOS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS))
#define FREETUXTV_IS_CHANNELS_GROUP_INFOS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS))
#define FREETUXTV_CHANNELS_GROUP_INFOS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelsGroupInfosClass))

typedef struct _FreetuxTVChannelsGroupInfosClass FreetuxTVChannelsGroupInfosClass;
typedef struct _FreetuxTVChannelsGroupInfos FreetuxTVChannelsGroupInfos;

struct _FreetuxTVChannelsGroupInfosClass
{
	GObjectClass parent_class;
};

struct _FreetuxTVChannelsGroupInfos
{
	GObject parent_instance;

	int id;
	int position;
	gchar *name;
	gchar *uri;

	FREETUXTV_CHANNELSGROUP_TYPEGROUP type;

	gchar *bregex;
	gchar *eregex;

	int nb_channels;
};

GType freetuxtv_channels_group_infos_get_type (void) G_GNUC_CONST;

FreetuxTVChannelsGroupInfos*
freetuxtv_channels_group_infos_new(gchar *name, FREETUXTV_CHANNELSGROUP_TYPEGROUP type);

void
freetuxtv_channels_group_infos_set_id (FreetuxTVChannelsGroupInfos *self, int id);

void
freetuxtv_channels_group_infos_set_position (FreetuxTVChannelsGroupInfos *self, int position);

void
freetuxtv_channels_group_infos_set_name (FreetuxTVChannelsGroupInfos *self, gchar *name);

void
freetuxtv_channels_group_infos_set_uri (FreetuxTVChannelsGroupInfos *self, gchar *uri);

void
freetuxtv_channels_group_infos_set_regex (FreetuxTVChannelsGroupInfos *self, gchar *bregex, gchar *eregex);

G_END_DECLS

#endif /* _FREETUXTV_CHANNELS_GROUP_INFOS_H_ */

