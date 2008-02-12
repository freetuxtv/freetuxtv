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

#include "freetuxtv-channels-list.h"
#include "freetuxtv-config.h"

G_DEFINE_TYPE (FreetuxTVChannelsList, freetuxtv_channels_list, GTK_TYPE_VBOX);

static void
freetuxtv_channels_list_onclicked (GtkWidget *widget, gpointer *data);

static int 
freetuxtv_channels_list_set_from_db_callback(void *data, int argc, char **argv, char **colsname);

GtkWidget *
freetuxtv_channels_list_new ()
{
	FreetuxTVChannelsList * channels_list = NULL;
	channels_list = gtk_type_new (freetuxtv_channels_list_get_type ());

	/* Creation du widget */
	gtk_box_set_homogeneous(GTK_BOX(channels_list),FALSE);
	gtk_box_set_spacing(GTK_BOX(channels_list),0);

	/* Création du champs de recherche */
	GtkWidget *hbox;
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX(channels_list), hbox, FALSE, FALSE, 10);
	
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
	channels_list->channels_widget = gtk_vbox_new(FALSE,0);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar),
					      channels_list->channels_widget);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar),
				       GTK_POLICY_NEVER,
				       GTK_POLICY_ALWAYS);
	gtk_box_pack_start (GTK_BOX(channels_list), scrollbar, TRUE, TRUE, 10);
	
	freetuxtv_channels_list_set_from_db (channels_list);

	gtk_widget_show_all (GTK_WIDGET(channels_list));

	return GTK_WIDGET (channels_list);
}

void
freetuxtv_channels_list_add_channel (FreetuxTVChannelsList *self,
				     FreetuxTVChannel *channel)
{
	gtk_box_pack_start (GTK_BOX(self->channels_widget), GTK_WIDGET(channel), FALSE, FALSE, 0);
	gtk_widget_show_all(GTK_WIDGET(channel));
}

int
freetuxtv_channels_list_set_from_db (FreetuxTVChannelsList *self)
{
	struct sqlite3 *db;
	int res;
	char *err=0;

	/* Ouverture de la BDD */
	res = sqlite3_open(FREETUXTV_SQLITE_DB,&db);
	if(res != SQLITE_OK){
		fprintf(stderr,
			"Sqlite3 : %s\n",
			sqlite3_errmsg(db));
		fprintf(stderr,
			"FreetuxTV : Cannot open database %s\n",
			FREETUXTV_SQLITE_DB);
		sqlite3_close(db);
		return -1;
	}
	
	/* Selection des channels */
	res=sqlite3_exec(db,
			 "SELECT name_channel, uri_channel FROM channel",
			 freetuxtv_channels_list_set_from_db_callback,
			 (void *)self, &err);
	if(res != SQLITE_OK){
		fprintf(stderr,
			"Sqlite3 : %s\n",
			sqlite3_errmsg(db));
		fprintf(stderr,
			"FreetuxTV : Cannot create table 'channel' %s\n",
			FREETUXTV_SQLITE_DB);
	}

	sqlite3_free(err);
	sqlite3_close(db);

	return 0;
}

static void
freetuxtv_channels_list_onclicked (GtkWidget *widget, gpointer *data)
{
	gtk_main_quit();
}

static int 
freetuxtv_channels_list_set_from_db_callback(void *data, int argc, char **argv, char **colsname)
{
	FreetuxTVChannelsList *self = (FreetuxTVChannelsList *) data;
	FreetuxTVChannel *channel;
	
	channel=freetuxtv_channel_new(argv[0], argv[1]);
	freetuxtv_channel_set_logo (channel, g_strconcat("./img/",argv[0],".jpg",NULL));
	freetuxtv_channels_list_add_channel (self, channel);
	return 0;
}

static void
freetuxtv_channels_list_init (FreetuxTVChannelsList *object)
{
	object->channels_widget = NULL;
}

static void
freetuxtv_channels_list_finalize (GObject *object)
{
	
}

static void
freetuxtv_channels_list_class_init (FreetuxTVChannelsListClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_channels_list_finalize;
}
