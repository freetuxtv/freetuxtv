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

#include "freetuxtv-app.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-player.h"

void
on_windowmain_destroy (GtkWidget *widget, gpointer *data)
{
	gtk_main_quit();
}

void 
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
				     gpointer user_data)
{
	gtk_main_quit();
}

void
on_windowmain_menuitemaboutdialog_activate (GtkMenuItem *menuitem,
					    gpointer user_data)
{
	GladeXML *gladexml;
	GtkWidget *widget;
	gladexml = glade_xml_new (FREETUXTV_GLADEXML,
				  "aboutdialog", NULL);
	glade_xml_signal_autoconnect (gladexml);

	widget = glade_xml_get_widget(gladexml,
				      "aboutdialog");
	
	gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG(widget), "FreetuxTV");
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(widget), VERSION);

}

void
on_windowmain_buttonclearfilter_clicked (GtkButton *button,
					 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GtkWidget *entryfilter;
	entryfilter = glade_xml_get_widget(app->windowmain,
					   "windowmain_entryfilter");
	gtk_entry_set_text(GTK_ENTRY(entryfilter), "");
	channels_list_apply_filter (app);
}

void
on_windowmain_buttongoback_clicked (GtkButton *button,
				    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	GtkWidget* widget;
	GList* children;
	int pos;
	FreetuxTVChannel *newchannel;
	
	widget = gtk_widget_get_parent (GTK_WIDGET(app->current.channel));
	children = gtk_container_get_children (GTK_CONTAINER(widget));
	pos =  g_list_index(children, app->current.channel);

	if(pos > 0){
		newchannel = FREETUXTV_CHANNEL(g_list_nth_data (children,
								pos - 1));
		on_channel_dbl_clicked (newchannel, (gpointer)app);
	}
	
}

void
on_windowmain_buttongoforward_clicked (GtkButton *button,
				       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	GtkWidget* widget;
	GList* children;
	int pos;
	FreetuxTVChannel *newchannel;
	
	widget = gtk_widget_get_parent (GTK_WIDGET(app->current.channel));
	children = gtk_container_get_children (GTK_CONTAINER(widget));
	pos =  g_list_index(children, app->current.channel);

	if(pos < g_list_length(children)){
		newchannel = FREETUXTV_CHANNEL(g_list_nth_data (children,
								pos + 1));
		on_channel_dbl_clicked (newchannel, (gpointer)app);
	}
	
}

void
on_windowmain_buttonstop_clicked (GtkButton *button,
				  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_player_stop (app->player);
}

void
on_windowmain_buttonfullscreen_clicked (GtkButton *button,
					gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_player_fullscreen (app->player);
}

void
on_windowmain_buttonminimode_clicked (GtkButton *button,
				      gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GladeXML *tmp;	
	GtkWidget *widget;

	gdouble volume;
	
	widget = glade_xml_get_widget(app->windowmain,
				      "windowmain");
	gtk_widget_hide (widget);

	volume = freetuxtv_player_get_volume (app->player);

	/* Affichage de la fenetre miniature */
	app->windowminimode = glade_xml_new (FREETUXTV_GLADEXML,
					     "windowminimode", NULL);
	glade_xml_signal_autoconnect (app->windowminimode);

	/* Connexion des signaux */
	widget = glade_xml_get_widget(app->windowminimode,
				      "windowminimode");
	g_signal_connect(G_OBJECT(widget),
			 "destroy",
			 G_CALLBACK(on_windowmain_destroy),
			 app);
	
	widget = glade_xml_get_widget(app->windowminimode,
				      "windowminimode_buttonnormalmode");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowminimode_buttonnormalmode_clicked),
			 app);
	
	widget = glade_xml_get_widget(app->windowminimode,
				      "windowminimode_buttonstayontop");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget),
				      app->config.windowminimode_stayontop);
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowminimode_buttonstayontop_clicked),
			 app);

	widget = glade_xml_get_widget(app->windowminimode,
				      "windowminimode_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);
	
	widget = glade_xml_get_widget (app->windowminimode,
				       "windowminimode_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
			 "value-changed",
			 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
			 app);
	gtk_range_set_value (GTK_RANGE(widget), volume);

	windowminimode_set_from_config (app);
}

