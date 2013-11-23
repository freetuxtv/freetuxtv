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
#include "gtk-libvlc-private.h"

typedef struct _GtkLibvlcInstancePrivate GtkLibvlcInstancePrivate;
struct _GtkLibvlcInstancePrivate
{
	libvlc_instance_t *libvlc_instance;

	int idLogFuncHandler;
};

#define GTK_LIBVLC_INSTANCE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_LIBVLC_INSTANCE, GtkLibvlcInstancePrivate))

// Macro to display the LIBVLC_VERSION
#define LIBVLC_VERSION(major, minor, revision) str(major) "." str(minor) "." str(revision)
#define str(s) #s

G_DEFINE_TYPE (GtkLibvlcInstance, gtk_libvlc_instance, G_TYPE_OBJECT);


GQuark gtk_libvlc_error = 0;

GQuark
gtk_libvlc_error_quark () {
	if (gtk_libvlc_error == 0){
		gtk_libvlc_error = g_quark_from_string("GTK_LIBVLC_ERROR");
	}
	return gtk_libvlc_error;
}

static void
gtk_libvlc_instance_init (GtkLibvlcInstance *object)
{
	GtkLibvlcInstancePrivate *priv;
	priv = GTK_LIBVLC_INSTANCE_PRIVATE(object);

	priv->libvlc_instance = NULL;
	priv->idLogFuncHandler = -1;
}

