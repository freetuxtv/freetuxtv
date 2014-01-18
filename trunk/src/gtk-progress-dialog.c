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

#include "gtk-progress-dialog.h"

#include <config.h>

typedef struct _GtkProgressDialogPrivate GtkProgressDialogPrivate;
struct _GtkProgressDialogPrivate
{
	GtkWindow* parent;

	GtkWidget* title_widget;
	GtkWidget* progress_widget;
	GtkWidget* text_widget;
};

#define GTK_PROGRESS_DIALOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_PROGRESS_DIALOG, GtkProgressDialogPrivate))

G_DEFINE_TYPE (GtkProgressDialog, gtk_progress_dialog, GTK_TYPE_DIALOG);

static void 
on_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data);

static void
gtk_progress_dialog_init (GtkProgressDialog *object)
{
	GtkProgressDialogPrivate* priv;
	
	priv = GTK_PROGRESS_DIALOG_PRIVATE(object);

	priv->parent = NULL;

	priv->title_widget = NULL;
	priv->progress_widget = NULL;
	priv->text_widget = NULL;
}

static void
gtk_progress_dialog_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtk_progress_dialog_parent_class)->finalize (object);
}

static void
gtk_progress_dialog_class_init (GtkProgressDialogClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GtkDialogClass* parent_class = GTK_DIALOG_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkProgressDialogPrivate));

	object_class->finalize = gtk_progress_dialog_finalize;
}

GtkProgressDialog*
gtk_progress_dialog_new(GtkWindow* parent)
{
	GtkProgressDialog* dialog;
	GtkProgressDialogPrivate* priv;

	dialog = g_object_new (GTK_TYPE_PROGRESS_DIALOG, NULL);

	priv = GTK_PROGRESS_DIALOG_PRIVATE(dialog);

	gtk_window_set_transient_for (GTK_WINDOW(dialog), GTK_WINDOW(parent));
	gtk_window_set_position (GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_default_size (GTK_WINDOW(dialog), 480, -1);
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
	//gtk_container_set_border_width (GTK_CONTAINER(dialog), 5);

	// Construct the dialog
	GtkWidget* vbox;
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_set_border_width (GTK_CONTAINER(vbox), 10);

	priv->title_widget = gtk_label_new("");
	gtk_misc_set_alignment (GTK_MISC(priv->title_widget), 0.00F, 0.50F);
	gtk_box_pack_start (GTK_BOX(vbox), priv->title_widget, FALSE, FALSE, 0);

	priv->progress_widget = gtk_progress_bar_new();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(priv->progress_widget), 0.0);
	gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(priv->progress_widget), 1.0);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR(priv->progress_widget), "0 %");
	gtk_box_pack_start (GTK_BOX(vbox), priv->progress_widget, FALSE, FALSE, 0);

	priv->text_widget = gtk_label_new("");
	gtk_misc_set_alignment (GTK_MISC(priv->text_widget), 0.00F, 0.00F);
	gtk_label_set_line_wrap (GTK_LABEL(priv->text_widget), TRUE);

	gtk_box_pack_start (GTK_BOX(vbox), priv->text_widget, TRUE, TRUE, 0);

	GtkBox* pDialogVBox;
#if GTK_API_VERSION == 3
	pDialogVBox = GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG (dialog)));
#else
	pDialogVBox = GTK_BOX(GTK_DIALOG (dialog)->vbox);
#endif

	gtk_box_pack_start (pDialogVBox, vbox, FALSE, FALSE, 0);
	gtk_widget_show_all(vbox);

	// Initialize signals for dialogprogress
	gtk_dialog_add_buttons (GTK_DIALOG(dialog),
	    "gtk-close", GTK_RESPONSE_CLOSE, NULL);

	g_signal_connect(G_OBJECT(dialog), "response",
	    G_CALLBACK(on_dialog_response), NULL);

	gtk_window_set_deletable (GTK_WINDOW(dialog), FALSE);

	gtk_progress_dialog_set_button_close_enabled(dialog, FALSE);

	while (gtk_events_pending()) {
	    gtk_main_iteration();
	}
	
	return dialog;
}

void
gtk_progress_dialog_set_title(GtkProgressDialog* dialog, gchar *title)
{
	g_return_if_fail(dialog!=NULL);
	g_return_if_fail(GTK_IS_PROGRESS_DIALOG(dialog));
	
	GtkProgressDialogPrivate* priv;
	priv = GTK_PROGRESS_DIALOG_PRIVATE(dialog);

	gchar *text;
	
	gtk_window_set_title(GTK_WINDOW(dialog), title);

	text = g_strdup_printf("<span size=\"large\"><b>%s</b></span>", title);
	gtk_label_set_markup(GTK_LABEL(priv->title_widget), text);
	g_free(text);
	
	while (gtk_events_pending()) {
	    gtk_main_iteration();
	}
}

void
gtk_progress_dialog_set_text(GtkProgressDialog* dialog, gchar *text)
{
	g_return_if_fail(dialog!=NULL);
	g_return_if_fail(GTK_IS_PROGRESS_DIALOG(dialog));
	
	GtkProgressDialogPrivate* priv;
	priv = GTK_PROGRESS_DIALOG_PRIVATE(dialog);

	gtk_label_set_markup(GTK_LABEL(priv->text_widget), text);
	
	while (gtk_events_pending()) {
	    gtk_main_iteration();
	}
}

void
gtk_progress_dialog_set_percent(GtkProgressDialog* dialog, gdouble percent)
{
	g_return_if_fail(dialog!=NULL);
	g_return_if_fail(GTK_IS_PROGRESS_DIALOG(dialog));

	GtkProgressDialogPrivate* priv;
	priv = GTK_PROGRESS_DIALOG_PRIVATE(dialog);

	gchar* text;
	
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(priv->progress_widget), percent);
				
	text = g_strdup_printf("%0.0f %%", percent * 100);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR(priv->progress_widget), text);
	g_free(text);
	
	while (gtk_events_pending()) {
	    gtk_main_iteration();
	}
}

void
gtk_progress_dialog_pulse(GtkProgressDialog* dialog)
{
	g_return_if_fail(dialog!=NULL);
	g_return_if_fail(GTK_IS_PROGRESS_DIALOG(dialog));

	GtkProgressDialogPrivate* priv;
	priv = GTK_PROGRESS_DIALOG_PRIVATE(dialog);
	
	gtk_progress_bar_pulse (GTK_PROGRESS_BAR(priv->progress_widget));
	
	while (gtk_events_pending()) {
	    gtk_main_iteration();
	}
}

void
gtk_progress_dialog_set_button_close_enabled(GtkProgressDialog* dialog, gboolean enabled)
{
	g_return_if_fail(dialog!=NULL);
	g_return_if_fail(GTK_IS_PROGRESS_DIALOG(dialog));

	GtkWidget* widget;
	widget = gtk_dialog_get_widget_for_response  (GTK_DIALOG (dialog), GTK_RESPONSE_CLOSE);
	gtk_widget_set_sensitive (widget, enabled);
	
	while (gtk_events_pending()) {
	    gtk_main_iteration();
	}
}

static gboolean
do_idle_destroy_window (gpointer user_data)
{
	GtkWidget* pWidget;

	pWidget = (GtkWidget*)user_data;
	gtk_widget_destroy (GTK_WIDGET(pWidget));

	return FALSE;
}

static void 
on_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	// We do the destruction after the button clicked event is finished
	g_idle_add (do_idle_destroy_window, (gpointer)dialog);
}