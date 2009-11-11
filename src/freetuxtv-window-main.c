/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <gtk/gtk.h>
#include <sqlite3.h>

#include "freetuxtv-window-main.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-utils.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-logos-list.h"
#include "freetuxtv-db-sync.h"
#include "freetuxtv-models.h"
#include "gtk-libvlc-media-player.h"

static gboolean
on_windowmain_deleteevent (GtkWidget *widget, GdkEvent *event, gpointer *data);

static void
on_windowmain_menuitempreferences_activate (GtkMenuItem *menuitem,
					    gpointer user_data);

static void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
					  gpointer user_data);

static void
on_windowmain_menuitemupdatelogos_activate (GtkMenuItem *menuitem,
					    gpointer user_data);

static void
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
				     gpointer user_data);

static void
on_windowmain_menuitemaboutdialog_activate (GtkMenuItem *menuitem,
					    gpointer user_data);

static gboolean
on_windowmain_valuechanged (GtkRange *range, GtkScrollType scroll,
			    gdouble value, gpointer user_data);

static void
on_windowmain_buttonclearfilter_clicked (GtkButton *button,
					 gpointer user_data);

static void
on_windowmain_buttongotocurrent_clicked (GtkButton *button,
					 gpointer user_data);

static void
on_windowmain_buttonprevious_clicked (GtkButton *button,
				      gpointer user_data);

static void
on_windowmain_buttonnext_clicked (GtkButton *button,
				  gpointer user_data);

static void
on_windowmain_buttonstop_clicked (GtkButton *button,
				  gpointer user_data);

static void
on_windowmain_buttonrecord_clicked (GtkButton *button,
				    gpointer user_data);

static void
on_windowmain_buttonplaypause_clicked (GtkButton *button,
				  gpointer user_data);

static void
on_windowmain_buttonfullscreen_clicked (GtkButton *button,
					gpointer user_data);

static void
on_windowmain_buttonminimode_clicked (GtkButton *button,
				      gpointer user_data);

static void
on_windowmain_entryfilter_changed (GtkEntry *entry,
				   gpointer user_data);

static void
on_windowmain_volumecontrol_value_changed (GtkRange *range,
					   gpointer user_data);

static void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
					    gpointer user_data);

static void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
					   gpointer user_data);

static void
on_dialogpreferences_directoryrecordings_changed (GtkFileChooser *chooser,
						  gpointer user_data);

static void
on_dialogpreferences_radiotranscoding_toggled (GtkToggleButton *togglebutton,
					       gpointer user_data);

static void
on_dialogpreferences_transcodingformats_changed (GtkComboBox *widget,
						 gpointer user_data);

static void
on_dialogpreferences_response (GtkDialog *dialog,
			       gint response_id,
			       gpointer   user_data);

static void
on_dialogaddgroup_response (GtkDialog *dialog,
			    gint response_id,
			    gpointer user_data);

static void
on_dialogaddgroup_buttonrefresh_clicked (GtkButton *button,
					 gpointer user_data);

static void
on_dialoggroupproperties_response (GtkDialog *dialog,
				   gint response_id,
				   gpointer user_data);

static void
on_aboutdialog_response (GtkDialog *dialog,
			 gint response_id,
			 gpointer user_data);

static void
dialogpreferences_update_view(FreetuxTVApp *app);

