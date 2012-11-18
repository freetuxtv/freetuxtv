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

#ifndef _GTK_PROGRESS_DIALOG_H_
#define _GTK_PROGRESS_DIALOG_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_TYPE_PROGRESS_DIALOG             (gtk_progress_dialog_get_type ())
#define GTK_PROGRESS_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PROGRESS_DIALOG, GtkProgressDialog))
#define GTK_PROGRESS_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_PROGRESS_DIALOG, GtkProgressDialogClass))
#define GTK_IS_PROGRESS_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PROGRESS_DIALOG))
#define GTK_IS_PROGRESS_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PROGRESS_DIALOG))
#define GTK_PROGRESS_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PROGRESS_DIALOG, GtkProgressDialogClass))

typedef struct _GtkProgressDialogClass GtkProgressDialogClass;
typedef struct _GtkProgressDialog GtkProgressDialog;

struct _GtkProgressDialogClass
{
	GtkDialogClass parent_class;
};

struct _GtkProgressDialog
{
	GtkDialog parent_instance;
};

GType gtk_progress_dialog_get_type (void) G_GNUC_CONST;

G_END_DECLS

GtkProgressDialog*
gtk_progress_dialog_new(GtkWindow* parent);

void
gtk_progress_dialog_set_title(GtkProgressDialog* dialog, gchar *title);

void
gtk_progress_dialog_set_text(GtkProgressDialog* dialog, gchar *text);

void
gtk_progress_dialog_set_percent(GtkProgressDialog* dialog, gdouble percent);

void
gtk_progress_dialog_pulse(GtkProgressDialog* dialog);

void
gtk_progress_dialog_set_button_close_enabled(GtkProgressDialog* dialog, gboolean enabled);

#endif /* _GTK_PROGRESS_DIALOG_H_ */
