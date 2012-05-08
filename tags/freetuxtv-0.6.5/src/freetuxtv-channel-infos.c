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

#include "freetuxtv-channel-infos.h"

typedef struct _FreetuxTVChannelInfosPrivate FreetuxTVChannelInfosPrivate;
struct _FreetuxTVChannelInfosPrivate
{
	gchar* nil;
};

#define FREETUXTV_CHANNEL_INFOS_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_CHANNEL_INFOS, FreetuxTVChannelInfosPrivate))



G_DEFINE_TYPE (FreetuxTVChannelInfos, freetuxtv_channel_infos, G_TYPE_OBJECT);

static void
freetuxtv_channel_infos_init (FreetuxTVChannelInfos *object)
{
	object->id = -1;
	object->name=NULL;
	object->url=NULL;

	object->logo_name = NULL;
	object->vlc_options = NULL;
	object->deinterlace_mode = NULL;

	object->channels_group = NULL;
}

static void
freetuxtv_channel_infos_finalize (GObject *object)
{
	FreetuxTVChannelInfos *self;
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

	if(self->vlc_options != NULL){
		g_strfreev(self->vlc_options);
		self->vlc_options = NULL;
	}

	G_OBJECT_CLASS (freetuxtv_channel_infos_parent_class)->finalize (object);
}

static void
freetuxtv_channel_infos_class_init (FreetuxTVChannelInfosClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVChannelInfosPrivate));

	object_class->finalize = freetuxtv_channel_infos_finalize;
}

FreetuxTVChannelInfos*
freetuxtv_channel_infos_new(gchar *name, gchar *url)
{
	FreetuxTVChannelInfos *pChannelInfos;
	pChannelInfos = g_object_new (FREETUXTV_TYPE_CHANNEL_INFOS, NULL);

	pChannelInfos->name = g_strdup(name);
	pChannelInfos->url = g_strdup(url);	

	pChannelInfos->logo_name = NULL;

	pChannelInfos->vlc_options = NULL;

	return pChannelInfos;
}

void
freetuxtv_channel_infos_set_id(FreetuxTVChannelInfos* self, int id)
{
	self->id = id;
}

void
freetuxtv_channel_infos_set_position(FreetuxTVChannelInfos* self, int position)
{
	self->position = position;
}

void
freetuxtv_channel_infos_set_logo(FreetuxTVChannelInfos* self,
                                 gchar *logo_name)
{
	if(self->logo_name != NULL){
		g_free(self->logo_name);
	}
	self->logo_name = g_strdup(logo_name);
}

void
freetuxtv_channel_infos_set_vlcoptions(FreetuxTVChannelInfos* self,
                                       gchar **options)
{
	if(self->vlc_options){
		g_strfreev (self->vlc_options);
	} 
	self->vlc_options = g_strdupv(options);
}

void
freetuxtv_channel_infos_set_deinterlace_mode(FreetuxTVChannelInfos* self,
                                             gchar *mode)
{
	if(self->deinterlace_mode){
		g_free(self->deinterlace_mode);
	} 
	self->deinterlace_mode = g_strdup(mode);
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

