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

G_DEFINE_TYPE (FreetuxTVChannel, freetuxtv_channel, GTK_TYPE_BUTTON);

static void
freetuxtv_channel_onclicked (GtkWidget *widget, gpointer data);

GtkWidget *
freetuxtv_channel_new (gchar *name, gchar *uri)
{
	FreetuxTVChannel * channel = NULL;
	channel = gtk_type_new (freetuxtv_channel_get_type ());

	channel->name=name;
	channel->uri=uri;

	/* Creation du widget */
	g_signal_connect(G_OBJECT(channel),
			 "clicked",
			 G_CALLBACK(freetuxtv_channel_onclicked),
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
	
	channel->logo = gtk_image_new ();
	gtk_box_pack_start (GTK_BOX(hbox), channel->logo, FALSE, FALSE, 0);
	GtkWidget *label;
	label = gtk_label_new (channel->name);
	gtk_box_pack_start (GTK_BOX(hbox), label, TRUE, TRUE, 0);
	
	return GTK_WIDGET(channel);
}

void
freetuxtv_channel_set_logo (FreetuxTVChannel *self, gchar *file)
{
	gtk_image_set_from_file (GTK_IMAGE(self->logo), file);
	gtk_widget_show(self->logo);
}

static void
freetuxtv_channel_onclicked (GtkWidget *widget, gpointer data)
{
	FreetuxTVChannel *self = FREETUXTV_CHANNEL(widget);
	g_print ("Launching : %s\n", self->name);
	freetuxtv_channel_play(self);
}

void
freetuxtv_channel_set_player (FreetuxTVChannel *self, FreetuxTVPlayer *player){
	self->player = player;	
}

void
freetuxtv_channel_play (FreetuxTVChannel *self)
{
	if(self->player!=NULL){
		freetuxtv_player_play(self->player, self->uri);
	}
}

static void
freetuxtv_channel_init (FreetuxTVChannel *object)
{
	object->name="";
	object->uri="";
	object->logo = NULL;
	object->player = NULL;
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