void
on_windowmain_entryfilter_changed (GtkEntry *entry, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	channels_list_apply_filter (app);
}

void
on_windowmain_volumecontrol_value_changed (GtkRange *range, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_player_set_volume (app->player,
				     gtk_range_get_value (range));
}

void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
					  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	app->dialogaddgroup = glade_xml_new (FREETUXTV_GLADEXML,
					     "dialogaddgroup", NULL);
	glade_xml_signal_autoconnect (app->dialogaddgroup);
	
	/* Connexion des signaux */
	GtkWidget *widget;
	widget = glade_xml_get_widget(app->dialogaddgroup,
				      "dialogaddgroup_add");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_dialogaddgroup_add_clicked),
			 app);
}

void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
					    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *windowminimode;
	GladeXML *tmp;
	
	GtkWidget *widget;
	gdouble volume;

	volume = freetuxtv_player_get_volume (app->player);
	
	widget = glade_xml_get_widget(app->windowminimode,
				      "windowminimode");
	gtk_window_get_size (GTK_WINDOW(widget), 
			     &app->config.windowminimode_width,
			     &app->config.windowminimode_height);

	gtk_widget_hide (widget);
	
	/* Reaffichage de la fenetre principal */
	widget = glade_xml_get_widget(app->windowmain,
				      "windowmain");
	gtk_widget_show (widget);
	
	widget = glade_xml_get_widget(app->windowmain,
				      "windowmain_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_volumecontrol");
	gtk_range_set_value (GTK_RANGE(widget), volume);
}

void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
					   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	if(app->config.windowminimode_stayontop){
		app->config.windowminimode_stayontop = FALSE;	
	}else{
		app->config.windowminimode_stayontop = TRUE;
	}

	windowminimode_set_from_config (app);
}

void
on_dialogaddgroup_add_clicked (GtkButton *button,
			       gpointer user_data)
{
	GtkWidget *windowmain;
	GtkWidget *channelslist;
	GtkWidget *groupname;
	GtkWidget *groupprotocole;
	GtkWidget *groupuri;
	GtkWidget *bregex;
	GtkWidget *eregex;
	GtkWidget *dialog;
	
	gchar *user_db;
		
	struct sqlite3 *db;
	int res;
	char *sqlerr = 0;

	gchar *errmsg = NULL;
	gchar *query;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	gchar * sgroupid;
	gchar * sgroupname;
	gchar * sgroupprotocole;
	gchar * sgroupuri;

	user_db = g_strdup_printf("%s/FreetuxTV/freetuxtv.db",
				  g_get_user_config_dir());
	
	groupname = glade_xml_get_widget(app->dialogaddgroup,
					 "dialogaddgroup_name");
	groupprotocole = glade_xml_get_widget(app->dialogaddgroup,
					      "dialogaddgroup_protocole");
	groupuri = glade_xml_get_widget(app->dialogaddgroup,
					"dialogaddgroup_uri");
	bregex = glade_xml_get_widget(app->dialogaddgroup,
				      "dialogaddgroup_bregex");
	eregex = glade_xml_get_widget(app->dialogaddgroup,
				      "dialogaddgroup_eregex");
	
	/* Verification des champs */
	if(g_ascii_strcasecmp(gtk_entry_get_text(GTK_ENTRY(groupname)),"") == 0
	   && errmsg==NULL){
		errmsg = g_strdup_printf("Veuillez remplir le nom du groupe !");
	}
	if(gtk_combo_box_get_active_text(GTK_COMBO_BOX(groupprotocole)) == NULL
	   && errmsg==NULL){
		errmsg = g_strdup_printf("Veuillez choisir le protocole de l'URI du groupe !");
	}
	if(g_ascii_strcasecmp(gtk_entry_get_text(GTK_ENTRY(groupuri)),"") == 0
	   && errmsg==NULL){
		errmsg = g_strdup_printf("Veuillez remplir l'URI du groupe !");
	}
	
	/* Ouverture de la BDD */
	if(errmsg==NULL){
		res = sqlite3_open(user_db,&db);
		if(res != SQLITE_OK){
			errmsg = g_strdup_printf("Impossible d'ouvrir la base de données.\n\nSQLite a retouné l'erreur :\n%s.",
						 sqlite3_errmsg(db));
			sqlite3_close(db);
		}
	}
	
	if(errmsg == NULL){
		sgroupname = g_strdup(gtk_entry_get_text(GTK_ENTRY(groupname)));
		sgroupuri = g_strconcat(gtk_combo_box_get_active_text(GTK_COMBO_BOX(groupprotocole)),
					gtk_entry_get_text(GTK_ENTRY(groupuri)),
					NULL);
		query = sqlite3_mprintf("INSERT INTO channels_group (name_channelsgroup, uri_channelsgroup, bregex_channelsgroup, eregex_channelsgroup) VALUES ('%q','%q', '%q', '%q');", 
					sgroupname,
					sgroupuri,
					gtk_entry_get_text(GTK_ENTRY(bregex)),
					gtk_entry_get_text(GTK_ENTRY(eregex))
					);
		res=sqlite3_exec(db, query, NULL, 0, &sqlerr);
		sqlite3_free(query);
		if(res != SQLITE_OK){
			errmsg = g_strdup_printf("Impossible d'ajouter le groupe \"%s\" dans la base de données.\n\nSQLite a retouné l'erreur :\n%s.",
						 gtk_entry_get_text(GTK_ENTRY(groupname)),
						 sqlite3_errmsg(db));
			g_free(sgroupname);
			g_free(sgroupuri);
		}
		sqlite3_free(sqlerr);
		sqlite3_close(db);
	}
	
	if(errmsg != NULL){
		windowmain_show_error (app, errmsg);
	}else{
		FreetuxTVChannelsGroup *channels_group;
		
		sgroupid = g_strdup_printf("%d", sqlite3_last_insert_rowid(db));
		channels_group = FREETUXTV_CHANNELS_GROUP (freetuxtv_channels_group_new (sgroupid,
											 sgroupname,
											 sgroupuri));
		channels_list_add_channels_group (app, channels_group);

		g_free(sgroupid);
		g_free(sgroupname);
		g_free(sgroupuri);

		channels_list_refresh_group (app, channels_group);
		//channels_group_reload_channels (channels_group, app);
		
		gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
	}

	g_free(errmsg);
}

