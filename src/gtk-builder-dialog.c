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

#include "gtk-builder-dialog.h"

#include <config.h>

typedef struct _GtkBuilderDialogPrivate GtkBuilderDialogPrivate;
struct _GtkBuilderDialogPrivate
{
	GtkBuilder* builder;
	gchar* uifile;
	gchar* toplevel_widget_name;
};

#define GTK_BUILDER_DIALOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_BUILDER_DIALOG, GtkBuilderDialogPrivate))

G_DEFINE_TYPE (GtkBuilderDialog, gtk_builder_dialog, GTK_TYPE_DIALOG);

enum
{
	PROP_0,

	PROP_UIFILE,
	PROP_TOPLEVEL_WIDGET_NAME,
};

static void
gtk_builder_dialog_init (GtkBuilderDialog *object)
{
	GtkBuilderDialogPrivate* priv;
	priv = GTK_BUILDER_DIALOG_PRIVATE(object);

	priv->builder = NULL;
	priv->uifile = NULL;
	priv->toplevel_widget_name = NULL;

	// Construct a builder object 
	priv->builder = gtk_builder_new ();
}

static void
gtk_builder_dialog_constructed (GObject *object)
{
	GtkBuilderDialogPrivate* priv;
	priv = GTK_BUILDER_DIALOG_PRIVATE(object);

	// Call parent function
	G_OBJECT_CLASS (gtk_builder_dialog_parent_class)->constructed (object);

	g_print("Loading dialog \"%s\" from %s\n", priv->toplevel_widget_name, priv->uifile);
	
	gtk_builder_add_from_file (priv->builder, priv->uifile, NULL);

	GtkWidget* dialog;
	dialog = (GtkWidget*) gtk_builder_get_object (priv->builder, priv->toplevel_widget_name);

	// Get the child of the window in the ui file and add it in the current widget
	GList *children;
	GtkWidget* content_area;
	GtkWidget* action_area;
	GtkWidget* dest_content_area;
	GtkWidget* child;
	
	content_area = gtk_dialog_get_content_area (GTK_DIALOG(dialog));
	action_area = gtk_dialog_get_action_area (GTK_DIALOG(dialog));
	dest_content_area = gtk_dialog_get_content_area (GTK_DIALOG(object));
	
	children = gtk_container_get_children(GTK_CONTAINER(content_area));
	while(children != NULL){
		child = (GtkWidget*)children->data;

		if(GTK_IS_WIDGET(child)){
			if(action_area != child){
				gtk_container_remove (GTK_CONTAINER(content_area), child);
				if(GTK_IS_WIDGET(child)){
					gtk_container_add (GTK_CONTAINER(dest_content_area), child);
				}
			}
		}

		children = g_list_next(children);
	}

	gint response;
	children = gtk_container_get_children(GTK_CONTAINER(action_area));
	while(children != NULL){
		child = (GtkWidget*)children->data;

		response = gtk_dialog_get_response_for_widget (GTK_DIALOG(dialog), child);
		
		if(GTK_IS_WIDGET(child)){
			gtk_container_remove (GTK_CONTAINER(action_area), child);

			gtk_dialog_add_action_widget (GTK_DIALOG(object), child, response);
		}

		children = g_list_next(children);
	}

	// Copy the propertie of the window
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

#if GTK_API_VERSION == 3

#else
	gboolean separator;
	separator = gtk_dialog_get_has_separator (GTK_DIALOG(dialog));
	gtk_dialog_set_has_separator (GTK_DIALOG(object), separator);
#endif
}

static void
gtk_builder_dialog_finalize (GObject *object)
{
	GtkBuilderDialogPrivate* priv;
	priv = GTK_BUILDER_DIALOG_PRIVATE(object);
	
	g_print("Destroying dialog \"%s\" from %s\n", priv->toplevel_widget_name, priv->uifile);
	
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

	G_OBJECT_CLASS (gtk_builder_dialog_parent_class)->finalize (object);
}

static void
gtk_builder_dialog_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_BUILDER_DIALOG (object));

	GtkBuilderDialogPrivate* priv;
	priv = GTK_BUILDER_DIALOG_PRIVATE(object);

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
gtk_builder_dialog_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_BUILDER_DIALOG (object));

	GtkBuilderDialogPrivate* priv;
	priv = GTK_BUILDER_DIALOG_PRIVATE(object);

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
gtk_builder_dialog_class_init (GtkBuilderDialogClass *klass)
{
	GObjectClass* gobject_class = G_OBJECT_CLASS (klass);

	gobject_class->constructed = gtk_builder_dialog_constructed;
	gobject_class->set_property = gtk_builder_dialog_set_property;
	gobject_class->get_property = gtk_builder_dialog_get_property;
	gobject_class->finalize = gtk_builder_dialog_finalize;

	g_type_class_add_private (klass, sizeof (GtkBuilderDialogPrivate));

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
gtk_builder_dialog_get_builder(GtkBuilderDialog* dialog)
{
	g_return_val_if_fail (GTK_IS_BUILDER_DIALOG (dialog), NULL);

	GtkBuilderDialogPrivate* priv;
	priv = GTK_BUILDER_DIALOG_PRIVATE(dialog);

	return priv->builder;
}
