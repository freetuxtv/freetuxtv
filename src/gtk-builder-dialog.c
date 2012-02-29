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
	gint ruf;
};

#define GTK_BUILDER_DIALOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_BUILDER_DIALOG, GtkBuilderDialogPrivate))

G_DEFINE_TYPE (GtkBuilderDialog, gtk_builder_dialog, GTK_TYPE_BUILDER_WINDOW);

enum
{
	PROP_0,

	PROP_UIFILE,
	PROP_TOPLEVEL_WIDGET_NAME,
};

static void
gtk_builder_dialog_init (GtkBuilderDialog *object)
{
	
}

static void
gtk_builder_dialog_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtk_builder_dialog_parent_class)->finalize (object);
}

static void
gtk_builder_dialog_class_init (GtkBuilderDialogClass *klass)
{
	GObjectClass* gobject_class = G_OBJECT_CLASS (klass);

	gobject_class->finalize = gtk_builder_dialog_finalize;

	g_type_class_add_private (klass, sizeof (GtkBuilderDialogPrivate));
}

gint
gtk_builder_dialog_run(GtkBuilderDialog* dialog)
{
	GtkWindow* pWindow;
	pWindow = gtk_builder_window_get_top_window(GTK_BUILDER_WINDOW(dialog));
	return gtk_dialog_run(GTK_DIALOG(pWindow));
}