void
windowmain_init(FreetuxTVApp *app)
{
	GtkWidget *widget;
	GtkWidget *button;
	GtkWidget *menu_bar = NULL;
	
	// Initialize menu bar	
	menu_bar = gtk_menu_bar_new ();
	GtkWidget *p_menu = NULL;
	GtkWidget *p_menu_item = NULL;
	
	p_menu_item = gtk_menu_item_new_with_mnemonic (_("_FreetuxTV"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), p_menu_item);
	p_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (p_menu_item), p_menu);
	p_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES, NULL);	
	gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
	g_signal_connect(G_OBJECT(p_menu_item),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitempreferences_activate),
			 app);
	p_menu_item = gtk_separator_menu_item_new();	
	gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
	p_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);	
	gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
	g_signal_connect(G_OBJECT(p_menu_item),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemquit_activate),
			 app);

	p_menu_item = gtk_menu_item_new_with_mnemonic (_("_Channels"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), p_menu_item);
	p_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (p_menu_item), p_menu);
	p_menu_item = gtk_menu_item_new_with_mnemonic (_("_Add a group"));
	gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
	g_signal_connect(G_OBJECT(p_menu_item),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemgroupsadd_activate),
			 app);
	p_menu_item = gtk_menu_item_new_with_mnemonic (_("_Update logos list"));
	gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
	g_signal_connect(G_OBJECT(p_menu_item),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemupdatelogos_activate),
			 app);

	p_menu_item = gtk_menu_item_new_with_mnemonic (_("_Video"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), p_menu_item);
	p_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (p_menu_item), p_menu);
	p_menu_item = gtk_menu_item_new_with_mnemonic (_("_Deinterlace"));
	gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
	g_signal_connect(G_OBJECT(p_menu_item),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemgroupsadd_activate),
			 app);

	p_menu_item = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), p_menu_item);
	p_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (p_menu_item), p_menu);
	p_menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);	
	gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
	g_signal_connect(G_OBJECT(p_menu_item),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemaboutdialog_activate),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_menubox");
	gtk_box_pack_start (GTK_BOX (widget), GTK_WIDGET (menu_bar), TRUE, TRUE, 0);
	gtk_widget_show_all(menu_bar);

	// Initialize signals for windowmain
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain");
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(on_windowmain_deleteevent),
			 app);

	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(on_windowmain_deleteevent),
			 app);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonclearfilter");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonclearfilter_clicked),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_entryfilter");
	g_signal_connect(G_OBJECT(widget),
			 "changed",
			 G_CALLBACK(on_windowmain_entryfilter_changed),
			 app);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_scaletime");
	g_signal_connect(G_OBJECT(widget),
			 "change-value",
			 G_CALLBACK(on_windowmain_valuechanged),
			 app);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttongotocurrent");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttongotocurrent_clicked),
			 app);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonprevious");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonprevious_clicked),
			 app);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonnext");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonnext_clicked),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonplaypause");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonplaypause_clicked),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonstop");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonstop_clicked),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonrecord");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonrecord_clicked),
			 app);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
			 "value-changed",
			 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonfullscreen");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonfullscreen_clicked),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_buttonminimode");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonminimode_clicked),
			 app);

	// Initialize signals for windowminimode
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode");
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(on_windowmain_deleteevent),
			 app);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode_buttonnormalmode");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowminimode_buttonnormalmode_clicked),
			 app);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode_buttonstayontop");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowminimode_buttonstayontop_clicked),
			 app);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
			 "value-changed",
			 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
			 app);

	// Initialize signals for dialogpreferences
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogpreferences");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
				"gtk-cancel", GTK_RESPONSE_CANCEL,
				"gtk-apply", GTK_RESPONSE_APPLY, NULL);
	
	g_signal_connect(G_OBJECT(widget),
			 "response",
			 G_CALLBACK(on_dialogpreferences_response),
			 app);
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(gtk_widget_hide_on_delete),
			 NULL);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogpreferences_directoryrecordings");	
	g_signal_connect(G_OBJECT(widget),
			 "selection-changed",
			 G_CALLBACK(on_dialogpreferences_directoryrecordings_changed),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogpreferences_radiotranscodingno");	
	g_signal_connect(G_OBJECT(widget),
			 "toggled",
			 G_CALLBACK(on_dialogpreferences_radiotranscoding_toggled),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogpreferences_radiotranscodingpredefinedformats");	
	g_signal_connect(G_OBJECT(widget),
			 "toggled",
			 G_CALLBACK(on_dialogpreferences_radiotranscoding_toggled),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogpreferences_transcodingformats");	
	g_signal_connect(G_OBJECT(widget),
			 "changed",
			 G_CALLBACK(on_dialogpreferences_transcodingformats_changed),
			 app);

	// Initialize signals for dialogaddgroup
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogaddgroup");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
				"gtk-close", GTK_RESPONSE_CLOSE, NULL);
	button = gtk_button_new_from_stock ("gtk-add");
	gtk_dialog_add_action_widget (GTK_DIALOG(widget),
				      button, FREETUXTV_RESPONSE_ADD);
	gtk_widget_show(button);
	
	g_signal_connect(G_OBJECT(widget),
			 "response",
			 G_CALLBACK(on_dialogaddgroup_response),
			 app);
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(gtk_widget_hide_on_delete),
			 NULL);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogaddgroup_buttonrefresh");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_dialogaddgroup_buttonrefresh_clicked),
			 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialogaddgroup_treeviewchannelsgroups");
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);
	 

	// Initialize signals for dialoggroupproperties
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "dialoggroupproperties");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
				"gtk-cancel", GTK_RESPONSE_CANCEL, "gtk-apply", GTK_RESPONSE_APPLY, NULL);
	gtk_widget_show(button);
	
	g_signal_connect(G_OBJECT(widget),
			 "response",
			 G_CALLBACK(on_dialoggroupproperties_response),
			 app);
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(gtk_widget_hide_on_delete),
			 NULL);

	// Initialize signals for aboutdialog
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
							"aboutdialog");

	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(widget), VERSION);

	g_signal_connect(G_OBJECT(widget),
			 "response",
			 G_CALLBACK(on_aboutdialog_response),
			 NULL);	
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(gtk_widget_hide_on_delete),
			 NULL);	
}

