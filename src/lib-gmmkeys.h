/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

/* Based on the rb-mmkeys-plugin.h file */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef G_MMEYS_H
#define G_MMEYS_H

#include <dbus/dbus-glib.h>
#include <glib.h>
#include <glib-object.h>

#ifdef HAVE_MMKEYS
#include <X11/Xlib.h>
#include <X11/XF86keysym.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#endif /* HAVE_MMKEYS */

#define G_TYPE_MMKEYS		(g_mmkeys_get_type ())
#define G_MMKEYS(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_MMKEYS, GMMKeys))
#define G_MMKEYS_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_MMKEYS, GMMKeysClass))
#define G_IS_MMKEYS(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_MMKEYS))
#define G_IS_MMKEYS_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_MMKEYS))
#define G_MMKEYS_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_MMKEYS, GMMKeysClass))

typedef struct
{
	GObject parent;

	enum {
		NONE = 0,
		SETTINGS_DAEMON,
		X_KEY_GRAB
	} grab_type;
	
	gchar *application;
	DBusGProxy *proxy;
} GMMKeys;

typedef struct
{
	GObjectClass parent_class;
} GMMKeysClass;

typedef enum {
	GMMKEYS_BUTTON_PLAY,
	GMMKEYS_BUTTON_PAUSE,
	GMMKEYS_BUTTON_STOP,
	GMMKEYS_BUTTON_PREV,
	GMMKEYS_BUTTON_NEXT
} GMMKeysButton;

GMMKeys*
g_mmkeys_new (gchar *application);

void
g_mmkeys_activate (GMMKeys *self);

void
g_mmkeys_deactivate (GMMKeys *self);

#endif /* G_MMEYS_H */
