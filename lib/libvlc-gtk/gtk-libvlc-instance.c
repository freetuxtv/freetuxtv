/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtk-libvlc-instance.h"

typedef struct _GtkLibvlcInstancePrivate GtkLibvlcInstancePrivate;
struct _GtkLibvlcInstancePrivate
{
	gchar* test;
};

#define GTK_LIBVLC_INSTANCE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_LIBVLC_INSTANCE, GtkLibvlcInstancePrivate))

// Macro to display the LIBVLC_VERSION
#define LIBVLC_VERSION(major, minor, revision) str(major) "." str(minor) "." str(revision)
#define str(s) #s

G_DEFINE_TYPE (GtkLibvlcInstance, gtk_libvlc_instance, G_TYPE_OBJECT);

static void
gtk_libvlc_instance_init (GtkLibvlcInstance *object)
{
	object->libvlc_instance = NULL;
}

static void
gtk_libvlc_instance_finalize (GObject *object)
{
	GtkLibvlcInstance *self;
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

	G_OBJECT_CLASS (gtk_libvlc_instance_parent_class)->finalize (object);
}

static void
gtk_libvlc_instance_class_init (GtkLibvlcInstanceClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkLibvlcInstancePrivate));

	object_class->finalize = gtk_libvlc_instance_finalize;
}

static void
on_vlc_exception(GtkLibvlcInstance *self, libvlc_exception_t * ex)
{
	if (libvlc_exception_raised (ex)){
		/*
#if LIBVLC_VERSION_MAJOR == 1 && ((LIBVLC_VERSION_MINOR == 0 && LIBVLC_VERSION_REVISION >= 2) || LIBVLC_VERSION_MINOR > 0)
		g_printerr("libvlc-gtk error: %s\n", libvlc_errmsg());
#else
		*/
		g_printerr("libvlc-gtk error: %s\n", libvlc_exception_get_message(ex));
		/*
#endif
		*/
	}
}

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
GtkLibvlcInstance*
gtk_libvlc_instance_new (const gchar* vlc_args[])
{
	GtkLibvlcInstance *self = NULL;
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

const gchar*
gtk_libvlc_get_libvlc_version (gint* major, gint *minor, gint *revision)
{
	if(major != NULL){
		*major = LIBVLC_VERSION_MAJOR;
	}
	if(minor != NULL){
		*minor = LIBVLC_VERSION_MINOR;
	}
	if(revision != NULL){
		*revision = LIBVLC_VERSION_REVISION;
	}
		
	return LIBVLC_VERSION(LIBVLC_VERSION_MAJOR, LIBVLC_VERSION_MINOR, LIBVLC_VERSION_REVISION);
}
