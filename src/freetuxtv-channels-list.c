/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <sqlite3.h>

#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group.h"

G_DEFINE_TYPE (FreetuxTVChannelsList, freetuxtv_channels_list, GTK_TYPE_EVENT_BOX);

extern GladeXML *gladexml;

static int 
on_exec_add_channels_group (void *data, int argc, char **argv, char **colsname);

GtkWidget *
freetuxtv_channels_list_new ()
{
	FreetuxTVChannelsList *self = NULL;
	self = gtk_type_new (freetuxtv_channels_list_get_type ());
	
	/* Widget contenant les groupes de chaines */
	GdkColor color;
	color.pixel = 0;
	color.red   = 0xdf * 0x100;
	color.green = 0xe0 * 0x100;
	color.blue  = 0xe6 * 0x100;
	gtk_widget_modify_bg(GTK_WIDGET(self), GTK_STATE_NORMAL, &color);
	
	self->channelsgroups_widget = NULL;
	
	freetuxtv_channels_list_update_from_db (self);
	
	gtk_widget_show_all (GTK_WIDGET(self));
	
	return GTK_WIDGET (self);
}

GtkWidget *
freetuxtv_channels_list_get_from_gladexml ()
{
	GtkWidget *self;
	
	self = glade_xml_get_widget (gladexml,
				     "windowmain_scrolledwindowchannels");
	self = gtk_bin_get_child (GTK_BIN(self));
	self = gtk_bin_get_child (GTK_BIN(self));
	return self;
}

void
freetuxtv_channels_list_add_channels_group (FreetuxTVChannelsList *self,
					    FreetuxTVChannelsGroup *channels_group)
{
	gtk_box_pack_start (GTK_BOX(self->channelsgroups_widget), 
			    GTK_WIDGET(channels_group), FALSE, FALSE, 0);
}

int
freetuxtv_channels_list_update_from_db (FreetuxTVChannelsList *self)
{
	struct sqlite3 *db;
	int res;
	char *err=0;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	if(self->channelsgroups_widget != NULL){
		gtk_widget_destroy(self->channelsgroups_widget);
		self->channelsgroups_widget = NULL;
	}
	self->channelsgroups_widget = gtk_vbox_new(FALSE,0);
	gtk_container_add (GTK_CONTAINER(self), 
			   self->channelsgroups_widget);
	
	/* Ouverture de la BDD */
	res = sqlite3_open(user_db,&db);
	if(res != SQLITE_OK){
		g_printerr("Sqlite3 : %s\n",
			   sqlite3_errmsg(db));
		g_printerr("FreetuxTV : Cannot open database %s\n",
			   user_db);
		sqlite3_close(db);
		return -1;
	}
	
	/* Selection des channels */
	gchar *query;
	query = "SELECT id_channelsgroup, name_channelsgroup, \
                 uri_channelsgroup			      \
                 FROM channels_group";
	res=sqlite3_exec(db, query, on_exec_add_channels_group,
			 (void *)self, &err);
	if(res != SQLITE_OK){
		g_printerr("Sqlite3 : %s\n",
			   sqlite3_errmsg(db));
		g_printerr("FreetuxTV : Cannot read channels' group in %s\n",
			   user_db);
	}
	
	sqlite3_free(err);
	sqlite3_close(db);

	g_free(user_db);
	
	gtk_widget_show_all (GTK_WIDGET(self));

	return 0;
}

void
freetuxtv_channels_list_apply_filter (FreetuxTVChannelsList *self)
{
	int count = 0;
	GList* tmp;

	GtkWidget *entryfilter;
	gchar *filter;
	entryfilter = glade_xml_get_widget (gladexml, "windowmain_entryfilter");
	filter = (gchar *)gtk_entry_get_text (GTK_ENTRY(entryfilter));

	tmp = g_list_first (gtk_container_get_children (GTK_CONTAINER(self->channelsgroups_widget)));
	while (tmp != NULL){
		FreetuxTVChannelsGroup *channels_group;
		channels_group = FREETUXTV_CHANNELS_GROUP (tmp->data);
		count += freetuxtv_channels_group_apply_filter (channels_group, 
								filter);
		tmp = g_list_next (tmp); 
	}
}

static int 
on_exec_add_channels_group (void *data, int argc, char **argv, char **colsname)
{
	FreetuxTVChannelsList *self = (FreetuxTVChannelsList *) data;
	FreetuxTVChannelsGroup *channels_group;
	channels_group=FREETUXTV_CHANNELS_GROUP (freetuxtv_channels_group_new (argv[0], argv[1], argv[2]));
	freetuxtv_channels_list_add_channels_group (self, channels_group);
	return 0;
}

static void
freetuxtv_channels_list_init (FreetuxTVChannelsList *object)
{
	object->channelsgroups_widget = NULL;
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
