/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-channels-group-infos.h"

G_DEFINE_TYPE (FreetuxTVChannelsGroupInfos, freetuxtv_channels_group_infos, G_TYPE_OBJECT);

FreetuxTVChannelsGroupInfos*
freetuxtv_channels_group_infos_new(int id, int rank, char *name, char *uri)
{
	FreetuxTVChannelsGroupInfos *channels_group_infos;
	channels_group_infos = g_object_new (FREETUXTV_TYPE_CHANNELS_GROUP_INFOS, NULL);

	channels_group_infos->id=id;
	channels_group_infos->rank=rank;
	channels_group_infos->name=g_strdup(name);
	channels_group_infos->uri=g_strdup(uri);	

	return channels_group_infos;
}

static GObject *
freetuxtv_channels_group_infos_constructor (GType                  gtype,
				     guint                  n_properties,
				     GObjectConstructParam *properties)
{
	GObject *obj;
	FreetuxTVChannelsGroupInfosClass *klass;
	GObjectClass *parent_class;  
	parent_class = G_OBJECT_CLASS (freetuxtv_channels_group_infos_parent_class);
	obj = parent_class->constructor (gtype, n_properties, properties);
	return obj;
}

static void
freetuxtv_channels_group_infos_class_init (FreetuxTVChannelsGroupInfosClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->constructor = freetuxtv_channels_group_infos_constructor;
}

static void
freetuxtv_channels_group_infos_init (FreetuxTVChannelsGroupInfos *self)
{
	self->name=NULL;
	self->uri=NULL;
}

