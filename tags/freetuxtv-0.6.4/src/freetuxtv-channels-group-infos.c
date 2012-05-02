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

#include "freetuxtv-channels-group-infos.h"

typedef struct _FreetuxTVChannelsGroupInfosPrivate FreetuxTVChannelsGroupInfosPrivate;
struct _FreetuxTVChannelsGroupInfosPrivate
{
	gchar* nil;
};

#define FREETUXTV_CHANNELS_GROUP_INFOS_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, FreetuxTVChannelsGroupInfosPrivate))



G_DEFINE_TYPE (FreetuxTVChannelsGroupInfos, freetuxtv_channels_group_infos, G_TYPE_OBJECT);

static void
freetuxtv_channels_group_infos_init (FreetuxTVChannelsGroupInfos *object)
{
	object->id=-1;
	object->name=NULL;
	object->uri=NULL;
	object->bregex=NULL;
	object->eregex=NULL;
	object->nb_channels = 0;
}

static void
freetuxtv_channels_group_infos_finalize (GObject *object)
{
	FreetuxTVChannelsGroupInfos *self;
	self = FREETUXTV_CHANNELS_GROUP_INFOS(object);

	if(self->name != NULL){
		g_free(self->name);
		self->name = NULL;
	}

	if(self->uri != NULL){
		g_free(self->uri);
		self->uri = NULL;
	}

	if(self->bregex != NULL){
		g_free(self->bregex);
		self->bregex = NULL;
	}

	if(self->eregex != NULL){
		g_free(self->eregex);
		self->eregex = NULL;
	}

	G_OBJECT_CLASS (freetuxtv_channels_group_infos_parent_class)->finalize (object);
}

static void
freetuxtv_channels_group_infos_class_init (FreetuxTVChannelsGroupInfosClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	// GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVChannelsGroupInfosPrivate));

	object_class->finalize = freetuxtv_channels_group_infos_finalize;
}


FreetuxTVChannelsGroupInfos*
freetuxtv_channels_group_infos_new(gchar *name, FREETUXTV_CHANNELSGROUP_TYPEGROUP type)
{
	g_return_val_if_fail(name != NULL, NULL);

	FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;
	pChannelsGroupInfos = g_object_new (FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, NULL);

	freetuxtv_channels_group_infos_set_name (pChannelsGroupInfos, name);
	
	pChannelsGroupInfos->name=g_strdup(name);
	pChannelsGroupInfos->uri = NULL;
	pChannelsGroupInfos->type = type;
	pChannelsGroupInfos->nb_channels = 0;

	return pChannelsGroupInfos;
}

void
freetuxtv_channels_group_infos_set_id (FreetuxTVChannelsGroupInfos *self, int id)
{
	self->id=id;	
}

void
freetuxtv_channels_group_infos_set_position (FreetuxTVChannelsGroupInfos *self, int position)
{
	self->position=position;
}

void
freetuxtv_channels_group_infos_set_name (FreetuxTVChannelsGroupInfos *self, gchar *name)
{
	g_return_if_fail(name != NULL);
	
	if(name != self->name){
		if(self->name != NULL){
			g_free(self->name);
			self->name = NULL;
		}		
		self->name=g_strdup(name);
	}
}

void
freetuxtv_channels_group_infos_set_uri (FreetuxTVChannelsGroupInfos *self, gchar *uri)
{	
	if(uri != self->uri){
		if(self->uri != NULL){
			g_free(self->uri);
			self->uri = NULL;
		}		
		self->uri=g_strdup(uri);
	}
}

void
freetuxtv_channels_group_infos_set_regex (FreetuxTVChannelsGroupInfos *self, gchar *bregex, gchar *eregex)
{
	if(bregex != self->bregex){
		if(self->bregex != NULL){
			g_free(self->bregex);
			self->bregex = NULL;
		}	
		self->bregex=g_strdup(bregex);
	}
	
	if(eregex != self->eregex){
		if(self->eregex != NULL){
			g_free(self->eregex);
			self->eregex = NULL;
		}		
		self->eregex=g_strdup(eregex);
	}
}