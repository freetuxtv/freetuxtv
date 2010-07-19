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

/* Based on the rb-mmkeys-plugin.c file */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "lib-gmmkeys.h"
#include "string.h"

#define g_marshal_value_peek_boolean(v)  (v)->data[0].v_int
#define g_marshal_value_peek_char(v)     (v)->data[0].v_int
#define g_marshal_value_peek_uchar(v)    (v)->data[0].v_uint
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_uint(v)     (v)->data[0].v_uint
#define g_marshal_value_peek_long(v)     (v)->data[0].v_long
#define g_marshal_value_peek_ulong(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_uint64(v)   (v)->data[0].v_uint64
#define g_marshal_value_peek_enum(v)     (v)->data[0].v_long
#define g_marshal_value_peek_flags(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_float(v)    (v)->data[0].v_float
#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#define g_marshal_value_peek_string(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_param(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_boxed(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#define g_marshal_value_peek_object(v)   (v)->data[0].v_pointer

G_DEFINE_TYPE (GMMKeys, g_mmkeys, G_TYPE_OBJECT);

static void
g_marshal_VOID__STRING_STRING (GClosure     *closure,
                               GValue       *return_value,
                               guint         n_param_values,
                               const GValue *param_values,
                               gpointer      invocation_hint,
                               gpointer      marshal_data);

static void
media_player_key_pressed (DBusGProxy *proxy,
                          const gchar *application,
                          const gchar *key,
                          GMMKeys *data);

enum {
	MM_KEY_PRESSED,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

#ifdef HAVE_MMKEYS

static void
grab_mmkey (int key_code,
            GdkWindow *root);

static void
ungrab_mmkey (int key_code,
              GdkWindow *root);

static GdkFilterReturn
filter_mmkeys (GdkXEvent *xevent,
               GdkEvent *event,
               gpointer data);

static void
mmkeys_grab (GMMKeys *self, gboolean grab);

#endif // HAVE_MMKEYS

GMMKeys*
g_mmkeys_new (gchar *application, GLogFunc log_func)
{
	int idLogFuncHandler = -1;
	if(log_func){
		idLogFuncHandler = g_log_set_handler (GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_MASK,
                                              log_func, NULL);
	}

	g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Initialize\n");
	
	GMMKeys* self;
	self = G_MMKEYS (g_object_new (G_TYPE_MMKEYS, NULL));
	if(self){
		if(application == NULL){
			self->application = g_strdup("GMMKeys");
		}else{
			self->application = g_strdup(application);
		}
		self->idLogFuncHandler = idLogFuncHandler;
	}else{
		if(idLogFuncHandler >= 0){
			g_log_remove_handler(GMMKEYS_LOG_DOMAIN, idLogFuncHandler);
		}
	}
	return self;
}

void
g_mmkeys_activate (GMMKeys *self)
{
	DBusGConnection *bus;

	g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Activating media player keys\n");

	bus = dbus_g_bus_get (DBUS_BUS_SESSION, NULL);
	if (self->grab_type == NONE && bus != NULL) {

		GError *error = NULL;

		self->proxy = dbus_g_proxy_new_for_name_owner (bus,
		                                               "org.gnome.SettingsDaemon",
		                                               "/org/gnome/SettingsDaemon/MediaKeys",
		                                               "org.gnome.SettingsDaemon.MediaKeys",
		                                               &error);
		if (self->proxy == NULL) {
			g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
			      "Unable to grab media player keys: %s\n", error->message);
			g_error_free (error);
		} else {
			dbus_g_proxy_call (self->proxy,
			                   "GrabMediaPlayerKeys", &error,
			                   G_TYPE_STRING, self->application,
			                   G_TYPE_UINT, 0,
			                   G_TYPE_INVALID,
			                   G_TYPE_INVALID);

			// if the method doesn't exist, try the old interface/path
			if (error != NULL &&
			    error->domain == DBUS_GERROR &&
			    error->code == DBUS_GERROR_UNKNOWN_METHOD) {
					g_clear_error (&error);
					g_object_unref (self->proxy);

					g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_INFO,
					      "Trying old dbus interface/path");
					self->proxy = dbus_g_proxy_new_for_name_owner (bus,
					                                               "org.gnome.SettingsDaemon",
					                                               "/org/gnome/SettingsDaemon",
					                                               "org.gnome.SettingsDaemon",
					                                               &error);
					if (self->proxy != NULL) {
						dbus_g_proxy_call (self->proxy,
						                   "GrabMediaPlayerKeys", &error,
						                   G_TYPE_STRING, self->application,
						                   G_TYPE_UINT, 0,
						                   G_TYPE_INVALID,
						                   G_TYPE_INVALID);
					}
				}

			if (error == NULL) {

				g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_INFO,
				      "Created dbus proxy for org.gnome.SettingsDaemon.MediaKeys; grabbing keys\n");
				dbus_g_object_register_marshaller (g_marshal_VOID__STRING_STRING,
				                                   G_TYPE_NONE, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INVALID);

				dbus_g_proxy_add_signal (self->proxy,
				                         "MediaPlayerKeyPressed",
				                         G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INVALID);

				dbus_g_proxy_connect_signal (self->proxy,
				                             "MediaPlayerKeyPressed",
				                             G_CALLBACK (media_player_key_pressed),
				                             self, NULL);

				self->grab_type = SETTINGS_DAEMON;
			} else {
				g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
				      "Unable to grab media player keys: %s\n", error->message);
				g_error_free (error);
			}
		}
	} else {
		g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_WARNING,
		      "Couldn't get dbus session bus\n");
	}
