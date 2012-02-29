/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2012 <eric.beuque@gmail.com>
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

#include "gtk-builder-object.h"

typedef struct _GtkBuilderObjectPrivate GtkBuilderObjectPrivate;
struct _GtkBuilderObjectPrivate
{
	GtkBuilder* builder;
	gchar* uifile;
	gchar* toplevel_widget_name;
};

#define GTK_BUILDER_OBJECT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_BUILDER_OBJECT, GtkBuilderObjectPrivate))

enum
{
	PROP_0,

	PROP_UI_FILE,
	PROP_TOPLEVEL_WIDGET_NAME
};



G_DEFINE_TYPE (GtkBuilderObject, gtk_builder_object, G_TYPE_OBJECT);

static void
gtk_builder_object_init (GtkBuilderObject *object)
{
	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	priv->builder = NULL;
	priv->uifile = NULL;
	priv->toplevel_widget_name = NULL;

	// Construct a builder object 
	priv->builder = gtk_builder_new ();
}

static void
gtk_builder_object_constructed (GObject *object)
{
	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	g_log("FreetuxTV", G_LOG_LEVEL_INFO,
	    "Loading GtkBuilder object \"%s\" from %s\n",
	    priv->toplevel_widget_name, priv->uifile);
	
	gtk_builder_add_from_file (priv->builder, priv->uifile, NULL);
	
	// Call parent function
    if (G_OBJECT_CLASS (gtk_builder_object_parent_class)->constructed)
        G_OBJECT_CLASS (gtk_builder_object_parent_class)->constructed (object);
}

static void
gtk_builder_object_finalize (GObject *object)
{
	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	g_log("FreetuxTV", G_LOG_LEVEL_INFO,
	    "Destroying GtkBuilder object \"%s\" from %s\n",
	    priv->toplevel_widget_name, priv->uifile);
	
	if(priv->uifile){
		g_free(priv->uifile);
		priv->uifile = NULL;
	}
	
	if(priv->toplevel_widget_name){
		g_free(priv->toplevel_widget_name);
		priv->toplevel_widget_name = NULL;
	}
	
	if(priv->builder){
		g_object_unref (priv->builder);
		priv->builder = NULL;
	}

	G_OBJECT_CLASS (gtk_builder_object_parent_class)->finalize (object);
}

static void
gtk_builder_object_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_BUILDER_OBJECT (object));

	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	switch (prop_id)
	{
	case PROP_UI_FILE:
		if(priv->uifile){
			g_free(priv->uifile);
			priv->uifile = NULL;
		}
		priv->uifile = g_strdup(g_value_get_string(value));
		break;
	case PROP_TOPLEVEL_WIDGET_NAME:
		if(priv->toplevel_widget_name){
			g_free(priv->toplevel_widget_name);
			priv->toplevel_widget_name = NULL;
		}
		priv->toplevel_widget_name = g_strdup(g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gtk_builder_object_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_BUILDER_OBJECT (object));

	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	switch (prop_id)
	{
	case PROP_UI_FILE:
		g_value_set_string (value, priv->uifile);
		break;
	case PROP_TOPLEVEL_WIDGET_NAME:
		g_value_set_string (value, priv->toplevel_widget_name);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gtk_builder_object_class_init (GtkBuilderObjectClass *klass)
{
	GObjectClass* gobject_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkBuilderObjectPrivate));

	gobject_class->constructed = gtk_builder_object_constructed;
	gobject_class->finalize = gtk_builder_object_finalize;
	gobject_class->set_property = gtk_builder_object_set_property;
	gobject_class->get_property = gtk_builder_object_get_property;

	g_object_class_install_property (gobject_class,
	    PROP_UI_FILE,
	    g_param_spec_string ("ui-file",
		    "UI File",
		    "The GtkBuilder UI File to use",
		    NULL,
		    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (gobject_class,
	    PROP_TOPLEVEL_WIDGET_NAME,
	    g_param_spec_string ("toplevel-widget-name",
		    "Toplevel widget name",
		    "The TopLevel widget name in the ui file",
		    NULL,
		    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

GtkBuilder*
gtk_builder_object_get_builder(GtkBuilderObject* object)
{
	g_return_val_if_fail (GTK_IS_BUILDER_OBJECT (object), NULL);

	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	return priv->builder;
}

GObject*
gtk_builder_object_get_object(GtkBuilderObject* object, gchar* object_name)
{
	g_return_val_if_fail (GTK_IS_BUILDER_OBJECT (object), NULL);

	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	GObject* pObject;
	pObject = gtk_builder_get_object (priv->builder, object_name);
	return pObject;
}

GObject*
gtk_builder_object_get_top_object(GtkBuilderObject* object)
{
	g_return_val_if_fail (GTK_IS_BUILDER_OBJECT (object), NULL);

	GtkBuilderObjectPrivate* priv;
	priv = GTK_BUILDER_OBJECT_PRIVATE(object);

	return gtk_builder_object_get_object(object,
	    priv->toplevel_widget_name);
}