void
windowmain_display_buttons (FreetuxTVApp *app, FreetuxTVWindowMode mode)
{
	GtkWidget *widget;
	gboolean sensitive;
	GtkWidget *image;
	// Button previous
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_buttonprevious");
	switch(mode){
	case WINDOW_MODE_STOPPED :
		if(app->current.path_channel != NULL){
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
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_buttonnext");
	switch(mode){
	case WINDOW_MODE_STOPPED :
		if(app->current.path_channel != NULL){
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
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_buttonplaypause");
	switch(mode){
	case WINDOW_MODE_STOPPED :
	case WINDOW_MODE_PAUSED :
		if(app->current.path_channel == NULL){
			sensitive = FALSE;
		}else{
			sensitive = TRUE;
		}
		image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_BUTTON);
		break;
	case WINDOW_MODE_PLAYING :
		sensitive = TRUE;
		image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON);
		break;
	case WINDOW_MODE_RECORDING :
		sensitive = FALSE;
		image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON);
		break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
	gtk_button_set_image (GTK_BUTTON(widget), image);
	
	// Button stop
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
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
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
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
	
	windowmain = (GtkWidget *) gtk_builder_get_object (app->gui,
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
windowmain_show_gerror (FreetuxTVApp *app, GError* error)
{
	g_return_if_fail(error != NULL);
	windowmain_show_error (app, error->message);
}

void
windowmain_statusbar_push (FreetuxTVApp *app, gchar *context, gchar *msg)
{
	int context_id;

	GtkWidget *statusbar;
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
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
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
							  "windowmain_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
						  context);
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
			   context_id);
	// while (g_main_context_iteration(NULL, FALSE)){}		
}

void
windowmain_update_statusbar_infos (FreetuxTVApp *app)
{
	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_channelscount");

	gint channels_count;
	channels_count = channels_list_get_channels_count (app);
	
	gchar *str;
	str = g_strdup_printf("%d", channels_count);
	gtk_label_set_text(GTK_LABEL(widget), str);
	g_free(str);	
}

void
windowmain_timebar_update (FreetuxTVApp *app, glong time_ms, glong length_ms, gfloat position)
{
	GtkWidget *widget;
	
	gdouble time_s = time_ms/1000.0;
	gdouble length_s = length_ms/1000.0;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_labeltime");
	gtk_label_set_text(GTK_LABEL(widget), format_time2((glong)time_s));
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_labellength");
	gtk_label_set_text(GTK_LABEL(widget), format_time2((glong)length_s));	

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "adjustment_time");
	gtk_adjustment_set_value (GTK_ADJUSTMENT(widget), position);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_hboxtimebar");
	if(gtk_libvlc_media_player_is_seekable(app->player)){
		gtk_widget_set_sensitive(widget, TRUE);
	}else{
		gtk_widget_set_sensitive(widget, FALSE);
	}
}

static gboolean
on_windowmain_deleteevent (GtkWidget *widget, GdkEvent *event, gpointer *data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;
	freetuxtv_quit (app);
	return TRUE;
}

static void 
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
				     gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_quit (app);
}

