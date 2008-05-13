/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CALLBACKS_H
#define FREETUXTV_CALLBACKS_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>

void
on_windowmain_destroy (GtkWidget *widget, gpointer *data);

void
on_menuitemquit_activate (GtkWidget *widget, gpointer *data);

void
on_menuitemaboutdialog_activate (GtkWidget *widget, gpointer *data);

void
on_buttonclearfilter_clicked (GtkWidget *widget, GdkEventButton *event,
			      gpointer *data);

void
on_entryfilter_changed (GtkEntry *entry, gpointer user_data);

void
on_volumecontrol_value_changed (GtkRange *range, gpointer  user_data);

#endif /* FREETUXTV_CALLBACKS_H */
