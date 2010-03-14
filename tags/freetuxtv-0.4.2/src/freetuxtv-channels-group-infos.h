/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
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

#ifndef FREETUXTV_CHANNELS_GROUP_INFOS_H
#define FREETUXTV_CHANNELS_GROUP_INFOS_H

#include <glib-object.h>

#define FREETUXTV_TYPE_CHANNELS_GROUP_INFOS                  (freetuxtv_channels_group_infos_get_type ())
#define FREETUXTV_CHANNELS_GROUP_INFOS(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelsGroupInfos))
#define FREETUXTV_IS_CHANNELS_GROUP_INFOS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS))
#define FREETUXTV_CHANNELS_GROUP_INFOS_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelGroupsInfosClass))
#define FREETUXTV_IS_CHANNELS_GROUP_INFOS_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS))
#define FREETUXTV_CHANNELS_GROUP_INFOS_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelGroupsInfosClass))

typedef struct _FreetuxTVChannelsGroupInfos        FreetuxTVChannelsGroupInfos;
typedef struct _FreetuxTVChannelsGroupInfosClass   FreetuxTVChannelsGroupInfosClass;

struct _FreetuxTVChannelsGroupInfos
{
	GObject parent_instance;

	int id;
	int rank;
	gchar *name;
	gchar *uri;

	gchar *bregex;
	gchar *eregex;

	int nb_channels;
	int nb_channels_visible;
};

struct _FreetuxTVChannelsGroupInfosClass
{
	GObjectClass parent_class;
};

FreetuxTVChannelsGroupInfos*
freetuxtv_channels_group_infos_new(gchar *name, gchar *uri);

void
freetuxtv_channels_group_infos_set_id (FreetuxTVChannelsGroupInfos *self, int id);

void
freetuxtv_channels_group_infos_set_rank (FreetuxTVChannelsGroupInfos *self, int rank);

void
freetuxtv_channels_group_infos_set_name (FreetuxTVChannelsGroupInfos *self, gchar *name);

void
freetuxtv_channels_group_infos_set_uri (FreetuxTVChannelsGroupInfos *self, gchar *uri);

void
freetuxtv_channels_group_infos_set_regex (FreetuxTVChannelsGroupInfos *self, gchar *bregex, gchar *eregex);

#endif /* FREETUXTV_CHANNELS_GROUP_INFOS_H */

