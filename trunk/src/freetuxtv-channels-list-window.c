/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
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


	gtk_widget_show_all (GTK_WIDGET(self));

	return GTK_WIDGET(self);
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
