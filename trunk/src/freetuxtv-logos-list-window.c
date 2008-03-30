/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-logos-list-window.h"

G_DEFINE_TYPE (FreetuxTVLogosListWindow, freetuxtv_logos_list_window, GTK_TYPE_WINDOW);

GtkWidget *
freetuxtv_logos_list_window_new ()
{
	FreetuxTVLogosListWindow *self = NULL;
	self = gtk_type_new (freetuxtv_logos_list_window_get_type ());


	gtk_widget_show_all (GTK_WIDGET(self));

	return GTK_WIDGET(self);
}

static void
freetuxtv_logos_list_window_init (FreetuxTVLogosListWindow *object)
{

}

static void
freetuxtv_logos_list_window_finalize (GObject *object)
{
	
}

static void
freetuxtv_logos_list_window_class_init (FreetuxTVLogosListWindowClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_logos_list_window_finalize;
}
