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

#include <sqlite3.h>

#include "freetuxtv-channels-group.h"
#include "freetuxtv-config.h"
#include "freetuxtv-channel.h"

G_DEFINE_TYPE (FreetuxTVChannelsGroup, freetuxtv_channels_group, GTK_TYPE_VBOX);

static int 
freetuxtv_channels_group_update_from_db_callback(void *data, int argc, char **argv, char **colsname);

static void
freetuxtv_channels_group_arrow_onclick (GtkWidget *widget, gpointer data);

GtkWidget *
freetuxtv_channels_group_new (gchar *id, gchar *name)
{
	FreetuxTVChannelsGroup *channels_group = NULL;
	
	channels_group = gtk_type_new (freetuxtv_channels_group_get_type ());
	gtk_box_set_homogeneous(GTK_BOX(channels_group),FALSE);
	gtk_box_set_spacing(GTK_BOX(channels_group),0);
	
	channels_group->id = g_strdup(id);
	channels_group->name = g_strdup(name);

	GdkColor color;
	color.pixel = 0;
	color.red   = 0xcf * 0x100;
        color.green = 0xe0 * 0x100;
        color.blue  = 0xf5 * 0x100;

	GtkWidget *eventbox = gtk_event_box_new ();
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	gtk_box_pack_start (GTK_BOX(channels_group), eventbox, FALSE, FALSE, 0);

	GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER(eventbox), hbox);
	
	/* Fleche pour l'expand */
	eventbox = gtk_event_box_new ();
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	g_signal_connect(G_OBJECT(eventbox),
			 "button-press-event",
			 G_CALLBACK(freetuxtv_channels_group_arrow_onclick),
			 NULL);
	gtk_box_pack_start (GTK_BOX(hbox), eventbox, FALSE, FALSE, 0);	
	GtkWidget *arrow = gtk_arrow_new (GTK_ARROW_DOWN, GTK_SHADOW_IN);
	channels_group->arrow = arrow;
	gtk_container_add (GTK_CONTAINER(eventbox), arrow);

	/* Nom du groupe */
	GtkWidget *label = gtk_label_new (channels_group->name);
	gtk_misc_set_padding (GTK_MISC(label),3,3);
	gtk_box_pack_start (GTK_BOX(hbox), label, FALSE, FALSE, 0);
	
	channels_group->channels_widget = gtk_vbox_new(FALSE, 1);
	gtk_box_pack_start (GTK_BOX(channels_group), channels_group->channels_widget, FALSE, FALSE, 0);

	freetuxtv_channels_group_update_from_db (channels_group);
	
	gtk_widget_show_all (GTK_WIDGET(channels_group));
	
	return GTK_WIDGET(channels_group);
}

void
freetuxtv_channels_group_add_channel (FreetuxTVChannelsGroup *self,
				      FreetuxTVChannel *channel)
{
	gtk_box_pack_start (GTK_BOX(self->channels_widget),
			    GTK_WIDGET(channel),
			    FALSE, FALSE, 0);
}

void
freetuxtv_channels_group_change_collasped (FreetuxTVChannelsGroup *self)
{
	if(self->collapsed == 'N') {
		freetuxtv_channels_group_set_collasped (self, 'Y');
	}else{
		freetuxtv_channels_group_set_collasped (self, 'N');	
	}	
}

void
freetuxtv_channels_group_set_collasped (FreetuxTVChannelsGroup *self,
					gchar mode)
{
	self->collapsed = mode;
	if(self->collapsed == 'N') {
		gtk_arrow_set (GTK_ARROW(self->arrow), GTK_ARROW_DOWN, GTK_SHADOW_NONE);
		gtk_widget_show_all (GTK_WIDGET(self->channels_widget));
	}else{
		gtk_arrow_set (GTK_ARROW(self->arrow), GTK_ARROW_RIGHT, GTK_SHADOW_NONE);
		gtk_widget_hide_all (GTK_WIDGET(self->channels_widget));	
	}
}

FreetuxTVChannelsGroup *
freetuxtv_channel_get_channels_group (GtkWidget *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WIDGET(self), NULL);

	if(FREETUXTV_IS_CHANNELS_GROUP(self)){
		return FREETUXTV_CHANNELS_GROUP(self);
	}else{
		return freetuxtv_channel_get_channels_group (gtk_widget_get_parent(self));
	}
}

int
freetuxtv_channels_group_update_from_db (FreetuxTVChannelsGroup *self)
{
	struct sqlite3 *db;
	int res;
	char *err=0;
	
	/* Ouverture de la BDD */
	res = sqlite3_open(FREETUXTV_SQLITE_DB,&db);
	if(res != SQLITE_OK){
		g_printerr("Sqlite3 : %s\n",
			   sqlite3_errmsg(db));
		g_printerr("FreetuxTV : Cannot open database %s\n",
			   FREETUXTV_SQLITE_DB);
		sqlite3_close(db);
		return -1;
	}
	
	/* Selection des channels */
	res=sqlite3_exec(db,
			 g_strconcat("SELECT id_channel, name_channel, logo_channel, uri_channel FROM channel WHERE channelsgroup_channel=",self->id,NULL),
			 freetuxtv_channels_group_update_from_db_callback,
			 (void *)self, &err);
	if(res != SQLITE_OK){
		g_printerr("Sqlite3 : %s\n",
			   sqlite3_errmsg(db));
		g_printerr("FreetuxTV : Cannot read channels for group %s in %s\n",
			   self->name,FREETUXTV_SQLITE_DB);
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
	
	return 0;
}

static void
freetuxtv_channels_group_arrow_onclick (GtkWidget *widget, gpointer data)
{
	FreetuxTVChannelsGroup *self;
	self = freetuxtv_channel_get_channels_group (GTK_WIDGET(widget));
	freetuxtv_channels_group_change_collasped (self);
}

static int 
freetuxtv_channels_group_update_from_db_callback(void *data, int argc, char **argv, char **colsname)
{
	FreetuxTVChannelsGroup *self = (FreetuxTVChannelsGroup *) data;
	FreetuxTVChannel *channel;
	channel = freetuxtv_channel_new(argv[1], argv[3]);
	freetuxtv_channel_set_logo (channel, g_strconcat("./img/channels/",
							 argv[2],NULL));
 
	freetuxtv_channels_group_add_channel (self, channel);
	return 0;
}

static void
freetuxtv_channels_group_init (FreetuxTVChannelsGroup *object)
{
	object->id=0;
	object->name="";
	object->channels_widget = NULL;

	object->collapsed = 'N';
	object->arrow = NULL;
}

static void
freetuxtv_channels_group_finalize (GObject *object)
{
	
}

static void
freetuxtv_channels_group_class_init (FreetuxTVChannelsGroupClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_channels_group_finalize;
}
