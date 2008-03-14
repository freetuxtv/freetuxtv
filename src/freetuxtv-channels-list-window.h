/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CHANNELS_LIST_WINDOW_H
#define FREETUXTV_CHANNELS_LIST_WINDOW_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define FREETUXTV_TYPE_CHANNELS_LIST_WINDOW            (freetuxtv_channels_list_window_get_type ())
#define FREETUXTV_CHANNELS_LIST_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNELS_LIST_WINDOW, FreetuxTVChannelsListWindow))
#define FREETUXTV_CHANNELS_LIST_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNELS_LIST_WINDOW, FreetuxTVChannelsListWindowClass))
#define FREETUXTV_IS_CHANNELS_LIST_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNELS_LIST_WINDOW))
#define FREETUXTV_IS_CHANNELS_LIST_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNELS_LIST_WINDOW))
#define FREETUXTV_CHANNELS_LIST_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNELS_LIST_WINDOW, FreetuxTVChannelsListWindowClass))

typedef struct _FreetuxTVChannelsListWindow FreetuxTVChannelsListWindow;
typedef struct _FreetuxTVChannelsListWindowClass FreetuxTVChannelsListWindowClass;

struct _FreetuxTVChannelsListWindow
{
	GtkWindow parent;

};

struct _FreetuxTVChannelsListWindowClass
{
	GtkWindowClass parent_class;
};

GType
freetuxtv_channels_list_window_get_type (void);

GtkWidget *
freetuxtv_channels_list_window_new ();

G_END_DECLS

#endif /* FREETUXTV_CHANNELS_LIST_WINDOW_H */