static void
on_windowmain_menuitemaboutdialog_activate (GtkMenuItem *menuitem,
					    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GtkWidget *widget;
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
							"aboutdialog");
	gtk_widget_show(widget);
}

static void
on_windowmain_buttonclearfilter_clicked (GtkButton *button,
					 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GtkWidget *entryfilter;
	entryfilter =  (GtkWidget *) gtk_builder_get_object (app->gui,
							     "windowmain_entryfilter");
	gtk_entry_set_text(GTK_ENTRY(entryfilter), "");
	
	GtkWidget *treeview;
	GtkTreeModel *model;
	treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
							  "windowmain_treeviewchannelslist");

	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));

	gtk_tree_view_collapse_all (GTK_TREE_VIEW(treeview));
	channels_list_set_playing (app, app->current.path_channel);
}

static void
on_windowmain_buttongotocurrent_clicked (GtkButton *button,
					 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	channels_list_set_playing(app, app->current.path_channel);
}

static void
on_windowmain_buttonprevious_clicked (GtkButton *button,
				    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_prev (app);	
}

static void
on_windowmain_buttonnext_clicked (GtkButton *button,
				       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_next (app);	
}

static void
on_windowmain_buttonstop_clicked (GtkButton *button,
				  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_stop (app);
}

static void
on_windowmain_buttonrecord_clicked (GtkButton *button,
				    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_record (app);
}

static void
on_windowmain_buttonplaypause_clicked (GtkButton *button,
				       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_playpause (app);
}

static void
on_windowmain_buttonfullscreen_clicked (GtkButton *button,
					gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_set_fullscreen (app->player, TRUE);
}

static void
on_windowmain_buttonminimode_clicked (GtkButton *button,
				      gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;	
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain");
	gtk_widget_hide(widget);

	// Display the mini mode window
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode");
	gtk_widget_show(widget);
	gtk_window_set_keep_above (GTK_WINDOW(widget),
				   app->config.windowminimode_stayontop);

	gtk_window_resize (GTK_WINDOW(widget),
			   app->config.windowminimode_width,
			   app->config.windowminimode_height);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode_buttonstayontop");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget),
				      app->config.windowminimode_stayontop);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);
}

static void
on_windowmain_entryfilter_changed (GtkEntry *entry, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *treeview;
	GtkTreeModel *model;
	treeview = (GtkWidget *) gtk_builder_get_object (app->gui,
							 "windowmain_treeviewchannelslist");
	
	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));

	const gchar* text;
	text = gtk_entry_get_text (entry);
	
	if(g_ascii_strcasecmp(text, "") == 0){
		gtk_tree_view_collapse_all (GTK_TREE_VIEW(treeview));
		channels_list_set_playing (app, app->current.path_channel);
	}else{
		gtk_tree_view_expand_all (GTK_TREE_VIEW(treeview));
	}
}

static void
on_windowmain_volumecontrol_value_changed (GtkRange *range, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	app->config.volume = gtk_range_get_value (range);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume);
}

static void
on_windowmain_menuitempreferences_activate (GtkMenuItem *menuitem,
					    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "dialogpreferences");
	gtk_widget_show(widget);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "dialogpreferences_channelonstartup");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.channelonstartup);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "dialogpreferences_enablenotifications");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.enable_notifications);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "dialogpreferences_directoryrecordings");
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (widget),
					     app->prefs.directoryrecordings);

	switch(app->prefs.transcoding_mode){
	case 0:
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_radiotranscodingno");		
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
		break;
	case 1:
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_radiotranscodingpredefinedformats");		
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);				
		break;
	default:
		break;
	}

	GtkTreeModel *model;
	GtkTreeIter iter;

	model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_transcodeformat");
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "dialogpreferences_transcodingformats");
	gtk_tree_model_get_iter_from_string (model, &iter, app->prefs.transcoding_format);
	gtk_combo_box_set_active_iter (GTK_COMBO_BOX(widget), &iter);

	dialogpreferences_update_view (app);

}

static void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
					  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "dialogaddgroup");
	gtk_widget_show(widget);
}