void
on_dialogaddgroup_cancel_clicked (GtkButton *button,
				  gpointer user_data)
{
	gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
}

void
on_aboutdialog_response (GtkDialog *dialog,
			 gpointer user_data)
{
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
windowmain_show_error (FreetuxTVApp *app, gchar *msg)
{
	GtkWidget *windowmain;
	GtkWidget* dialog;
	
	windowmain = glade_xml_get_widget (app->windowmain,
					   "windowmain");
	
	dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
					GTK_DIALOG_MODAL, 
					GTK_MESSAGE_ERROR,
					GTK_BUTTONS_OK,
					msg);
	
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void
windowmain_statusbar_push (FreetuxTVApp *app, gchar *context, gchar *msg)
{
	
	int context_id;

	GtkWidget *statusbar;
	statusbar = glade_xml_get_widget (app->windowmain,
					  "windowmain_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
						  context);
	gtk_statusbar_push (GTK_STATUSBAR(statusbar), 
			    context_id,
			    msg);
	while (g_main_context_iteration(NULL, FALSE)){}
}


void
windowmain_statusbar_pop (FreetuxTVApp *app, gchar *context)
{
	
	int context_id;

	GtkWidget *statusbar;
	statusbar = glade_xml_get_widget (app->windowmain,
					  "windowmain_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
						  context);
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
			   context_id);
	// while (g_main_context_iteration(NULL, FALSE)){}		
}

void
windowminimode_set_from_config (FreetuxTVApp *app)
{
	GtkWidget *widget;

	widget = glade_xml_get_widget(app->windowminimode,
				      "windowminimode");
	gtk_window_set_keep_above (GTK_WINDOW(widget),
				   app->config.windowminimode_stayontop);

	gtk_window_resize (GTK_WINDOW(widget),
			   app->config.windowminimode_width,
			   app->config.windowminimode_height);
}
