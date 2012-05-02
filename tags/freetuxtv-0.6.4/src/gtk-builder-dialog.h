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

#ifndef _GTK_BUILDER_DIALOG_H_
#define _GTK_BUILDER_DIALOG_H_

#include <glib-object.h>

#include <gtk/gtk.h>

#include "gtk-builder-window.h"

G_BEGIN_DECLS

#define GTK_TYPE_BUILDER_DIALOG             (gtk_builder_dialog_get_type ())
#define GTK_BUILDER_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BUILDER_DIALOG, GtkBuilderDialog))
#define GTK_BUILDER_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_BUILDER_DIALOG, GtkBuilderDialogClass))
#define GTK_IS_BUILDER_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BUILDER_DIALOG))
#define GTK_IS_BUILDER_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_BUILDER_DIALOG))
#define GTK_BUILDER_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_BUILDER_DIALOG, GtkBuilderDialogClass))

typedef struct _GtkBuilderDialogClass GtkBuilderDialogClass;
typedef struct _GtkBuilderDialog GtkBuilderDialog;

struct _GtkBuilderDialogClass
{
	GtkBuilderWindowClass parent_class;
};

struct _GtkBuilderDialog
{
	GtkBuilderWindow parent_instance;
};

GType gtk_builder_dialog_get_type (void) G_GNUC_CONST;

gint
gtk_builder_dialog_run(GtkBuilderDialog* dialog);

G_END_DECLS

#endif /* _GTK_BUILDER_DIALOG_H_ */