static void
on_windowmain_menuitemupdatelogos_activate (GtkMenuItem *menuitem,
					    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);
	
	if(error == NULL){
		logos_list_synchronize (app, &dbsync, &error);			
	}

	if(error == NULL){
		channels_list_load_channels (app, &dbsync, &error);
	}

	dbsync_close_db(&dbsync);
	
	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}


static gboolean
on_windowmain_valuechanged (GtkRange *range, GtkScrollType scroll,
			    gdouble value, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	g_print("change %f\n", value); // TODO
	if(!gtk_libvlc_media_player_is_playing(app->player)){
		gtk_libvlc_media_player_play(app->player, NULL);
	}
	if(value >= 0.0 && value <= 1.0){
		gtk_libvlc_media_player_set_position(app->player, value);
	}
	return FALSE;
}

static void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
					    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *windowminimode;
	
	GtkWidget *widget;
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode");
	gtk_window_get_size (GTK_WINDOW(widget),
			     &app->config.windowminimode_width,
			     &app->config.windowminimode_height);

	gtk_widget_hide (widget);
	
	// Display the normal mode window
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain");
	gtk_widget_show (widget);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowmain_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);
}

static void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
					   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	if(app->config.windowminimode_stayontop){
		app->config.windowminimode_stayontop = FALSE;	
	}else{
		app->config.windowminimode_stayontop = TRUE;
	}

	GtkWidget *widget;	
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "windowminimode");
	gtk_window_set_keep_above (GTK_WINDOW(widget),
				   app->config.windowminimode_stayontop);
}

static void
on_dialogpreferences_directoryrecordings_changed (GtkFileChooser *chooser,
						  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);	
}

static void
on_dialogpreferences_radiotranscoding_toggled (GtkToggleButton *togglebutton,
					       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_transcodingformats_changed (GtkComboBox *widget,
						 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);	
}

static void
on_dialogpreferences_response (GtkDialog *dialog,
			       gint response_id,
			       gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	GtkWidget* widget;
	if(response_id == GTK_RESPONSE_APPLY){
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_channelonstartup");
		app->prefs.channelonstartup = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_enablenotifications");
		app->prefs.enable_notifications = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_directoryrecordings");
		g_free(app->prefs.directoryrecordings);
		app->prefs.directoryrecordings = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_radiotranscodingno");	
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.transcoding_mode = 0;	
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_radiotranscodingpredefinedformats");		
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.transcoding_mode = 1;	
		}

		GtkTreeModel *model;
		GtkTreeIter iter;
		
		model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_transcodeformat");
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_transcodingformats");
		gtk_combo_box_get_active_iter (GTK_COMBO_BOX(widget), &iter);
		app->prefs.transcoding_format = gtk_tree_model_get_string_from_iter (model, &iter);

	}
	gtk_widget_hide(GTK_WIDGET(dialog));
}

