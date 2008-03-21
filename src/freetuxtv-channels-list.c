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

#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group.h"

G_DEFINE_TYPE (FreetuxTVChannelsList, freetuxtv_channels_list, GTK_TYPE_VBOX);

static void
freetuxtv_channels_list_event_button (GtkWidget *widget,
									  GdkEventButton *event,
									  gpointer func_data);

static void
freetuxtv_channels_list_event_entry_changed (GtkWidget *widget, 
											 gpointer func_data);

static int 
freetuxtv_channels_list_db_exec_add_channels_group (void *data, int argc, 
													char **argv, char **colsname);

GtkWidget *
freetuxtv_channels_list_new ()
{
	FreetuxTVChannelsList *self = NULL;
	self = gtk_type_new (freetuxtv_channels_list_get_type ());

	/* Creation du widget */
	gtk_box_set_homogeneous(GTK_BOX(self),FALSE);
	gtk_box_set_spacing(GTK_BOX(self),0);

	/* Création du champs de recherche */
	GtkWidget *hbox;
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX(self), hbox, FALSE, FALSE, 10);
	
	GtkWidget *button;
	button = gtk_button_new_with_label ("X");
	gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 3);
	g_signal_connect(G_OBJECT(button),
					 "clicked",
					 G_CALLBACK(freetuxtv_channels_list_event_button),
					 NULL);
	
	self->filter_widget = gtk_entry_new ();
	g_signal_connect(G_OBJECT(self->filter_widget),
					 "changed",
					 G_CALLBACK(freetuxtv_channels_list_event_entry_changed),
					 NULL);
	gtk_box_pack_start (GTK_BOX(hbox), 
						self->filter_widget, FALSE, FALSE, 3);
	
	/* Widget contenant les groupes de chaines */
	GtkWidget *scrollbar;
	scrollbar = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar),
								   GTK_POLICY_NEVER,
								   GTK_POLICY_ALWAYS);
	gtk_box_pack_start (GTK_BOX(self), scrollbar, TRUE, TRUE, 0);
	
	GtkWidget *eventbox;
	eventbox = gtk_event_box_new();
	GdkColor color;
	color.pixel = 0;
	color.red   = 0xdf * 0x100;
	color.green = 0xe0 * 0x100;
	color.blue  = 0xe6 * 0x100;
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar),
										  eventbox);
	
	self->channelsgroups_widget = gtk_vbox_new(FALSE,0);
	gtk_container_add (GTK_CONTAINER(eventbox), 
					   self->channelsgroups_widget );
	
	freetuxtv_channels_list_update_from_db (self);
	
	gtk_widget_show_all (GTK_WIDGET(self));
	
	return GTK_WIDGET (self);
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
	query = "SELECT id_channelsgroup, name_channelsgroup \
             FROM channels_group";
	res=sqlite3_exec(db,
					 query,
					 freetuxtv_channels_list_db_exec_add_channels_group,
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
	
	return 0;
}

void
freetuxtv_channels_list_apply_filter (FreetuxTVChannelsList *self)
{
	int count = 0;
	GList* tmp;
	tmp = g_list_first (gtk_container_get_children (GTK_CONTAINER(self->channelsgroups_widget)));
	while (tmp != NULL){
		FreetuxTVChannelsGroup *channels_group;
		channels_group = FREETUXTV_CHANNELS_GROUP (tmp->data);
		gchar *filter;
		filter = (gchar *)gtk_entry_get_text (GTK_ENTRY(self->filter_widget));
		count += freetuxtv_channels_group_apply_filter (channels_group, filter);
		tmp = g_list_next (tmp); 
	}
}

FreetuxTVChannelsList *
freetuxtv_channels_list_get_from_widget (GtkWidget *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WIDGET(self), NULL);

	if(FREETUXTV_IS_CHANNELS_LIST(self)){
		return FREETUXTV_CHANNELS_LIST(self);
	}else{
		return freetuxtv_channels_list_get_from_widget (gtk_widget_get_parent(self));
	}
}

static void
freetuxtv_channels_list_event_button (GtkWidget *widget, GdkEventButton *event, gpointer func_data)
{
	FreetuxTVChannelsList *self;
	self = freetuxtv_channels_list_get_from_widget (widget);
	gtk_entry_set_text(GTK_ENTRY(self->filter_widget),"");
	freetuxtv_channels_list_apply_filter (self);
}

static void
freetuxtv_channels_list_event_entry_changed (GtkWidget *widget, gpointer func_data){
	FreetuxTVChannelsList *self;
	self = freetuxtv_channels_list_get_from_widget (widget);
	freetuxtv_channels_list_apply_filter (self);
}

static int 
freetuxtv_channels_list_db_exec_add_channels_group (void *data, int argc,
						    char **argv, char **colsname)
{
	FreetuxTVChannelsList *self = (FreetuxTVChannelsList *) data;
	FreetuxTVChannelsGroup *channels_group;
	channels_group=FREETUXTV_CHANNELS_GROUP (freetuxtv_channels_group_new (argv[0], argv[1]));
	freetuxtv_channels_list_add_channels_group (self, channels_group);
	return 0;
}

static void
freetuxtv_channels_list_init (FreetuxTVChannelsList *object)
{
	object->channelsgroups_widget = NULL;
	object->filter_widget = NULL;
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
