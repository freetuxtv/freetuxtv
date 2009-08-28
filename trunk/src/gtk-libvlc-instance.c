/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "gtk-libvlc-instance.h"

static void
gtk_libvlc_instance_class_init(GtkLibVLCInstanceClass *klass);
static void
gtk_libvlc_instance_init(GtkLibVLCInstance *object);
static void
gtk_libvlc_instance_dispose(GObject *object);
static void
gtk_libvlc_instance_finalize(GObject *object);

static void
on_vlc_exception(GtkLibVLCInstance *self, libvlc_exception_t * ex);

G_DEFINE_TYPE (GtkLibVLCInstance, gtk_libvlc_instance, G_TYPE_OBJECT);

/**
 * gtk_libvlc_instance_new:
 * @vlc_args: a string array containing the vlc command line arguments. Can be NULL.
 *
 * Create a new LibVLC instance for a #GtkLibVLCMediaPlayer. A #GtkLibVLCInstance is a singleton and should 
 * have only one instance for the entire program.
 *
 * Returns: a LibVLC instance. Should be freed when it is no longer needed with g_object_unref().
 *
 * Since: 0.1
 */
GtkLibVLCInstance*
gtk_libvlc_instance_new (const gchar* vlc_args[])
{
	GtkLibVLCInstance *self = NULL;
	self = g_object_new (GTK_TYPE_LIBVLC_INSTANCE, NULL);
	
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
	if(vlc_args == NULL){
		self->libvlc_instance = libvlc_new(0, NULL, &_vlcexcep);
	}else{
		self->libvlc_instance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), (gchar**)vlc_args, &_vlcexcep);
	}	
#else
	if(vlc_args == NULL){
		self->libvlc_instance = libvlc_new(0, NULL, &_vlcexcep);
	}else{
		self->libvlc_instance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args, &_vlcexcep);
	}
	on_vlc_exception(self, &_vlcexcep);
#endif
	
	return self;
}

static void
gtk_libvlc_instance_class_init (GtkLibVLCInstanceClass *klass)
{
	GObjectClass *object_class;
	
	object_class = G_OBJECT_CLASS (klass);
	
	object_class->dispose = gtk_libvlc_instance_dispose;
	object_class->finalize = gtk_libvlc_instance_finalize;
}

static void
gtk_libvlc_instance_init (GtkLibVLCInstance *object)
{
	object->libvlc_instance = NULL;
}

static void
gtk_libvlc_instance_dispose(GObject *object)
{
	 GtkLibVLCInstance *self;
	 g_return_if_fail(object != NULL);
	 g_return_if_fail(GTK_IS_LIBVLC_INSTANCE(object));	 
	 
	 self = GTK_LIBVLC_INSTANCE(object);

	 if(self->libvlc_instance != NULL){
#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
		 libvlc_destroy (self->libvlc_instance);
#else
		 libvlc_release (self->libvlc_instance);
#endif
		 self->libvlc_instance = NULL;
	 }

	 G_OBJECT_CLASS (gtk_libvlc_instance_parent_class)->dispose (object);
}

static void
gtk_libvlc_instance_finalize(GObject *object)
{
	 GtkLibVLCInstance *self;
	 g_return_if_fail(object != NULL);
	 g_return_if_fail(GTK_IS_LIBVLC_INSTANCE(object));	 
	 
	 self = GTK_LIBVLC_INSTANCE(object);

	 G_OBJECT_CLASS (gtk_libvlc_instance_parent_class)->finalize (object);
}

static void
on_vlc_exception(GtkLibVLCInstance *self, libvlc_exception_t * ex)
{
	if (libvlc_exception_raised (ex)){
		//#if LIBVLC_VERSION_MAJOR == 1 && ((LIBVLC_VERSION_MINOR == 0 && LIBVLC_VERSION_REVISION >= 2) || LIBVLC_VERSION_MINOR > 0)
		// g_printerr("libvlc-gtk error: %s\n", libvlc_errmsg());
		//#else
		g_printerr("libvlc-gtk error: %s\n", libvlc_exception_get_message(ex));
		//#endif
	}
}