static void
on_dialogaddgroup_response (GtkDialog *dialog,
			    gint response_id,
			    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GError* error = NULL;
	gchar *errmsg = NULL;

	GtkWidget* widget;
	
	const gchar *sgroupname = NULL;
	const gchar *sgroupuri = NULL;
	const gchar *sbregex = NULL;
	const gchar *seregex = NULL;
	
	if(response_id == FREETUXTV_RESPONSE_ADD){
		DBSync dbsync;
		dbsync_open_db (&dbsync, &error);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogaddgroup_notebook");
		if(error == NULL){
			int page;
			page = gtk_notebook_get_current_page (GTK_NOTEBOOK(widget));
			switch(page){
			case 0:
				widget = (GtkWidget *)gtk_builder_get_object (app->gui,
									      "dialogaddgroup_treeviewchannelsgroups");
				GtkTreeSelection *selection;
				selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
				
				GtkTreeModel* model;
				model = (GtkTreeModel *) gtk_builder_get_object (app->gui,
										 "treestore_channelsgroup");
				GList *list;
				list = gtk_tree_selection_get_selected_rows (selection, &model);
				
				GList* iterator = NULL;
				iterator = g_list_first (list);
				
				if(iterator == NULL){
					errmsg = g_strdup_printf(_("Please select a least one group !"));
				}
				
				GtkTreePath* last_ppath = NULL;
				FreetuxTVChannelsGroupInfos *channels_group_infos;
				
				while(iterator != NULL && error == NULL){
					GtkTreePath* path;
					path = (GtkTreePath*)iterator->data;
					GtkTreePath* parent_path;
					
					GtkTreeIter iter;
					GtkTreeIter iter_parent;
					gboolean valid = TRUE;				
					
					if(gtk_tree_path_get_depth (path) == 1){
						last_ppath = path;
						
						if(gtk_tree_model_get_iter (model, &iter_parent, path)){
							if(gtk_tree_model_iter_children (model, &iter, &iter_parent)){
								
								do {
									gtk_tree_model_get (model, &iter, MODEL_CHANNELSGROUP_NAME, &sgroupname,
											    MODEL_CHANNELSGROUP_URI, &sgroupuri,
											    MODEL_CHANNELSGROUP_BREGEX, &sbregex,
											    MODEL_CHANNELSGROUP_EREGEX, &seregex, -1);
									
									channels_group_infos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname, (gchar*)sgroupuri);
									freetuxtv_channels_group_infos_set_regex (channels_group_infos, (gchar*)sbregex, (gchar*)seregex);
									channels_list_add_channels_group (app, channels_group_infos, &dbsync, &error);
									
								}while (gtk_tree_model_iter_next(model, &iter) && error == NULL);
								
							}
						}
					}else{
						parent_path = gtk_tree_path_copy (path);
						gtk_tree_path_up(parent_path);
						
						if(last_ppath != NULL){
							if(gtk_tree_path_compare (parent_path, last_ppath) == 0){
								valid = FALSE;
							}
						}
						gtk_tree_path_free (parent_path);
						
						if(valid){
							if(gtk_tree_model_get_iter (model, &iter, path)){
								gtk_tree_model_get (model, &iter, MODEL_CHANNELSGROUP_NAME, &sgroupname,
										    MODEL_CHANNELSGROUP_URI, &sgroupuri,
										    MODEL_CHANNELSGROUP_BREGEX, &sbregex,
										    MODEL_CHANNELSGROUP_EREGEX, &seregex, -1);
								
							channels_group_infos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname, (gchar*)sgroupuri);
							freetuxtv_channels_group_infos_set_regex (channels_group_infos, (gchar*)sbregex, (gchar*)seregex);
							channels_list_add_channels_group (app, channels_group_infos, &dbsync, &error);
							}						
						}					
					}
					
					iterator = g_list_next(iterator);
				}
				g_list_free (list);
				
				break;
			case 1:
				
				widget = (GtkWidget *) gtk_builder_get_object (app->gui,
								       "dialogaddgroup_name");
				sgroupname = gtk_entry_get_text(GTK_ENTRY(widget));
				
				widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									       "dialogaddgroup_uri");
				sgroupuri = gtk_entry_get_text(GTK_ENTRY(widget));
				
				widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									       "dialogaddgroup_bregex");
				sbregex = gtk_entry_get_text(GTK_ENTRY(widget));
				
				widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									       "dialogaddgroup_eregex");
				seregex = gtk_entry_get_text(GTK_ENTRY(widget));			
				
				/* Check the field */
				if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
					errmsg = g_strdup_printf(_("Please enter the group's name !"));
				}
				if(g_ascii_strcasecmp(sgroupuri,"") == 0 && errmsg == NULL){
					errmsg = g_strdup_printf(_("Please enter the group's URI !"));
				}
				
				if(errmsg == NULL){				
					if(g_ascii_strcasecmp(sbregex,"") == 0 ){
						sbregex = NULL;
					}
					if(g_ascii_strcasecmp(seregex,"") == 0 ){
						seregex = NULL;
					}
					
					FreetuxTVChannelsGroupInfos *channels_group_infos;		
					channels_group_infos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname, (gchar*)sgroupuri);
					freetuxtv_channels_group_infos_set_regex (channels_group_infos, (gchar*)sbregex, (gchar*)seregex);
					
					channels_list_add_channels_group (app, channels_group_infos, &dbsync, &error);
				}
				
				break;
			}
		}
		dbsync_close_db(&dbsync);

		if(errmsg != NULL){
			windowmain_show_error (app, errmsg);
			g_free(errmsg);
		}
		if(error != NULL){
			windowmain_show_gerror (app, error);
			g_error_free (error);
		}

	}
	if (response_id == GTK_RESPONSE_CLOSE){
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
on_dialogaddgroup_buttonrefresh_clicked (GtkButton *button,
					 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	GtkWidget* widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "dialogaddgroup_entrychannelsgroupfile");
	gchar *url;
	gchar *dst_file;
	url = (gchar*)gtk_entry_get_text (GTK_ENTRY(widget));
	dst_file = g_strdup_printf("%s/FreetuxTV/cache/channels_groups.dat",
				   g_get_user_config_dir());
	freetuxtv_fileutils_get_file (url, dst_file, &error);
	
	if(error == NULL){
		load_model_channels_group_from_file(app, &error);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}

	g_free(dst_file);
}

