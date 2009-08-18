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
#include "freetuxtv-i18n.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-logos-list.h"
#include "gtk-libvlc-media-player.h"

gboolean
on_windowmain_deleteevent (GtkWidget *widget, GdkEvent *event, gpointer *data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;
	freetuxtv_action_quit (app);
	return TRUE;
}

void 
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
				     gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_quit (app);
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
	g_signal_connect(G_OBJECT(widget),
			 "response",
			 G_CALLBACK(on_aboutdialog_response),
			 NULL);
	
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


	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(app->channelslist));
	
	GtkWidget *treeview;
	treeview = glade_xml_get_widget (app->windowmain,
					 "windowsmain_treeviewchannelslist");
	gtk_tree_view_expand_all (GTK_TREE_VIEW(treeview));
}

void
on_windowmain_buttonprevious_clicked (GtkButton *button,
				    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_prev_channel (app);	
}

void
on_windowmain_buttonnext_clicked (GtkButton *button,
				       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_next_channel (app);	
}

void
on_windowmain_buttonstop_clicked (GtkButton *button,
				  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_stop_channel (app);
}

void
on_windowmain_buttonrecord_clicked (GtkButton *button,
				    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_record_channel (app);
}

void
on_windowmain_buttonplaypause_clicked (GtkButton *button,
				       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_playpause_channel (app);
}

void
on_windowmain_buttonfullscreen_clicked (GtkButton *button,
					gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_set_fullscreen (app->player, TRUE);
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

	volume = gtk_libvlc_media_player_get_volume (app->player);

	/* Affichage de la fenetre miniature */
	app->windowminimode = glade_xml_new (FREETUXTV_GLADEXML,
					     "windowminimode", NULL);
	glade_xml_signal_autoconnect (app->windowminimode);

	/* Connexion des signaux */
	widget = glade_xml_get_widget(app->windowminimode,
				      "windowminimode");
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(on_windowmain_deleteevent),
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
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(app->channelslist));

	GtkWidget *treeview;
	treeview = glade_xml_get_widget (app->windowmain,
					 "windowsmain_treeviewchannelslist");
	gtk_tree_view_expand_all (GTK_TREE_VIEW(treeview));
}

void
on_windowmain_volumecontrol_value_changed (GtkRange *range, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	app->config.volume = gtk_range_get_value (range);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume);
}

void
on_windowmain_menuitempreferences_activate (GtkMenuItem *menuitem,
					    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	app->dialogpreferences = glade_xml_new (FREETUXTV_GLADEXML,
						"dialogpreferences", NULL);
	glade_xml_signal_autoconnect (app->dialogpreferences);

	GtkWidget *widget;
	widget = glade_xml_get_widget(app->dialogpreferences,
				      "dialogpreferences");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
				"gtk-cancel", GTK_RESPONSE_CANCEL,
				"gtk-apply", GTK_RESPONSE_APPLY, NULL);
	g_signal_connect(G_OBJECT(widget),
			 "response",
			 G_CALLBACK(on_dialogpreferences_response),
			 app);
	
	widget = glade_xml_get_widget(app->dialogpreferences,
				      "dialogpreferences_channelonstartup");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->config.channelonstartup);
	
	widget = glade_xml_get_widget(app->dialogpreferences,
				      "dialogpreferences_directoryrecord");
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (widget),
					     app->config.directoryrecord);
}

void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
					  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	app->dialogaddgroup = glade_xml_new (FREETUXTV_GLADEXML,
					     "dialogaddgroup", NULL);
	glade_xml_signal_autoconnect (app->dialogaddgroup);
	
	// Connexion des signaux
	GtkWidget *widget;
	widget = glade_xml_get_widget(app->dialogaddgroup,
				      "dialogaddgroup_add");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_dialogaddgroup_add_clicked),
			 app);
}

