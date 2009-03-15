/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-channel-infos.h"

G_DEFINE_TYPE (FreetuxTVChannelInfos, freetuxtv_channel_infos, G_TYPE_OBJECT);

FreetuxTVChannelInfos*
freetuxtv_channel_infos_new(int id, int rank, char *name, char *url)
{
	FreetuxTVChannelInfos *channel_infos;
	channel_infos = g_object_new (FREETUXTV_TYPE_CHANNEL_INFOS, NULL);

	
	channel_infos->id = id;
	channel_infos->rank = rank;
	channel_infos->name = g_strdup(name);
	channel_infos->url = g_strdup(url);	

	channel_infos->logo_name = NULL;

	return channel_infos;
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
freetuxtv_channel_infos_class_init (FreetuxTVChannelInfosClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->constructor = freetuxtv_channel_infos_constructor;
}

static void
freetuxtv_channel_infos_init (FreetuxTVChannelInfos *self)
{
	self->name=NULL;
	self->url=NULL;
	self->redirect_url=NULL;

	self->logo_name = NULL;
}

