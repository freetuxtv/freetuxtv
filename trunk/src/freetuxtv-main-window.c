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

#include "freetuxtv-main-window.h"

G_DEFINE_TYPE (FreetuxTVMainWindow, freetuxtv_main_window, GTK_TYPE_WINDOW);

static void
freetuxtv_main_window_ondestroy (GtkWidget *widget, gpointer *data);

GtkWidget *
freetuxtv_main_window_new ()
{
	FreetuxTVMainWindow * main_window = NULL;
	main_window = gtk_type_new (freetuxtv_main_window_get_type ());

	/* Creation de la fenetre */
	gtk_window_set_title (GTK_WINDOW(main_window), "FreetuxTV");
	gtk_window_set_default_size (GTK_WINDOW(main_window), 1024, 600);
	g_signal_connect(G_OBJECT(main_window),
			 "destroy",
			 G_CALLBACK(freetuxtv_main_window_ondestroy),
			 NULL);
	
	/* Separateur entre menu / panneau d'onglet et fenetre video */
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(main_window), vbox);

	/* Création de la barre de menu */
	GtkWidget *menubar;
	GtkWidget *menu;
	GtkWidget *menuitem;
	menubar = gtk_menu_bar_new();
	gtk_box_pack_start (GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	
	/* Menu Fichier */
	menu = gtk_menu_new ();
	
	menuitem = gtk_menu_item_new_with_label("Quitter");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	
	menuitem = gtk_menu_item_new_with_label ("Fichier");
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
	
	GtkWidget * tablabel;
	GtkWidget * tabcontent;

	/* Onglet "Chaines" */
	tablabel = gtk_label_new ("Chaines");
	gtk_label_set_angle (GTK_LABEL(tablabel), 90.00);
	main_window->channelslist = FREETUXTV_CHANNELS_LIST(freetuxtv_channels_list_new());
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), GTK_WIDGET(main_window->channelslist), tablabel);
	
	/* Onglet "En cours" */
	tablabel = gtk_label_new ("En cours");
	gtk_label_set_angle (GTK_LABEL(tablabel), 90.00);
	tabcontent = gtk_label_new ("Bienvenue dans FreetuxTV ! Enjoy !");
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), tabcontent, tablabel);
	
	/* Cadre du player */
	main_window->player = FREETUXTV_PLAYER(freetuxtv_player_new ());
	gtk_box_pack_start (GTK_BOX(hbox), GTK_WIDGET(main_window->player), TRUE, TRUE, 0);
	
	gtk_widget_show_all (GTK_WIDGET(main_window));

	return GTK_WIDGET (main_window);
}

FreetuxTVMainWindow *
freetuxtv_channel_get_main_window (GtkWidget *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WIDGET(self), NULL);

	if(FREETUXTV_IS_MAIN_WINDOW(self)){
		return FREETUXTV_MAIN_WINDOW(self);
	}else{
		return freetuxtv_channel_get_main_window (gtk_widget_get_parent(self));
	}
}

static void
freetuxtv_main_window_ondestroy (GtkWidget *widget, gpointer *data)
{
	gtk_main_quit();
}

static void
freetuxtv_main_window_init (FreetuxTVMainWindow *object)
{
	object->player = NULL;
	object->channelslist = NULL;
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


/*

FreetuxTVMainWindow *
freetuxtv_main_window_new ()
{
	FreetuxTVMainWindow * self;

	self->mainwin = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	return self;
	
}
*/