#ifdef HAVE_MMKEYS
	if (self->grab_type == NONE) {
		g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		      "Attempting old-style key grabs\n");
		mmkeys_grab (self, TRUE);
		self->grab_type = X_KEY_GRAB;
	}
#endif // HAVE_MMKEYS
}

void
g_mmkeys_deactivate (GMMKeys *self)
{
	g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Deactivating media player keys\n");
	if (self->proxy != NULL) {
		GError *error = NULL;

		if (self->grab_type == SETTINGS_DAEMON) {
			dbus_g_proxy_call (self->proxy,
			                   "ReleaseMediaPlayerKeys", &error,
			                   G_TYPE_STRING, self->application,
			                   G_TYPE_INVALID, G_TYPE_INVALID);
			if (error != NULL) {
				g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
				      "Could not release media player keys: %s\n", error->message);
				g_error_free (error);
			}
			self->grab_type = NONE;
		}

		g_object_unref (self->proxy);
		self->proxy = NULL;
	}
#ifdef HAVE_MMKEYS
	if (self->grab_type == X_KEY_GRAB) {
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		      "Undoing old-style key grabs\n");
		mmkeys_grab (self, FALSE);
		self->grab_type = NONE;
	}
#endif // HAVE_MMKEYS
}


static void
media_player_key_pressed (DBusGProxy *proxy,
                          const gchar *application,
                          const gchar *key,
                          GMMKeys *data)
{
	g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
	      "Got media key '%s' for application '%s'\n",
	      key, application);

	if (strcmp (application, data->application))
		return;

	if (strcmp (key, "Play") == 0) {		
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0, GMMKEYS_BUTTON_PLAY
		               );
	} else if (strcmp (key, "Pause") == 0) {			
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0, GMMKEYS_BUTTON_PAUSE
		               );
	} else if (strcmp (key, "Stop") == 0) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0,GMMKEYS_BUTTON_STOP
		               );
	} else if (strcmp (key, "Previous") == 0) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0,GMMKEYS_BUTTON_PREV
		               );
	} else if (strcmp (key, "Next") == 0) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0,GMMKEYS_BUTTON_NEXT
		               );
	}
}

#ifdef HAVE_MMKEYS

static void
grab_mmkey (int key_code,
            GdkWindow *root)
{
	gdk_error_trap_push ();

	XGrabKey (GDK_DISPLAY (), key_code,
	          0,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);
	XGrabKey (GDK_DISPLAY (), key_code,
	          Mod2Mask,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);
	XGrabKey (GDK_DISPLAY (), key_code,
	          Mod5Mask,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);
	XGrabKey (GDK_DISPLAY (), key_code,
	          LockMask,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);
	XGrabKey (GDK_DISPLAY (), key_code,
	          Mod2Mask | Mod5Mask,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);
	XGrabKey (GDK_DISPLAY (), key_code,
	          Mod2Mask | LockMask,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);
	XGrabKey (GDK_DISPLAY (), key_code,
	          Mod5Mask | LockMask,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);
	XGrabKey (GDK_DISPLAY (), key_code,
	          Mod2Mask | Mod5Mask | LockMask,
	          GDK_WINDOW_XID (root), True,
	          GrabModeAsync, GrabModeAsync);

	gdk_flush ();
	if (gdk_error_trap_pop ()) {
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
		      "Error grabbing key\n");
	}
}

static void
ungrab_mmkey (int key_code,
              GdkWindow *root)
{
	gdk_error_trap_push ();

	XUngrabKey (GDK_DISPLAY (), key_code, 0, GDK_WINDOW_XID (root));
	XUngrabKey (GDK_DISPLAY (), key_code, Mod2Mask, GDK_WINDOW_XID (root));
	XUngrabKey (GDK_DISPLAY (), key_code, Mod5Mask, GDK_WINDOW_XID (root));
	XUngrabKey (GDK_DISPLAY (), key_code, LockMask, GDK_WINDOW_XID (root));
	XUngrabKey (GDK_DISPLAY (), key_code, Mod2Mask | Mod5Mask, GDK_WINDOW_XID (root));
	XUngrabKey (GDK_DISPLAY (), key_code, Mod2Mask | LockMask, GDK_WINDOW_XID (root));
	XUngrabKey (GDK_DISPLAY (), key_code, Mod5Mask | LockMask, GDK_WINDOW_XID (root));
	XUngrabKey (GDK_DISPLAY (), key_code, Mod2Mask | Mod5Mask | LockMask, GDK_WINDOW_XID (root));

	gdk_flush ();
	if (gdk_error_trap_pop ()) {
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
		      "Error grabbing key\n");
	}
}


