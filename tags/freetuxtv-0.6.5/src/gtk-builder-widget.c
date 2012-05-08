/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2012 <eric.beuque@gmail.com>
 * 
freetuxtv is free software: you can redistribute it and/or modify it
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

#include "gtk-builder-widget.h"

enum
{
	SIGNAL_DESTROY,

	LAST_SIGNAL
};


static guint builder_widget_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GtkBuilderWidget, gtk_builder_widget, GTK_TYPE_BUILDER_OBJECT);

static void
gtk_builder_widget_init (GtkBuilderWidget *object)
{
	
}

static void
on_destroy (GtkBuilderWidget *self, gpointer user_data)
{
	g_signal_emit (
	    G_OBJECT (self),
	    builder_widget_signals [SIGNAL_DESTROY],
	    0);
}

static void
gtk_builder_widget_constructed (GObject *object)
{
	// Call parent function
    if (G_OBJECT_CLASS (gtk_builder_widget_parent_class)->constructed)
        G_OBJECT_CLASS (gtk_builder_widget_parent_class)->constructed (object);

	g_signal_connect(G_OBJECT(object), "destroy",
		G_CALLBACK(on_destroy), NULL);
}

static void
gtk_builder_widget_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtk_builder_widget_parent_class)->finalize (object);
}

static void
gtk_builder_widget_on_destroy (GtkBuilderWidget *self, gpointer user_data)
{
	
}

static void
gtk_builder_widget_class_init (GtkBuilderWidgetClass *klass)
{
	GObjectClass* gobject_class = G_OBJECT_CLASS (klass);

	gobject_class->constructed = gtk_builder_widget_constructed;
	gobject_class->finalize = gtk_builder_widget_finalize;

	klass->destroy = gtk_builder_widget_on_destroy;

	builder_widget_signals[SIGNAL_DESTROY] =
		g_signal_new ("destroy",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (GtkBuilderWidgetClass, destroy),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__OBJECT,
		              G_TYPE_NONE, 0, NULL);
}

void
gtk_builder_widget_destroy(GtkBuilderWidget* object)
{
	GtkWidget* widget;
	widget = (GtkWidget*)gtk_builder_object_get_top_object (GTK_BUILDER_OBJECT(object));
	gtk_widget_destroy(widget);

	g_object_unref (G_OBJECT(object));
}
