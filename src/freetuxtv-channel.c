/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with brasero.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include "freetuxtv-channel.h"
#include "freetuxtv-main-window.h"
#include "freetuxtv-player.h"

G_DEFINE_TYPE (FreetuxTVChannel, freetuxtv_channel, GTK_TYPE_EVENT_BOX);

static void
freetuxtv_channel_onclick (GtkWidget *widget, gpointer data);

GtkWidget *
freetuxtv_channel_new (gchar *name, gchar *uri)
{
	FreetuxTVChannel * channel = NULL;
	channel = gtk_type_new (freetuxtv_channel_get_type ());

	channel->name = g_strdup(name);
	channel->uri = g_strdup(uri);

	/* Creation du widget */
	g_signal_connect(G_OBJECT(channel),
			 "button-press-event",
			 G_CALLBACK(freetuxtv_channel_onclick),
			 NULL);
	
	GdkColor color;
	color.pixel = 0;
	color.red   = 0xff * 0x100;
        color.green = 0xff * 0x100;
        color.blue  = 0xff * 0x100;
	gtk_widget_modify_bg(GTK_WIDGET(channel), GTK_STATE_NORMAL, &color);

	GtkWidget *hbox;
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(channel), hbox);
	
	/* Ajout du logo */
	channel->logo = gtk_image_new ();
	gtk_misc_set_padding (GTK_MISC(channel->logo),2,2);
	gtk_box_pack_start (GTK_BOX(hbox), channel->logo, FALSE, FALSE, 0);
	
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), vbox, TRUE, TRUE, 0);

	/* Ajout du nom du canal */
	GtkWidget *label;
	label = gtk_label_new (g_strconcat("<small>",channel->name,"</small>",NULL));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment (GTK_MISC(label),0,0.5);
	gtk_box_pack_start (GTK_BOX(vbox), label, TRUE, TRUE, 0);
	
	
	/* Barre de progression */
	GtkWidget *progressbar;
	progressbar = gtk_progress_bar_new ();
	//gtk_progress_bar_set_text (GTK_PROGRESS_BAR(progressbar), "12:00 - 14:00");
	gtk_progress_set_text_alignment (GTK_PROGRESS(progressbar), 0, 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(progressbar), 0.50);
	gtk_box_pack_start (GTK_BOX(vbox), progressbar, FALSE, FALSE, 0);

	/* Ajout du titre du programme */
	label = gtk_label_new ("<small>Programme TV indisponible</small>");
	gtk_label_set_use_markup (GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment (GTK_MISC(label),0,0.5);
	gtk_label_set_ellipsize (GTK_LABEL(label), PANGO_ELLIPSIZE_END);
	gtk_box_pack_start (GTK_BOX(vbox), label, TRUE, TRUE, 0);

	gtk_widget_show_all (GTK_WIDGET(channel));

	return GTK_WIDGET(channel);
}

void
freetuxtv_channel_set_logo (FreetuxTVChannel *self, gchar *file)
{
	gtk_image_set_from_file (GTK_IMAGE(self->logo), file);
	gtk_widget_show(self->logo);
}

static void
freetuxtv_channel_onclick (GtkWidget *widget, gpointer data)
{
	FreetuxTVChannel *self = FREETUXTV_CHANNEL(widget);
	g_print ("FreetuxTV : launching channel \"%s\"\n", self->name);
	freetuxtv_channel_play(self);
}

void
freetuxtv_channel_play (FreetuxTVChannel *self)
{
	FreetuxTVMainWindow *main_window;
	main_window = freetuxtv_channel_get_main_window(GTK_WIDGET(self));
	freetuxtv_player_play(main_window->player, self->uri);
}

static void
freetuxtv_channel_init (FreetuxTVChannel *object)
{
	object->name="";
	object->uri="";
	object->logo = NULL;
}

static void
freetuxtv_channel_finalize (GObject *object)
{
	
}

static void
freetuxtv_channel_class_init (FreetuxTVChannelClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_channel_finalize;
}
