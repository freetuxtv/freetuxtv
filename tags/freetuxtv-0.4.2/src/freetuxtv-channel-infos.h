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

#ifndef FREETUXTV_CHANNEL_INFOS_H
#define FREETUXTV_CHANNEL_INFOS_H

#include <glib-object.h>

#define FREETUXTV_TYPE_CHANNEL_INFOS                  (freetuxtv_channel_infos_get_type ())
#define FREETUXTV_CHANNEL_INFOS(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfos))
#define FREETUXTV_IS_CHANNEL_INFOS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNEL_INFOS))
#define FREETUXTV_CHANNEL_INFOS_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfosClass))
#define FREETUXTV_IS_CHANNEL_INFOS_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNEL_INFOS))
#define FREETUXTV_CHANNEL_INFOS_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfosClass))

#include "freetuxtv-channels-group-infos.h"

typedef struct _FreetuxTVChannelInfos        FreetuxTVChannelInfos;
typedef struct _FreetuxTVChannelInfosClass   FreetuxTVChannelInfosClass;

struct _FreetuxTVChannelInfos
{
	GObject parent_instance;

	int id;
	int order;
	char *name;
	char *url;

	char *logo_name;

	FreetuxTVChannelsGroupInfos *channels_group;
};

struct _FreetuxTVChannelInfosClass
{
	GObjectClass parent_class;
	
};

FreetuxTVChannelInfos*
freetuxtv_channel_infos_new(char *name, char *url);

void
freetuxtv_channel_infos_set_id(FreetuxTVChannelInfos* self, int id);

void
freetuxtv_channel_infos_set_order(FreetuxTVChannelInfos* self, int order);

void
freetuxtv_channel_infos_set_logo(FreetuxTVChannelInfos* self,
				 char *logo_name);

void
freetuxtv_channel_infos_set_channels_group(FreetuxTVChannelInfos* self,
					   FreetuxTVChannelsGroupInfos *group);

#endif /* FREETUXTV_CHANNEL_INFOS_H */

