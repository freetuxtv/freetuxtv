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

#ifndef _GTK_BUILDER_OBJECT_H_
#define _GTK_BUILDER_OBJECT_H_

#include <glib-object.h>

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_TYPE_BUILDER_OBJECT             (gtk_builder_object_get_type ())
#define GTK_BUILDER_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BUILDER_OBJECT, GtkBuilderObject))
#define GTK_BUILDER_OBJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_BUILDER_OBJECT, GtkBuilderObjectClass))
#define GTK_IS_BUILDER_OBJECT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BUILDER_OBJECT))
#define GTK_IS_BUILDER_OBJECT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_BUILDER_OBJECT))
#define GTK_BUILDER_OBJECT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_BUILDER_OBJECT, GtkBuilderObjectClass))

typedef struct _GtkBuilderObjectClass GtkBuilderObjectClass;
typedef struct _GtkBuilderObject GtkBuilderObject;

struct _GtkBuilderObjectClass
{
	GObjectClass parent_class;
};

struct _GtkBuilderObject
{
	GObject parent_instance;
};

GType gtk_builder_object_get_type (void) G_GNUC_CONST;

GtkBuilder*
gtk_builder_object_get_builder(GtkBuilderObject* object);

GObject*
gtk_builder_object_get_object(GtkBuilderObject* object, gchar* object_name);

GObject*
gtk_builder_object_get_top_object(GtkBuilderObject* object);

G_END_DECLS

#endif /* _GTK_BUILDER_OBJECT_H_ */
