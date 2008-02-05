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

#include <malloc.h>
#include <gtk/gtk.h>

#include "freetuxtv-channels-list.h"

static void
freetuxtv_channels_list_onclicked (GtkWidget *widget, gpointer *data);

FreetuxTVChannelsList *
freetuxtv_channels_list_new ()
{
	FreetuxTVChannelsList * self;
	self = (FreetuxTVChannelsList *)malloc(sizeof(FreetuxTVChannelsList));
	
	self->count=0;

	/* Creation du widget */
	self->widget = gtk_vbox_new(FALSE, 0);
	
	/* Création du champs de recherche */
	GtkWidget *hbox;
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX(self->widget), hbox, FALSE, FALSE, 10);
	
	GtkWidget *button;
	button = gtk_button_new_with_label ("X");
	gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 3);
	g_signal_connect(G_OBJECT(button),
			 "clicked",
			 G_CALLBACK(freetuxtv_channels_list_onclicked),
			 NULL);
	
	GtkWidget *entry;
	entry = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX(hbox), entry, FALSE, FALSE, 3);

	GtkWidget *scrollbar;
	scrollbar = gtk_scrolled_window_new(NULL, NULL);
	self->channels_widget = gtk_vbox_new(FALSE,0);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar),
					      self->channels_widget);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar),
				       GTK_POLICY_NEVER,
				       GTK_POLICY_ALWAYS);
	gtk_box_pack_start (GTK_BOX(self->widget), scrollbar, TRUE, TRUE, 10);
	
	return self;
}

void
freetuxtv_channels_list_add_channel (FreetuxTVChannelsList *self,
				     FreetuxTVChannel *channel)
{
	self->count++;
	if(self->count==1){
		self->channels = (FreetuxTVChannel **)malloc(sizeof(FreetuxTVChannel *));	
	}else{
		self->channels = (FreetuxTVChannel **) realloc (self->channels,
								self->count * sizeof(FreetuxTVChannel *));	
	}
	self->channels[self->count-1] = channel;
	
	gtk_box_pack_start (GTK_BOX(self->channels_widget), channel->widget, FALSE, FALSE, 0);
	gtk_widget_show_all(channel->widget);
}

static void
freetuxtv_channels_list_onclicked (GtkWidget *widget, gpointer *data)
{
	gtk_main_quit();
}