static void
on_dialoggroupproperties_response (GtkDialog *dialog,
				   gint response_id,
				   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gchar *errmsg = NULL;
	
	if(response_id == GTK_RESPONSE_APPLY){
		GtkWidget *groupname;
		GtkWidget *groupuri;
		GtkWidget *bregex;
		GtkWidget *eregex;
		
		const gchar *sgroupname;
		const gchar *sgroupuri;
		const gchar *sbregex;
		const gchar *seregex;
		
		groupname = (GtkWidget *) gtk_builder_get_object (app->gui,
								  "dialoggroupproperties_name");
		groupuri = (GtkWidget *) gtk_builder_get_object (app->gui,
								 "dialoggroupproperties_uri");
		bregex = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties_bregex");
		eregex = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties_eregex");
		
		sgroupname = gtk_entry_get_text(GTK_ENTRY(groupname));
		sgroupuri = gtk_entry_get_text(GTK_ENTRY(groupuri));
		sbregex = gtk_entry_get_text(GTK_ENTRY(bregex));
		seregex = gtk_entry_get_text(GTK_ENTRY(eregex));
		

		/* Check the field */
		if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
			errmsg = g_strdup_printf(_("Please enter the group's name !"));
		}
		if(g_ascii_strcasecmp(sgroupuri,"") == 0 && errmsg == NULL){
			errmsg = g_strdup_printf(_("Please enter the group's URI !"));
		}
		
		if(errmsg != NULL){
			windowmain_show_error (app, errmsg);
		}else{
			
			if(g_ascii_strcasecmp(sbregex,"") == 0 ){
				bregex = NULL;
			}
			if(g_ascii_strcasecmp(seregex,"") == 0 ){
				seregex = NULL;
			}

			FreetuxTVChannelsGroupInfos *channels_group_infos;
			GtkTreePath *path_group;
			channels_group_infos = (FreetuxTVChannelsGroupInfos *)g_object_get_data (G_OBJECT(dialog), "ChannelsGroup");
			path_group = (GtkTreePath *)g_object_get_data (G_OBJECT(dialog), "PathChannelsGroup");
			
			freetuxtv_channels_group_infos_set_name (channels_group_infos, (gchar*)sgroupname);
			freetuxtv_channels_group_infos_set_uri (channels_group_infos, (gchar*)sgroupuri);
			freetuxtv_channels_group_infos_set_regex (channels_group_infos, (gchar*)sbregex, (gchar*)seregex);
			
			channels_list_update_channels_group (app, path_group, channels_group_infos);

			g_object_set_data (G_OBJECT(dialog), "ChannelsGroup", NULL);
			g_object_set_data (G_OBJECT(dialog), "PathChannelsGroup", NULL);
			
			gtk_widget_hide(GTK_WIDGET(dialog));	
		}
		
		g_free(errmsg);
	}
	if (response_id == GTK_RESPONSE_CANCEL){
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
on_aboutdialog_response (GtkDialog *dialog,
			 gint response_id,
			 gpointer user_data)
{
	gtk_widget_hide(GTK_WIDGET(dialog));
}


static void
dialogpreferences_update_view(FreetuxTVApp *app)
{
	GtkWidget *textbuffer;
	textbuffer = (GtkWidget *) gtk_builder_get_object (app->gui,
							   "textbuffer_previewtranscoding");
	gchar *text;
	text = get_recording_options(app, "%filename%", TRUE, NULL);
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(textbuffer), text, -1);
	g_free(text);
}
