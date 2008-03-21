/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
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

#include <sqlite3.h>

#include "freetuxtv-channels-group.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channel.h"

G_DEFINE_TYPE (FreetuxTVChannelsGroup, freetuxtv_channels_group, GTK_TYPE_VBOX);

static int 
freetuxtv_channels_group_db_exec_add_channel (void *data, int argc, 
											  char **argv, char **colsname);

static void
freetuxtv_channels_group_arrow_event_button (GtkWidget *widget, 
											 GdkEventButton *event, 
											 gpointer func_data);

GtkWidget *
freetuxtv_channels_group_new (gchar *id, gchar *name)
{
	FreetuxTVChannelsGroup *self = NULL;
	
	self = gtk_type_new (freetuxtv_channels_group_get_type ());
	gtk_box_set_homogeneous(GTK_BOX(self),FALSE);
	gtk_box_set_spacing(GTK_BOX(self),0);
	
	self->id = g_strdup(id);
	self->name = g_strdup(name);

	GdkColor color;
	color.pixel = 0;
	color.red   = 0xcf * 0x100;
	color.green = 0xe0 * 0x100;
	color.blue  = 0xf5 * 0x100;

	GtkWidget *eventbox = gtk_event_box_new ();
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	gtk_box_pack_start (GTK_BOX(self), eventbox, FALSE, FALSE, 0);

	GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER(eventbox), hbox);
	
	/* Fleche pour l'expand */
	eventbox = gtk_event_box_new ();
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	g_signal_connect(G_OBJECT(eventbox),
					 "button-press-event",
					 G_CALLBACK(freetuxtv_channels_group_arrow_event_button),
					 NULL);
	gtk_box_pack_start (GTK_BOX(hbox), eventbox, FALSE, FALSE, 0);	
	GtkWidget *arrow = gtk_arrow_new (GTK_ARROW_DOWN, GTK_SHADOW_IN);
	self->arrow = arrow;
	gtk_container_add (GTK_CONTAINER(eventbox), arrow);

	/* Nom du groupe */
	GtkWidget *label = gtk_label_new (self->name);
	gtk_misc_set_padding (GTK_MISC(label),3,3);
	gtk_box_pack_start (GTK_BOX(hbox), label, FALSE, FALSE, 0);
	
	self->channels_widget = gtk_vbox_new(FALSE, 1);
	gtk_box_pack_start (GTK_BOX(self), 
						self->channels_widget, FALSE, FALSE, 0);

	freetuxtv_channels_group_update_from_db (self);
	
	gtk_widget_show_all (GTK_WIDGET(self));
	
	return GTK_WIDGET(self);
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
		gtk_arrow_set (GTK_ARROW(self->arrow), 
					   GTK_ARROW_DOWN,
					   GTK_SHADOW_NONE);
		FreetuxTVChannelsList *channels_list;
		channels_list = freetuxtv_channels_list_get_from_widget (GTK_WIDGET(self));
		gchar *filter;
		filter = (gchar *) gtk_entry_get_text ( GTK_ENTRY (channels_list->filter_widget));
		freetuxtv_channels_group_apply_filter (self, filter);
	}else{
		gtk_arrow_set (GTK_ARROW(self->arrow), 
					   GTK_ARROW_RIGHT,
					   GTK_SHADOW_NONE);
		gtk_widget_hide_all (GTK_WIDGET(self->channels_widget));	
	}
}

int
freetuxtv_channels_group_apply_filter (FreetuxTVChannelsGroup *self, gchar *filter)
{
	int count = 0;
	GList* tmp;
	tmp = g_list_first (gtk_container_get_children (GTK_CONTAINER(self->channels_widget)));
	gtk_widget_show_all (GTK_WIDGET(self));
	while (tmp != NULL){
		FreetuxTVChannel *channel;
		channel = FREETUXTV_CHANNEL (tmp->data);
		count += freetuxtv_channel_apply_filter (channel, filter);
		tmp = g_list_next (tmp); 
	}
	if( count == 0 ){
		gtk_widget_hide_all (GTK_WIDGET(self));
	}
	return count;
}

FreetuxTVChannelsGroup *
freetuxtv_channels_group_get_from_widget (GtkWidget *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WIDGET(self), NULL);

	if(FREETUXTV_IS_CHANNELS_GROUP(self)){
		return FREETUXTV_CHANNELS_GROUP(self);
	}else{
		return freetuxtv_channels_group_get_from_widget (gtk_widget_get_parent(self));
	}
}

int
freetuxtv_channels_group_update_from_db (FreetuxTVChannelsGroup *self)
{
	struct sqlite3 *db;
	int res;
	char *err=0;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
						  "/FreetuxTV/freetuxtv.db", NULL);
	
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
	query = g_strconcat("SELECT id_channel, name_channel, filename_channellogo, \
                         uri_channel FROM channel LEFT JOIN channel_logo \
                         ON  id_channellogo=idchannellogo_channel	\
                         WHERE channelsgroup_channel=",
						self->id, NULL);
	res = sqlite3_exec(db, query,
					   freetuxtv_channels_group_db_exec_add_channel,
					   (void *)self, &err);
	g_free(query);
	if(res != SQLITE_OK){
		g_printerr("Sqlite3 : %s\n",
				   sqlite3_errmsg(db));
		g_printerr("FreetuxTV : Cannot read channels for group %s in %s\n",
				   self->name,user_db);
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);

	return 0;
}

static void
freetuxtv_channels_group_arrow_event_button (GtkWidget *widget,
											 GdkEventButton *event,
											 gpointer func_data)
{
	FreetuxTVChannelsGroup *self;
	self = freetuxtv_channels_group_get_from_widget (GTK_WIDGET(widget));
	freetuxtv_channels_group_change_collasped (self);
}

static int 
freetuxtv_channels_group_db_exec_add_channel (void *data, int argc,
											  char **argv, char **colsname)
{
	FreetuxTVChannelsGroup *self = (FreetuxTVChannelsGroup *) data;
	FreetuxTVChannel *channel;
	channel = FREETUXTV_CHANNEL(freetuxtv_channel_new(argv[1],
													  argv[3]));
	
	freetuxtv_channel_set_logo (channel, (gchar *)argv[2]);
	
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
