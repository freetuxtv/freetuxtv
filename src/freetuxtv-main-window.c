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

#include <gtk/gtk.h>
#include <malloc.h>

#include "freetuxtv-main-window.h"

FreetuxTVMainWindowClass *
freetuxtv_main_window_new ()
{
	FreetuxTVMainWindowClass * self;
	self = (FreetuxTVMainWindowClass *)malloc(sizeof(FreetuxTVMainWindowClass *));
	/* Creation de la fenetre */
	self->mainwin = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(self->mainwin), "FreetuxTV");
	gtk_window_set_default_size (GTK_WINDOW(self->mainwin), 800, 600);
	
	/* Separateur entre menu / panneau d'onglet et fenetre video */
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(self->mainwin), vbox);
	
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

	/* Onglet "En cours" */
	GtkWidget * tablabel;
	GtkWidget * tabcontent;
	tablabel = gtk_label_new ("En cours");
	gtk_label_set_angle (GTK_LABEL(tablabel), 90.00);
	tabcontent = gtk_label_new ("Bienvenu dans FreetuxTV ! Enjoy !");
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), tabcontent, tablabel);
	
	/* Onglet "Chaines"  */
	tablabel = gtk_label_new ("Chaines");
	gtk_label_set_angle (GTK_LABEL(tablabel), 90.00);
	tabcontent = gtk_label_new ("Contexte");
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), tabcontent, tablabel);

	/* Cadre du player */
	self->player = freetuxtv_player_new ();
	gtk_box_pack_start (GTK_BOX(hbox), self->player->widget, TRUE, TRUE, 0);

	return self;
	
}
