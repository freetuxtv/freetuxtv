/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2011 <eric.beuque@gmail.com>
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

#include "gtk-builder-window.h"

typedef struct _GtkBuilderWindowPrivate GtkBuilderWindowPrivate;
struct _GtkBuilderWindowPrivate
{
	GtkBuilder* builder;
	gchar* uifile;
	gchar* toplevel_widget_name;
};

#define GTK_BUILDER_WINDOW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_BUILDER_WINDOW, GtkBuilderWindowPrivate))

enum
{
	PROP_0,

	PROP_UIFILE,
	PROP_TOPLEVEL_WIDGET_NAME,
};


G_DEFINE_TYPE (GtkBuilderWindow, gtk_builder_window, GTK_TYPE_WINDOW);

static void
gtk_builder_window_init (GtkBuilderWindow *object)
{
	GtkBuilderWindowPrivate* priv;
	priv = GTK_BUILDER_WINDOW_PRIVATE(object);

	GtkBuilderWindowClass* klass;

	klass = GTK_BUILDER_WINDOW_GET_CLASS(object);

	priv->builder = NULL;
	priv->uifile = NULL;
	priv->toplevel_widget_name = NULL;

	// Construct a builder object 
	priv->builder = gtk_builder_new ();
}

static void
gtk_builder_window_constructed (GObject *object)
{
	GtkBuilderWindowPrivate* priv;
	priv = GTK_BUILDER_WINDOW_PRIVATE(object);

	GtkBuilderWindowClass* klass;
	klass = GTK_BUILDER_WINDOW_GET_CLASS(object);

	// Call parent function
	G_OBJECT_CLASS (gtk_builder_window_parent_class)->constructed (object);

	g_print("Loading \"%s\" from %s\n", priv->toplevel_widget_name, priv->uifile);
	
	gtk_builder_add_from_file (priv->builder, priv->uifile, NULL);

	GtkWidget* dialog;
	dialog = (GtkWidget*) gtk_builder_get_object (priv->builder, priv->toplevel_widget_name);

	// Get the child of the window in the ui file and add it in the current widget
	GtkWidget* child;
	child = gtk_bin_get_child(GTK_BIN(dialog));
	gtk_container_remove (GTK_CONTAINER(dialog), child);
	gtk_container_add (GTK_CONTAINER(object), child);

	// Copy the propertie of the window
	/*
	GParamSpec **ppListParamSpec;
	guint n_properties;
	ppListParamSpec = g_object_class_list_properties (G_OBJECT_GET_CLASS(dialog), &n_properties);

	int i;
	const gchar *property_name;
	gboolean bTest;
	
	for (i=0; i<n_properties; i++){

		bTest = ppListParamSpec[i] != NULL;
		if(bTest){
			bTest = (ppListParamSpec[i]->flags & G_PARAM_WRITABLE) && (ppListParamSpec[i]->flags & G_PARAM_READABLE);
		}
		if(bTest){
			bTest = !(ppListParamSpec[i]->flags & G_PARAM_CONSTRUCT_ONLY);
		}
		
		if(bTest){

			property_name = g_param_spec_get_name (ppListParamSpec[i]);
			
			g_print("Properties %s\n", property_name);

			GValue value = {0, };
			g_value_init (&value, ppListParamSpec[i]->value_type);
			
			g_object_get_property (G_OBJECT(object), property_name, &value);
			g_object_set_property (G_OBJECT(dialog), property_name, &value);

			//g_value_unset(value);
			//value = NULL;
			
			g_param_spec_unref (ppListParamSpec[i]);
			ppListParamSpec[i] = NULL;
		}
	}

	ppListParamSpec = NULL;
	*/
	
	const gchar* szTitle = gtk_window_get_title (GTK_WINDOW(dialog));
	gtk_window_set_title (GTK_WINDOW(object), szTitle);

	gboolean modal = gtk_window_get_modal (GTK_WINDOW(dialog));
	gtk_window_set_modal (GTK_WINDOW(object), modal);
	
	guint border_width = gtk_container_get_border_width (GTK_CONTAINER(dialog));
	gtk_container_set_border_width (GTK_CONTAINER(object), border_width);

	gint default_width;
	gint default_height;
	gtk_window_get_default_size (GTK_WINDOW(dialog), &default_width, &default_height);
	gtk_window_set_default_size (GTK_WINDOW(object), default_width, default_height);
}

static void
gtk_builder_window_finalize (GObject *object)
{
	GtkBuilderWindowPrivate* priv;
	priv = GTK_BUILDER_WINDOW_PRIVATE(object);
	
	g_print("Destroying \"%s\" from %s\n", priv->toplevel_widget_name, priv->uifile);
	
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
	
	G_OBJECT_CLASS (gtk_builder_window_parent_class)->finalize (object);
}

static void
gtk_builder_window_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_BUILDER_WINDOW (object));
	
	GtkBuilderWindow* self;
	self = GTK_BUILDER_WINDOW(object);

	GtkBuilderWindowPrivate* priv;
	priv = GTK_BUILDER_WINDOW_PRIVATE(object);

	switch (prop_id)
	{
	case PROP_UIFILE:
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
gtk_builder_window_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_BUILDER_WINDOW (object));

	GtkBuilderWindowPrivate* priv;
	priv = GTK_BUILDER_WINDOW_PRIVATE(object);

	switch (prop_id)
	{
	case PROP_UIFILE:
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
gtk_builder_window_class_init (GtkBuilderWindowClass *klass)
{
	GObjectClass* gobject_class = G_OBJECT_CLASS (klass);
	//GtkDialogClass* parent_class = GTK_DIALOG_CLASS (klass);

	gobject_class->constructed = gtk_builder_window_constructed;
	gobject_class->set_property = gtk_builder_window_set_property;
	gobject_class->get_property = gtk_builder_window_get_property;
	gobject_class->finalize = gtk_builder_window_finalize;

	g_type_class_add_private (klass, sizeof (GtkBuilderWindowPrivate));

	g_object_class_install_property (gobject_class,
	                                 PROP_UIFILE,
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
gtk_builder_window_get_builder(GtkBuilderWindow* dialog)
{
	g_return_val_if_fail (GTK_IS_BUILDER_WINDOW (dialog), NULL);

	GtkBuilderWindowPrivate* priv;
	priv = GTK_BUILDER_WINDOW_PRIVATE(dialog);

	return priv->builder;
}

