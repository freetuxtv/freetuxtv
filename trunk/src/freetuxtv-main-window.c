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

#include "freetuxtv-main-window.h"

#include "freetuxtv-channels-list-window.h"
#include "freetuxtv-logos-list-window.h"

G_DEFINE_TYPE (FreetuxTVMainWindow, freetuxtv_main_window, GTK_TYPE_WINDOW);

static void
on_mainwindow_destroy (GtkWidget *widget, gpointer *data);

typedef struct _MenuBarAction
{
	FreetuxTVMainWindow *window;
	enum {
		FREETUXTV_EXIT               = 0,
		FREETUXTV_SHOW_CHANNELS_LIST = 1,
		FREETUXTV_SHOW_LOGOS_LIST    = 2
	} action;
} MenuBarAction;

static void
on_menubar_activate (GtkWidget *widget, gpointer *data);

GtkWidget *
freetuxtv_main_window_new ()
{
	FreetuxTVMainWindow *self = NULL;
	self = gtk_type_new (freetuxtv_main_window_get_type ());

	/* Creation de la fenetre */
	gtk_window_set_title (GTK_WINDOW(self), "FreetuxTV");
	gtk_window_set_default_size (GTK_WINDOW(self), 1024, 600);
	gtk_window_set_position (GTK_WINDOW(self), GTK_WIN_POS_CENTER);
	gtk_window_maximize (GTK_WINDOW(self));

	GError *error;
	GdkPixbuf *icon;
	icon = gdk_pixbuf_new_from_file (FREETUXTV_DIR "/images/freetuxtv.ico", 
					 &error);

	gtk_window_set_icon (GTK_WINDOW(self), icon);

	g_signal_connect(G_OBJECT(self), "destroy",
			 G_CALLBACK(on_mainwindow_destroy), NULL);
	
	/* Separateur entre menu / panneau d'onglet et fenetre video */
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(self), vbox);
	
	/* Création de la barre de menu */
	GtkWidget *menubar;
	GtkWidget *menu;
	GtkWidget *menuitem;
	MenuBarAction *menubaraction;

	menubar = gtk_menu_bar_new();
	gtk_box_pack_start (GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	
	/* Menu Fichier */
	menu = gtk_menu_new ();
	
	menuitem = gtk_menu_item_new_with_label("Quitter");
	menubaraction = g_new0(MenuBarAction, 1);
	menubaraction->window = self;
	menubaraction->action = FREETUXTV_EXIT;
	g_signal_connect (G_OBJECT (menuitem), "activate",
			  G_CALLBACK(on_menubar_activate), menubaraction);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	
	menuitem = gtk_menu_item_new_with_label ("Fichier");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(menuitem), menu);
	gtk_menu_shell_append (GTK_MENU_SHELL(menubar), menuitem);
	
	/* Menu Options */
	menu = gtk_menu_new ();
	
	menuitem = gtk_menu_item_new_with_label ("Liste des chaînes");
	menubaraction = g_new0(MenuBarAction, 1);
	menubaraction->window = self;
	menubaraction->action = FREETUXTV_SHOW_CHANNELS_LIST;
	g_signal_connect (G_OBJECT (menuitem), "activate",
			  G_CALLBACK(on_menubar_activate), menubaraction);
	gtk_menu_shell_append (GTK_MENU_SHELL(menu), menuitem);

	menuitem = gtk_menu_item_new_with_label ("Liste des logos");
	menubaraction = g_new0(MenuBarAction, 1);
	menubaraction->window = self;
	menubaraction->action = FREETUXTV_SHOW_LOGOS_LIST;
	g_signal_connect (G_OBJECT (menuitem), "activate",
			  G_CALLBACK(on_menubar_activate), menubaraction);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

	menuitem = gtk_menu_item_new_with_label("Options");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(menuitem), menu);
	gtk_menu_shell_append (GTK_MENU_SHELL(menubar), menuitem);

	/* Menu Aide */
	menu = gtk_menu_new ();
	
	menuitem = gtk_menu_item_new_with_label("A propos de...");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	
	menuitem = gtk_menu_item_new_with_label ("Aide");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(menuitem), menu);
	gtk_menu_shell_append (GTK_MENU_SHELL(menubar), menuitem);
	
	/* Separateur entre menu / panneau d'onglet et fenetre video */
	GtkWidget *hbox;
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

	/* Panneau d'onglets */
	GtkWidget *notebook;
	notebook = gtk_notebook_new ();
	gtk_box_pack_start (GTK_BOX(hbox), notebook, FALSE, FALSE, 0);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook), GTK_POS_LEFT);
	
	GtkWidget *tablabel;
	GtkWidget *tabcontent;

	/* Onglet "Chaînes" */
	tablabel = gtk_label_new ("Chaînes");
	gtk_label_set_angle (GTK_LABEL(tablabel), 90.00);
	self->channelslist = FREETUXTV_CHANNELS_LIST(freetuxtv_channels_list_new());
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), 
				  GTK_WIDGET(self->channelslist), tablabel);
	
	/* Onglet "En cours" */
	tablabel = gtk_label_new ("En cours");
	gtk_label_set_angle (GTK_LABEL(tablabel), 90.00);
	tabcontent = gtk_label_new ("Bienvenue dans FreetuxTV ! Enjoy !");
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), tabcontent, tablabel);
	
	/* Cadre du player */
	self->player = FREETUXTV_PLAYER(freetuxtv_player_new ());
	gtk_box_pack_start (GTK_BOX(hbox), GTK_WIDGET(self->player), 
			    TRUE, TRUE, 0);
	
	/* Barre de status */
	self->statusbar = GTK_STATUSBAR(gtk_statusbar_new ());
	gtk_statusbar_set_has_resize_grip (self->statusbar, TRUE);
	gtk_statusbar_push (self->statusbar, 
			    gtk_statusbar_get_context_id(self->statusbar, 
							 "HelloMsg"),
			    "Bienvenue dans FreetuxTV");
	gtk_box_pack_end (GTK_BOX (vbox), GTK_WIDGET(self->statusbar), 
			  FALSE, FALSE, 3);

	gtk_widget_show_all (GTK_WIDGET(self));

	return GTK_WIDGET (self);
}