static GdkFilterReturn
filter_mmkeys (GdkXEvent *xevent,
               GdkEvent *event,
               gpointer data)
{
	XEvent *xev;
	XKeyEvent *key;
	GMMKeys *self;
	xev = (XEvent *) xevent;
	if (xev->type != KeyPress) {
		return GDK_FILTER_CONTINUE;
	}

	key = (XKeyEvent *) xevent;

	self = (GMMKeys *)data;

	if (XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioPlay) == key->keycode) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0, GMMKEYS_BUTTON_PLAY
		               );
		return GDK_FILTER_REMOVE;
	} else if (XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioPause) == key->keycode) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0, GMMKEYS_BUTTON_PAUSE
		               );
		return GDK_FILTER_REMOVE;
	} else if (XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioStop) == key->keycode) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0, GMMKEYS_BUTTON_STOP
		               );
		return GDK_FILTER_REMOVE;
	} else if (XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioPrev) == key->keycode) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0, GMMKEYS_BUTTON_PREV
		               );
		return GDK_FILTER_REMOVE;
	} else if (XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioNext) == key->keycode) {	
		g_signal_emit (G_OBJECT (data),
		               signals [MM_KEY_PRESSED],
		               0, GMMKEYS_BUTTON_NEXT
		               );
		return GDK_FILTER_REMOVE;
	} else {
		return GDK_FILTER_CONTINUE;
	}
}

static void
mmkeys_grab (GMMKeys *self, gboolean grab)
{
	gint keycodes[] = {0, 0, 0, 0, 0};
	GdkDisplay *display;
	GdkScreen *screen;
	GdkWindow *root;
	guint i, j;

	keycodes[0] = XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioPlay);
	keycodes[1] = XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioStop);
	keycodes[2] = XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioPrev);
	keycodes[3] = XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioNext);
	keycodes[4] = XKeysymToKeycode (GDK_DISPLAY (), XF86XK_AudioPause);

	display = gdk_display_get_default ();

	for (i = 0; i < gdk_display_get_n_screens (display); i++) {
		screen = gdk_display_get_screen (display, i);

		if (screen != NULL) {
			root = gdk_screen_get_root_window (screen);

			for (j = 0; j < G_N_ELEMENTS (keycodes) ; j++) {
				if (keycodes[j] != 0) {
					if (grab)
						grab_mmkey (keycodes[j], root);
					else
						ungrab_mmkey (keycodes[j], root);
				}
			}

			if (grab)
				gdk_window_add_filter (root, filter_mmkeys,
				                       (gpointer) self);
			else
				gdk_window_remove_filter (root, filter_mmkeys,
				                          (gpointer) self);
		}
	}
}

#endif

static void
g_marshal_VOID__STRING_STRING (GClosure     *closure,
                               GValue       *return_value,
                               guint         n_param_values,
                               const GValue *param_values,
                               gpointer      invocation_hint,
                               gpointer      marshal_data)
{
	typedef void (*GMarshalFunc_VOID__STRING_STRING) (gpointer     data1,
	                                                  gpointer     arg_1,
	                                                  gpointer     arg_2,
	                                                  gpointer     data2);
	register GMarshalFunc_VOID__STRING_STRING callback;
	register GCClosure *cc = (GCClosure*) closure;
	register gpointer data1, data2;

	g_return_if_fail (n_param_values == 3);

	if (G_CCLOSURE_SWAP_DATA (closure)) {
		data1 = closure->data;
		data2 = g_value_peek_pointer (param_values + 0);
	}else{
		data1 = g_value_peek_pointer (param_values + 0);
		data2 = closure->data;
	}
	callback = (GMarshalFunc_VOID__STRING_STRING) (marshal_data ? marshal_data : cc->callback);

	callback (data1,
	          g_marshal_value_peek_string (param_values + 1),
	          g_marshal_value_peek_string (param_values + 2),
	          data2);
}

static void
g_mmkeys_finalize (GObject *object)
{
	GMMKeys *self = G_MMKEYS(object);
	
	g_log(GMMKEYS_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Finalize\n");
	if(self->idLogFuncHandler >= 0){
		g_log_remove_handler(GMMKEYS_LOG_DOMAIN, self->idLogFuncHandler);
	}
}

static void
g_mmkeys_init (GMMKeys *object)
{
	object->idLogFuncHandler = -1;
}

static void
g_mmkeys_class_init (GMMKeysClass *klass)
{
	GObjectClass *object_class;

	object_class = (GObjectClass*) klass;

	object_class->finalize = g_mmkeys_finalize;

	signals[MM_KEY_PRESSED] =
		g_signal_new ("mm_key_pressed",
		              G_TYPE_FROM_CLASS (klass),
		              G_SIGNAL_RUN_LAST,
		              0,
		              NULL, NULL,
		              g_cclosure_marshal_VOID__INT,
		              G_TYPE_NONE, 1, G_TYPE_INT);
}