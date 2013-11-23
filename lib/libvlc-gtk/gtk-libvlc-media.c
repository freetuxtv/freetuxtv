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

#include "gtk-libvlc-media.h"

typedef struct _GtkLibvlcMediaPrivate GtkLibvlcMediaPrivate;
struct _GtkLibvlcMediaPrivate
{
	gchar** options;
};

#define GTK_LIBVLC_MEDIA_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_LIBVLC_MEDIA, GtkLibvlcMediaPrivate))



G_DEFINE_TYPE (GtkLibvlcMedia, gtk_libvlc_media, G_TYPE_OBJECT);

static void
gtk_libvlc_media_init (GtkLibvlcMedia *object)
{
	GtkLibvlcMediaPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PRIVATE(object);
	
	object->mrl = NULL;
	
	priv->options = NULL;
}

static void
gtk_libvlc_media_finalize (GObject *object)
{
	GtkLibvlcMedia *self;
	GtkLibvlcMediaPrivate* priv;
	
	self = GTK_LIBVLC_MEDIA(object);
	priv = GTK_LIBVLC_MEDIA_PRIVATE(self);
	
	if(self->mrl != NULL){
		g_free(self->mrl);
		self->mrl = NULL;
	}

	if(priv->options != NULL){
		g_strfreev(priv->options);
		priv->options = NULL;
	}

	G_OBJECT_CLASS (gtk_libvlc_media_parent_class)->finalize (object);
}

static void
gtk_libvlc_media_class_init (GtkLibvlcMediaClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkLibvlcMediaPrivate));

	object_class->finalize = gtk_libvlc_media_finalize;
}

/**
 * gtk_libvlc_media_new:
 * @mrl: the MRL (media resource locator) of the media.
 *
 * Create a new media to be played in a #GtkLibVLCMediaPlayer with the given mrl.
 *
 * Returns: a media. Should be freed when it is no longer needed with g_object_unref().
 *
 * Since: 0.1
 */
GtkLibvlcMedia*
gtk_libvlc_media_new (gchar* mrl)
{
	g_return_val_if_fail(mrl != NULL, NULL);

	GtkLibvlcMedia *self = NULL;
	self = g_object_new (GTK_TYPE_LIBVLC_MEDIA, NULL);

	self->mrl = g_strdup(mrl);
	
	return self;
}

/**
 * gtk_libvlc_set_options:
 * @media: a pointer to the media.
 * @options: the VLC options to add to the media 
 *
 * Set an array of options to the media.
 *
 * Since: 0.1
 */
void
gtk_libvlc_media_set_options (GtkLibvlcMedia* media, gchar** options)
{
	g_return_if_fail(media != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA(media));
	
	GtkLibvlcMediaPrivate* priv;
	
	priv = GTK_LIBVLC_MEDIA_PRIVATE(media);
	
	if(priv->options){
		g_strfreev(priv->options);
		priv->options = NULL;
	}
	priv->options = g_strdupv(options);
}

/**
 * gtk_libvlc_add_option:
 * @media: a pointer to the media.
 * @option: the VLC option to add to the media 
 *
 * Add an option to the media.
 *
 * Since: 0.1
 */
void
gtk_libvlc_media_add_option (GtkLibvlcMedia* media, gchar* option)
{
	g_return_if_fail(media != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA(media));
	g_return_if_fail(option != NULL);
	
	GtkLibvlcMediaPrivate* priv;
	
	priv = GTK_LIBVLC_MEDIA_PRIVATE(media);
	
	gchar *options[2];
	options[0] = option;
	options[1] = NULL;

	gtk_libvlc_media_add_options (media, options);
}

/**
 * gtk_libvlc_add_options:
 * @media: a pointer to the media.
 * @option: the VLC options to add to the media 
 *
 * Add options to the media.
 *
 * Since: 0.1
 */
void
gtk_libvlc_media_add_options (GtkLibvlcMedia* media, gchar** options)
{
	g_return_if_fail(media != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA(media));
	
	GtkLibvlcMediaPrivate* priv;
	
	priv = GTK_LIBVLC_MEDIA_PRIVATE(media);

	gchar* szOriginalOptions = NULL;
	gchar* szNewOptions = NULL;
	gchar* szFinalOptions = NULL;

	if(priv->options){
		szOriginalOptions = g_strjoinv("\t", priv->options);
	}
	if(options){
		szNewOptions = g_strjoinv("\t", options);
	}
	if(szOriginalOptions && szNewOptions){
		szFinalOptions = g_strconcat(szOriginalOptions, "\t", szNewOptions, NULL);
	}else{
		if(szOriginalOptions){
			szFinalOptions = g_strdup(szOriginalOptions);
		}
		if(szNewOptions){
			szFinalOptions = g_strdup(szNewOptions);
		}
	}

	if(priv->options){
		g_strfreev(priv->options);
		priv->options = NULL;
	}
	if(szFinalOptions){
		priv->options = g_strsplit(szFinalOptions, "\t", 0);
	}

	if(szOriginalOptions){
		g_free(szOriginalOptions);
		szOriginalOptions = NULL;
	}

	if(szNewOptions){
		g_free(szNewOptions);
		szNewOptions = NULL;
	}

	if(szFinalOptions){
		g_free(szFinalOptions);
		szFinalOptions = NULL;
	}
}

/**
 * gtk_libvlc_get_options:
 * @media: a pointer to the media.
 *
 * Get the list of options options to the media.
 *
 * Returns: an array of the options attached to the media.
 *          Return NULL if no options attached to the media.
 *
 * Since: 0.1
 */
const gchar**
gtk_libvlc_media_get_options (GtkLibvlcMedia* media)
{
	g_return_val_if_fail(media != NULL, NULL);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA(media), NULL);
	
	GtkLibvlcMediaPrivate* priv;
	gchar** options = NULL;
	
	priv = GTK_LIBVLC_MEDIA_PRIVATE(media);

	options = priv->options;
	/*
	  if(media->options){
	  options = g_strdupv(media->options);
	  }
	*/
	
	return (const gchar**)options;
}
