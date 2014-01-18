/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2011 <eric.beuque@gmail.com>
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

#include <string.h>
#include "freetuxtv-player-error-dialog.h"


typedef struct _FreetuxTVPlayerErrorDialogPrivate FreetuxTVPlayerErrorDialogPrivate;
struct _FreetuxTVPlayerErrorDialogPrivate
{
	FreetuxTVApp* app;
	GtkTextBuffer* pTextBuffer;
};

#define FREETUXTV_PLAYER_ERROR_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_PLAYER_ERROR_DIALOG, FreetuxTVPlayerErrorDialogPrivate))

G_DEFINE_TYPE (FreetuxTVPlayerErrorDialog, freetuxtv_player_error_dialog, GTK_TYPE_DIALOG);

static void
on_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	if(response_id == GTK_RESPONSE_CLOSE){
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
freetuxtv_player_error_dialog_init (FreetuxTVPlayerErrorDialog *object)
{
	FreetuxTVPlayerErrorDialogPrivate *priv;
	priv = FREETUXTV_PLAYER_ERROR_PRIVATE (object);

	priv->app = NULL;

	priv->pTextBuffer = gtk_text_buffer_new (NULL);

	GtkWidget* pTextView;
	pTextView = gtk_text_view_new_with_buffer (priv->pTextBuffer);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(pTextView), GTK_WRAP_WORD);

	GtkWidget* pScroll;
	pScroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(pScroll), pTextView);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(pScroll), GTK_SHADOW_IN);
	//gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(pScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	GtkWidget* vbox;
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start (GTK_BOX (vbox),
	    GTK_WIDGET(pScroll), TRUE, TRUE, 5);

	GtkWidget* area;
	area = gtk_dialog_get_content_area (GTK_DIALOG(object));
	gtk_box_pack_start (GTK_BOX (area),
	    GTK_WIDGET(vbox), TRUE, TRUE, 5);

	gtk_widget_show_all(vbox);
	
	gtk_dialog_add_button (GTK_DIALOG(object), "gtk-close", GTK_RESPONSE_CLOSE);
	gtk_window_set_default_size (GTK_WINDOW(object), 400, 200);

	g_signal_connect(G_OBJECT(object), "response",
	    G_CALLBACK(on_response), NULL);
}

static void
freetuxtv_player_error_dialog_finalize (GObject *object)
{
	G_OBJECT_CLASS (freetuxtv_player_error_dialog_parent_class)->finalize (object);
}

static void
freetuxtv_player_error_dialog_class_init (FreetuxTVPlayerErrorDialogClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	
	g_type_class_add_private (klass, sizeof (FreetuxTVPlayerErrorDialogPrivate));

	object_class->finalize = freetuxtv_player_error_dialog_finalize;
}

GtkWidget*
freetuxtv_player_error_dialog_new (
    GtkWindow *parent, GtkDialogFlags flags, FreetuxTVApp* app)
{
	GtkWidget *widget;
	GtkDialog *dialog;

	g_return_val_if_fail (parent == NULL || GTK_IS_WINDOW (parent), NULL);

	widget = g_object_new (FREETUXTV_TYPE_PLAYER_ERROR_DIALOG, NULL);
	dialog = GTK_DIALOG (widget);

	if (parent != NULL)
		gtk_window_set_transient_for (GTK_WINDOW (widget),
		    GTK_WINDOW (parent));

	if (flags & GTK_DIALOG_MODAL)
		gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

	if (flags & GTK_DIALOG_DESTROY_WITH_PARENT)
		gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);

	/*
	if (flags & GTK_DIALOG_NO_SEPARATOR)
		gtk_dialog_set_has_separator (dialog, FALSE);*/

	

	return widget;
}

void
freetuxtv_player_error_dialog_set_message (
    FreetuxTVPlayerErrorDialog* pPlayerErrorDialog, const gchar *message)
{
	FreetuxTVPlayerErrorDialogPrivate *priv;
	priv = FREETUXTV_PLAYER_ERROR_PRIVATE (pPlayerErrorDialog);

	if (message)
	{
		gtk_text_buffer_set_text (priv->pTextBuffer, message, strlen(message));
	}
}