static void
gtk_libvlc_instance_finalize (GObject *object)
{
	GtkLibvlcInstance *self;
	g_return_if_fail(object != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_INSTANCE(object));

	self = GTK_LIBVLC_INSTANCE(object);

	GtkLibvlcInstancePrivate *priv;
	priv = GTK_LIBVLC_INSTANCE_PRIVATE(self);

	if(priv->libvlc_instance != NULL){
#ifdef LIBVLC_OLD_INSTANCE
		libvlc_destroy (priv->libvlc_instance);
#else
		libvlc_release (priv->libvlc_instance);
#endif // LIBVLC_OLD_INSTANCE
		priv->libvlc_instance = NULL;
	}

	if(priv->idLogFuncHandler >= 0){
		g_log_remove_handler(GTK_LIBVLC_LOG_DOMAIN, priv->idLogFuncHandler);
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

static gboolean
raise_error(GtkLibvlcInstance *self, GError** error, gpointer user_data)
{
	gboolean bRes = FALSE;
	const gchar *szErrMsg;
	
#ifdef LIBVLC_OLD_VLCEXCEPTION
	g_return_if_fail(user_data != NULL);
	libvlc_exception_t *ex = (libvlc_exception_t*)user_data;
	if (libvlc_exception_raised (ex)){
		bRes = TRUE;
		
		szErrMsg = libvlc_exception_get_message(ex);
		if(error != NULL){
			*error = g_error_new (GTK_LIBVLC_ERROR,
			    GTK_LIBVLC_ERROR_LIBVLC,
			    "%s", szErrMsg);
		}
	}
#else
	szErrMsg = libvlc_errmsg();
	if(szErrMsg){
		bRes = TRUE;
		if(error != NULL){
			*error = g_error_new (GTK_LIBVLC_ERROR,
			    GTK_LIBVLC_ERROR_LIBVLC,
			    "%s", szErrMsg);
		}
		libvlc_clearerr();
	}
#endif // LIBVLC_OLD_VLCEXCEPTION
	
	return bRes;
}

/**
 * gtk_libvlc_instance_new:
 * @vlc_args: a NULL terminated string array containing the vlc command line arguments. Can be NULL.
 * @log_func: pointer to a Glib log function to send internal libvlc-gtk message.
 * @error: a pointer to an error object that will be initialized if an error happend.
 * Should be freed when it is no longer needed with g_error_free(). Can be NULL.
 *
 * Create a new LibVLC instance for a #GtkLibVLCMediaPlayer. A #GtkLibVLCInstance is a singleton and should 
 * have only one instance for the entire program.
 *
 * Returns: a LibVLC instance. Should be freed when it is no longer needed with g_object_unref().
 *
 * Since: 0.1
 */
GtkLibvlcInstance*
gtk_libvlc_instance_new (const gchar* vlc_args[], GLogFunc log_func, GError** error)
{
	if(error != NULL){
		g_return_val_if_fail(*error == NULL, NULL);
	}

	GError* pError = NULL;

	gchar* szListOptions = NULL;

	int idLogFuncHandler = -1;
	if(log_func){
		idLogFuncHandler = g_log_set_handler (GTK_LIBVLC_LOG_DOMAIN, G_LOG_LEVEL_MASK,
                                              log_func, NULL);
	}
	
	GtkLibvlcInstance *self = NULL;
	self = g_object_new (GTK_TYPE_LIBVLC_INSTANCE, NULL);

	GtkLibvlcInstancePrivate *priv;
	priv = GTK_LIBVLC_INSTANCE_PRIVATE(self);

	priv->idLogFuncHandler = idLogFuncHandler;
	
#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	int iVlcArgsCount = 0;
	
	if(vlc_args){
		szListOptions = g_strjoinv(" ", (gchar**)vlc_args);
		iVlcArgsCount = g_strv_length((gchar**)vlc_args);
	}
	
	if(szListOptions){
		g_log(GTK_LIBVLC_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Using instance vlc with %d options [%s]\n", iVlcArgsCount, szListOptions);
		g_free(szListOptions);
		szListOptions = NULL;
	}

#ifdef LIBVLC_OLD_INSTANCE
	if(vlc_args == NULL){
		priv->libvlc_instance = libvlc_new(0, NULL, &_vlcexcep);
	}else{
		priv->libvlc_instance = libvlc_new(iVlcArgsCount, (gchar**)vlc_args, &_vlcexcep);
	}	
#else
#ifdef LIBVLC_OLD_VLCEXCEPTION
	if(vlc_args == NULL){
		priv->libvlc_instance = libvlc_new(0, NULL, &_vlcexcep);
	}else{
		priv->libvlc_instance = libvlc_new(iVlcArgsCount, vlc_args, &_vlcexcep);
	}
#else
	if(vlc_args == NULL){
		priv->libvlc_instance = libvlc_new(0, NULL);
	}else{
		priv->libvlc_instance = libvlc_new(iVlcArgsCount, vlc_args);
	}
#endif // LIBVLC_OLD_VLCEXCEPTION
#endif // LIBVLC_OLD_INSTANCE

#ifdef LIBVLC_OLD_VLCEXCEPTION
	raise_error(self, &pError, &_vlcexcep);
#else
	raise_error(self, &pError, NULL);	
#endif // LIBVLC_OLD_VLCEXCEPTION

	if(pError == NULL && !priv->libvlc_instance){
		pError = g_error_new (GTK_LIBVLC_ERROR,
		    GTK_LIBVLC_ERROR_LIBVLC,
		    "%s", "Cannot initialize the LibVLC");
	}
	
	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
		g_object_unref(self);
		self = NULL;
	}

	return self;
}

/**
 * gtk_libvlc_instance_get_libvlc_instance:
 * @pLibvlcInstance: a pointer to a #GtkLibvlcInstance object.
 * @error: a pointer to an error object that will be initialized if an error happend.
 * Should be freed when it is no longer needed with g_error_free(). Can be NULL.
 *
 * Return an handle that contain a pointer on a #libvlc_instance_t object 
 * which is attached to the #GtkLibvlcInstance object.
 *
 * Returns: a #LIBVLC_INSTANCE_HANDLE object that is a pointer to a 
 * #libvlc_instance_t object.
 *
 * Since: 0.1
 */ 
LIBVLC_INSTANCE_HANDLE
gtk_libvlc_instance_get_libvlc_instance(GtkLibvlcInstance* pLibvlcInstance, GError** error)
{
	g_return_val_if_fail(pLibvlcInstance != NULL, NULL);
	if(error != NULL){
		g_return_val_if_fail(*error == NULL, NULL);
	}

	LIBVLC_INSTANCE_HANDLE handle;
	
	GtkLibvlcInstancePrivate *priv;
	priv = GTK_LIBVLC_INSTANCE_PRIVATE(pLibvlcInstance);

	handle = priv->libvlc_instance;

	return handle;
}

/**
 * gtk_libvlc_get_libvlc_version:
 * @major: if not NULL, take the value of the major number of the LibVLC
 * version used at compilation time.
 * @minor: if not NULL, take the value of the minor number of the LibVLC
 * version used at compilation time.
 * @revision: if not NULL, take the value of the revision number of the LibVLC
 * version used at compilation time.
 *
 * Return a string that represent the version of the LibVLC
 * version used at compilation time.
 *
 * Returns: a string that represent the version of the LibVLC version.
 *
 * Since: 0.1
 */ 
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
