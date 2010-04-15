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

#include "freetuxtv-channels-group-infos.h"

G_DEFINE_TYPE (FreetuxTVChannelsGroupInfos, freetuxtv_channels_group_infos, G_TYPE_OBJECT);

FreetuxTVChannelsGroupInfos*
freetuxtv_channels_group_infos_new(gchar *name, gchar *uri)
{
	g_return_val_if_fail(name != NULL, NULL);

	FreetuxTVChannelsGroupInfos *channels_group_infos;
	channels_group_infos = g_object_new (FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, NULL);

	freetuxtv_channels_group_infos_set_name (channels_group_infos, name);
	
	channels_group_infos->name=g_strdup(name);
	channels_group_infos->uri=g_strdup(uri);
	channels_group_infos->nb_channels = 0;	
	channels_group_infos->nb_channels_visible = 0;

	return channels_group_infos;
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

static GObject *
freetuxtv_channels_group_infos_constructor (GType gtype,
					    guint n_properties,
					    GObjectConstructParam *properties)
{
	GObject *obj;
	GObjectClass *parent_class;  
	parent_class = G_OBJECT_CLASS (freetuxtv_channels_group_infos_parent_class);
	obj = parent_class->constructor (gtype, n_properties, properties);
	return obj;
}

static void
freetuxtv_channels_group_infos_dispose (GObject *object)
{
	FreetuxTVChannelsGroupInfos *self;
	
	g_return_if_fail(object != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(object));
	 
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

	G_OBJECT_CLASS (freetuxtv_channels_group_infos_parent_class)->dispose (object);
	
}

static void
freetuxtv_channels_group_infos_class_init (FreetuxTVChannelsGroupInfosClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->constructor = freetuxtv_channels_group_infos_constructor;
	gobject_class->dispose = freetuxtv_channels_group_infos_dispose;
}

static void
freetuxtv_channels_group_infos_init (FreetuxTVChannelsGroupInfos *self)
{
	self->id=-1;
	self->name=NULL;
	self->uri=NULL;
	self->bregex=NULL;
	self->eregex=NULL;
	self->nb_channels = 0;	
	self->nb_channels_visible = 0;
}

