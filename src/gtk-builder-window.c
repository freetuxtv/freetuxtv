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
	gint ruf;
};

#define GTK_BUILDER_WINDOW_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_BUILDER_WINDOW, GtkBuilderWindowPrivate))

G_DEFINE_TYPE (GtkBuilderWindow, gtk_builder_window, GTK_TYPE_BUILDER_WIDGET);

static void
gtk_builder_window_init (GtkBuilderWindow *object)
{

}

static void
gtk_builder_window_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtk_builder_window_parent_class)->finalize (object);
}

static void
gtk_builder_window_class_init (GtkBuilderWindowClass *klass)
{
	GObjectClass* gobject_class = G_OBJECT_CLASS (klass);

	gobject_class->finalize = gtk_builder_window_finalize;

	g_type_class_add_private (klass, sizeof (GtkBuilderWindowPrivate));
}

GtkWindow*
gtk_builder_window_get_top_window(GtkBuilderWindow* object)
{
	g_return_val_if_fail (GTK_IS_BUILDER_WINDOW (object), NULL);

	GObject* pObject = gtk_builder_object_get_top_object (GTK_BUILDER_OBJECT(object));
	
	g_return_val_if_fail(GTK_IS_WINDOW(pObject), NULL);
	return GTK_WINDOW(pObject);
}

