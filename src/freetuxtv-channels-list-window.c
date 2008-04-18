/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-channels-list-window.h"

G_DEFINE_TYPE (FreetuxTVChannelsListWindow, freetuxtv_channels_list_window, GTK_TYPE_WINDOW);


GtkWidget *
freetuxtv_channels_list_window_new ()
{

	

	FreetuxTVChannelsListWindow *self = NULL;
	self = gtk_type_new (freetuxtv_channels_list_window_get_type ());

	gtk_window_set_title(GTK_WINDOW(self), "Gestion des chaines");
	gtk_window_set_default_size (GTK_WINDOW(self), 640, 480);
	gtk_window_set_position(GTK_WINDOW(self),GTK_WIN_POS_CENTER);	
	
	GtkWidget *p_main_box = NULL;
	
	GtkWidget* label = gtk_label_new ("Gestion des chaines");

	/* Creation du conteneur principal */
	p_main_box = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (self), p_main_box);

	/* Creation du bouton "Quitter" */
	{
	  GtkWidget *closeButton = NULL;

	  closeButton = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
	  g_signal_connect (G_OBJECT (closeButton), "clicked", G_CALLBACK (on_closeButton_clicked), NULL);
	  gtk_box_pack_start (GTK_BOX (p_main_box), closeButton, FALSE, FALSE, 0);
	}

	

	/* Affichage des widgets */
	gtk_widget_show_all (GTK_WIDGET(self));
	
	return GTK_WIDGET(self);
}

static void
on_closeButton_clicked(GtkWidget *widget, gpointer *data)
{
	FreetuxTVChannelsListWindow *self = NULL;
	self = gtk_type_new (freetuxtv_channels_list_window_get_type ());

	GtkWidget *pQuestion;

	pQuestion = gtk_message_dialog_new (GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Voulez vous vraiment\nquitter ce programme?");
	/* Affichage et attente d une reponse */
	switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
	{
	case GTK_RESPONSE_YES:
            /* OUI -> on quitte l application */
            
            break;
        case GTK_RESPONSE_NO:
            /* NON -> on detruit la boite de message */
            gtk_widget_destroy(pQuestion);
            break;
	}
	
}

static void
freetuxtv_channels_list_window_init (FreetuxTVChannelsListWindow *object)
{

}

static void
freetuxtv_channels_list_window_finalize (GObject *object)
{
	
}

static void
freetuxtv_channels_list_window_class_init (FreetuxTVChannelsListWindowClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_channels_list_window_finalize;
}