FreetuxTVMainWindow *
freetuxtv_main_window_get_from_widget (GtkWidget *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WIDGET(self), NULL);

	if(FREETUXTV_IS_MAIN_WINDOW(self)){
		return FREETUXTV_MAIN_WINDOW(self);
	}else{
		return freetuxtv_main_window_get_from_widget (gtk_widget_get_parent(self));
	}
}

void freetuxtv_main_window_play_channel (FreetuxTVMainWindow *self, 
					 FreetuxTVChannel *channel)
{
	self->current_channel=channel;
	
	gchar *text;
	gtk_statusbar_pop (self->statusbar,
			   gtk_statusbar_get_context_id(self->statusbar, 
							"PlayChannelMsg"));
	text = g_strconcat ("En cours de lecture : ", channel->name, NULL);
	gtk_statusbar_push (self->statusbar,
			    gtk_statusbar_get_context_id(self->statusbar, 
							 "PlayChannelMsg"), 
			    text);
	g_free(text);

	freetuxtv_player_play (self->player, channel->uri);
}

static void
on_mainwindow_destroy (GtkWidget *widget, gpointer *data)
{
	gtk_main_quit();
}

static void
on_menubar_activate (GtkWidget *widget, gpointer *data)
{
	GtkWidget *window;
	MenuBarAction *action = (MenuBarAction*)data;
	
	switch(action->action){
	case FREETUXTV_EXIT:
		gtk_main_quit();
		break;
	case FREETUXTV_SHOW_CHANNELS_LIST:
		window = freetuxtv_channels_list_window_new();
		gtk_window_set_modal (GTK_WINDOW(window),TRUE);
		gtk_window_set_transient_for (GTK_WINDOW(window),
					      GTK_WINDOW(action->window));
		gtk_window_set_position (GTK_WINDOW(window),
					 GTK_WIN_POS_CENTER_ON_PARENT);
		gtk_widget_show (window);
		break;
	case FREETUXTV_SHOW_LOGOS_LIST:
		window = freetuxtv_logos_list_window_new();
		gtk_window_set_modal (GTK_WINDOW(window),TRUE);
		gtk_window_set_transient_for (GTK_WINDOW(window),
					      GTK_WINDOW(action->window));
		gtk_window_set_position (GTK_WINDOW(window),
					 GTK_WIN_POS_CENTER_ON_PARENT);
		gtk_widget_show (window);
		break;
	}
}

static void
freetuxtv_main_window_init (FreetuxTVMainWindow *object)
{
	object->player = NULL;
	object->channelslist = NULL;
	object->statusbar = NULL;
	object->current_channel = NULL;
}

static void
freetuxtv_main_window_finalize (GObject *object)
{
	
}

static void
freetuxtv_main_window_class_init (FreetuxTVMainWindowClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_main_window_finalize;
}
