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

#include "freetuxtv-tv-channel-infos.h"

typedef struct _FreetuxTVTvChannelInfosPrivate FreetuxTVTvChannelInfosPrivate;
struct _FreetuxTVTvChannelInfosPrivate
{
	gint id;

	gchar* name;

	gchar* logo_filename;

	GList* labels;
};

#define FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_TV_CHANNEL_INFOS, FreetuxTVTvChannelInfosPrivate))



G_DEFINE_TYPE (FreetuxTVTvChannelInfos, freetuxtv_tv_channel_infos, G_TYPE_OBJECT);

static void
freetuxtv_tv_channel_infos_init (FreetuxTVTvChannelInfos *object)
{
	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(object);

	priv->id = -1;
	priv->name = NULL;

	priv->labels = NULL;
}

static void
freetuxtv_tv_channel_infos_finalize (GObject *object)
{
	FreetuxTVTvChannelInfosPrivate* priv;
	gchar* text;
	GList* list_tmp;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(object);

	if(priv->name){
		g_free(priv->name);
		priv->name = NULL;
	}

	list_tmp = priv->labels;
	while(list_tmp){
		text = (gchar*) list_tmp->data;
		g_free(text);
		list_tmp->data = NULL;
		list_tmp = list_tmp->next;
	}
	if(priv->labels){
		g_list_free(priv->labels);
		priv->labels = NULL;
	}
	
	G_OBJECT_CLASS (freetuxtv_tv_channel_infos_parent_class)->finalize (object);
}

static void
freetuxtv_tv_channel_infos_class_init (FreetuxTVTvChannelInfosClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
//	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVTvChannelInfosPrivate));

	object_class->finalize = freetuxtv_tv_channel_infos_finalize;
}

FreetuxTVTvChannelInfos*
freetuxtv_tv_channel_infos_new(gchar* name)
{
	FreetuxTVTvChannelInfos* tv_channel_infos;
	FreetuxTVTvChannelInfosPrivate* priv;
	
	tv_channel_infos = g_object_new (FREETUXTV_TYPE_TV_CHANNEL_INFOS, NULL);
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	priv->name = g_strdup(name);

	return tv_channel_infos;
}

void
freetuxtv_tv_channel_infos_set_id(FreetuxTVTvChannelInfos* tv_channel_infos, gint id)
{
	g_return_if_fail(tv_channel_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos));
	
	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	priv->id = id;
}

gint
freetuxtv_tv_channel_infos_get_id(FreetuxTVTvChannelInfos* tv_channel_infos)
{
	g_return_val_if_fail(tv_channel_infos != NULL, -1);
	g_return_val_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos), -1);

	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	return priv->id;

}

void
freetuxtv_tv_channel_infos_set_name(FreetuxTVTvChannelInfos* tv_channel_infos, gchar* name)
{
	g_return_if_fail(tv_channel_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos));
	
	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	if(priv->name){
		g_free(priv->name);
		priv->name = NULL;
	}

	priv->name = g_strdup(name);
}

const gchar*
freetuxtv_tv_channel_infos_get_name(FreetuxTVTvChannelInfos* tv_channel_infos)
{
	g_return_val_if_fail(tv_channel_infos != NULL, NULL);
	g_return_val_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos), NULL);
	
	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	return priv->name;
}

void
freetuxtv_tv_channel_infos_set_logo_filename(FreetuxTVTvChannelInfos* tv_channel_infos, gchar* logo_filename)
{
	g_return_if_fail(tv_channel_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos));
	
	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	if(priv->logo_filename){
		g_free(priv->logo_filename);
		priv->logo_filename = NULL;
	}

	priv->logo_filename = g_strdup(logo_filename);
}

const gchar*
freetuxtv_tv_channel_infos_get_logo_filename(FreetuxTVTvChannelInfos* tv_channel_infos)
{
	g_return_val_if_fail(tv_channel_infos != NULL, NULL);
	g_return_val_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos), NULL);
	
	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	return priv->logo_filename;
}

void
freetuxtv_tv_channel_infos_add_label(FreetuxTVTvChannelInfos* tv_channel_infos, gchar *label)
{
	g_return_if_fail(tv_channel_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos));
	
	FreetuxTVTvChannelInfosPrivate* priv;
	gchar* text;

	text = g_strdup(label);
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	priv->labels = g_list_append (priv->labels, text);
}

GList*
freetuxtv_tv_channel_infos_get_labels(FreetuxTVTvChannelInfos* tv_channel_infos)
{
	g_return_val_if_fail(tv_channel_infos != NULL, NULL);
	g_return_val_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos), NULL);
	
	FreetuxTVTvChannelInfosPrivate* priv;
	
	priv = FREETUXTV_TV_CHANNEL_INFOS_PRIVATE(tv_channel_infos);

	return priv->labels;
}