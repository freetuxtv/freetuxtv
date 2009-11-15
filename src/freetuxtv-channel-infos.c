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

#include "freetuxtv-channel-infos.h"

G_DEFINE_TYPE (FreetuxTVChannelInfos, freetuxtv_channel_infos, G_TYPE_OBJECT);

FreetuxTVChannelInfos*
freetuxtv_channel_infos_new(char *name, char *url)
{
	FreetuxTVChannelInfos *channel_infos;
	channel_infos = g_object_new (FREETUXTV_TYPE_CHANNEL_INFOS, NULL);

	channel_infos->name = g_strdup(name);
	channel_infos->url = g_strdup(url);	

	channel_infos->logo_name = NULL;

	return channel_infos;
}

void
freetuxtv_channel_infos_set_id(FreetuxTVChannelInfos* self, int id)
{
	self->id = id;
}

void
freetuxtv_channel_infos_set_order(FreetuxTVChannelInfos* self, int order)
{
	self->order = order;
}

void
freetuxtv_channel_infos_set_logo(FreetuxTVChannelInfos* self,
				 char *logo_name)
{
	if(self->logo_name != NULL){
		g_free(self->logo_name);
	}
	self->logo_name = g_strdup(logo_name);
}


void
freetuxtv_channel_infos_set_channels_group(FreetuxTVChannelInfos* self,
					   FreetuxTVChannelsGroupInfos *channels_group)
{
	if(self->channels_group != channels_group){
		if(self->channels_group != NULL){
			g_object_unref(self->channels_group);
		}
		g_object_ref(channels_group);
		self->channels_group = channels_group;
	}
}

static GObject *
freetuxtv_channel_infos_constructor (GType                  gtype,
				     guint                  n_properties,
				     GObjectConstructParam *properties)
{
	GObject *obj;
	FreetuxTVChannelInfosClass *klass;
	GObjectClass *parent_class;  
	parent_class = G_OBJECT_CLASS (freetuxtv_channel_infos_parent_class);
	obj = parent_class->constructor (gtype, n_properties, properties);
	return obj;
}

static void
freetuxtv_channel_infos_dispose (GObject *object)
{
	FreetuxTVChannelInfos *self;
	
	g_return_if_fail(object != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(object));
	 
	self = FREETUXTV_CHANNEL_INFOS(object);

	if(self->name != NULL){
		g_free(self->name);
		self->name = NULL;
	}

	if(self->url != NULL){
		g_free(self->url);
		self->url = NULL;
	}

	if(self->channels_group != NULL){
		g_object_unref(self->channels_group);
		self->channels_group = NULL;
	}

	G_OBJECT_CLASS (freetuxtv_channel_infos_parent_class)->dispose (object);
	
}

static void
freetuxtv_channel_infos_class_init (FreetuxTVChannelInfosClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->constructor = freetuxtv_channel_infos_constructor;
	gobject_class->dispose = freetuxtv_channel_infos_dispose;
}

static void
freetuxtv_channel_infos_init (FreetuxTVChannelInfos *self)
{
	self->id = -1;
	self->name=NULL;
	self->url=NULL;

	self->logo_name = NULL;
	self->channels_group = NULL;
}