void
on_windowmain_menuitemupdatelogos_activate (GtkMenuItem *menuitem,
					    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	logos_list_synchronize (app);			
	channels_list_load_channels (app);
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

	volume = gtk_libvlc_media_player_get_volume (app->player);
	
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
	
	gchar *errmsg = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	gchar * sgroupname;
	gchar * sgroupprotocole;
	gchar * sgroupuri;

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
		errmsg = g_strdup_printf(_("Please enter the group's name !"));
	}
	if(gtk_combo_box_get_active_text(GTK_COMBO_BOX(groupprotocole)) == NULL
	   && errmsg==NULL){
		errmsg = g_strdup_printf(_("Please choose the protocole of the group's URI !"));
	}
	if(g_ascii_strcasecmp(gtk_entry_get_text(GTK_ENTRY(groupuri)),"") == 0
	   && errmsg==NULL){
		errmsg = g_strdup_printf(_("Please enter the group's URI !"));
	}
	
	if(errmsg != NULL){
		windowmain_show_error (app, errmsg);
	}else{

		sgroupname = g_strdup(gtk_entry_get_text(GTK_ENTRY(groupname)));
		sgroupuri = g_strconcat(gtk_combo_box_get_active_text(GTK_COMBO_BOX(groupprotocole)),
					gtk_entry_get_text(GTK_ENTRY(groupuri)),
					NULL);

		FreetuxTVChannelsGroupInfos *channels_group_infos;		
		channels_group_infos = freetuxtv_channels_group_infos_new (sgroupname, sgroupuri);
		freetuxtv_channels_group_infos_set_regex (channels_group_infos,
							  (gchar*)gtk_entry_get_text(GTK_ENTRY(bregex)),
							  (gchar*)gtk_entry_get_text(GTK_ENTRY(eregex)));

		channels_list_add_channels_group (app, channels_group_infos);
		g_free(sgroupname);
		g_free(sgroupuri);
	
		
		// TODO channels_list_add_channels_group (app, channels_group);

		// TODO channels_list_refresh_group (app, channels_group);
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
			 gint response_id,
			 gpointer user_data)
{
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
on_dialogpreferences_response (GtkDialog *dialog,
			       gint response_id,
			       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	GtkWidget* widget;
	
	if(response_id == GTK_RESPONSE_APPLY){	
		widget = glade_xml_get_widget(app->dialogpreferences,
					      "dialogpreferences_channelonstartup");
		app->config.channelonstartup = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	
		widget = glade_xml_get_widget(app->dialogpreferences,
					      "dialogpreferences_directoryrecord");
		g_free(app->config.directoryrecord);
		app->config.directoryrecord = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (widget));	       

	}
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
windowmain_display_buttons (FreetuxTVApp *app, FreetuxTVWindowMode mode)
{
	GtkWidget *widget;
	gboolean sensitive;
	GtkWidget *image;
	// Button previous
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonprevious");
	switch(mode){
	case WINDOW_MODE_STOPPED :
		if(app->current.channel != NULL){
			sensitive = TRUE;		
		}else{
			sensitive = FALSE;	
		}
		break;
	case WINDOW_MODE_RECORDING :
		sensitive = FALSE;
		break;
	case WINDOW_MODE_PLAYING :
		sensitive = TRUE;
		break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
	
	// Button next
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonnext");
	switch(mode){
	case WINDOW_MODE_STOPPED :
		if(app->current.channel != NULL){
			sensitive = TRUE;		
		}else{
			sensitive = FALSE;	
		}
		break;
	case WINDOW_MODE_RECORDING :
		sensitive = FALSE;
		break;
	case WINDOW_MODE_PLAYING :
		sensitive = TRUE;		
		break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
	
	// Button play/pause
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonplaypause");
	switch(mode){
	case WINDOW_MODE_STOPPED :
		if(app->current.channel == NULL){
			sensitive = FALSE;
		}else{
			sensitive = TRUE;
		}
		image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_BUTTON);
		break;
	case WINDOW_MODE_PLAYING :
	case WINDOW_MODE_RECORDING :
		sensitive = TRUE;
		image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON);
		break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
	gtk_button_set_image (GTK_BUTTON(widget), image);
	
	// Button stop
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonstop");
	switch(mode){
	case WINDOW_MODE_STOPPED :
		sensitive = FALSE;
		break;
	case WINDOW_MODE_PLAYING :
	case WINDOW_MODE_RECORDING :
		sensitive = TRUE;		
		break;
	}
	gtk_widget_set_sensitive(widget, sensitive);

	// Button record
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonrecord");
	switch(mode){
	case WINDOW_MODE_STOPPED :
	case WINDOW_MODE_RECORDING :
		sensitive = FALSE;
		break;
	case WINDOW_MODE_PLAYING :
		sensitive = TRUE;		
		break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
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
					msg, NULL);
	
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
