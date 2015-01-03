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

#include "gtk-libvlc-media-player.h"
#include "gtk-libvlc-private.h"

#include <gdk/gdkx.h>
#include <string.h>

typedef struct _GtkLibvlcMediaPlayerPrivate GtkLibvlcMediaPlayerPrivate;
struct _GtkLibvlcMediaPlayerPrivate
{
	gboolean initialized;

	GtkTreePath *current_media;
	gchar **current_options;

	gboolean is_media_parsed;
	gboolean play_next_at_end;
	gboolean loop;

#ifndef LIBVLC_DEPRECATED_PLAYLIST
	libvlc_media_player_t *libvlc_mediaplayer;
#endif // LIBVLC_DEPRECATED_PLAYLIST

	GtkWidget* pWigdetOriginalParent;
	GtkWidget* pWindowFullscreen;

	gint root_x;
	gint root_y;

	gboolean isModeFullscreen;

	GtkAccelGroup *pAccelGroup;
};

#define GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_LIBVLC_MEDIA_PLAYER, GtkLibvlcMediaPlayerPrivate))

G_DEFINE_TYPE (GtkLibvlcMediaPlayer, gtk_libvlc_media_player, GTK_TYPE_WIDGET);

enum {
   SIGNAL_EVENT_OCCURRED,
   NB_SIGNALS
};

static guint g_signals [NB_SIGNALS] = { 0 };

#ifndef LIBVLC_DEPRECATED_PLAYLIST
static void 
on_vlc_event(const libvlc_event_t *event, void *user_data);
#endif // LIBVLC_DEPRECATED_PLAYLIST

#ifndef LIBVLC_OLD_FULLSCREEN_MODE
static gboolean
on_buttonpress_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
#endif // LIBVLC_OLD_FULLSCREEN_MODE

static gboolean
raise_error(GtkLibvlcMediaPlayer *self, GError** error, gpointer user_data)
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

static void
gtk_libvlc_media_player_init (GtkLibvlcMediaPlayer *object)
{
	object->libvlc_instance = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(object);

#ifndef LIBVLC_DEPRECATED_PLAYLIST
	priv->libvlc_mediaplayer = NULL;
	priv->is_media_parsed = FALSE;
#endif // LIBVLC_DEPRECATED_PLAYLIST

	object->media_list = NULL;

	priv->initialized = FALSE;
	priv->play_next_at_end = TRUE;
	priv->loop = FALSE;

	priv->current_options = NULL;
	
#ifndef LIBVLC_OLD_FULLSCREEN_MODE
	priv->pWigdetOriginalParent = NULL;
	priv->pWindowFullscreen = NULL;
#endif // LIBVLC_OLD_FULLSCREEN_MODE
	
	priv->isModeFullscreen = FALSE;

	priv->pAccelGroup = NULL;
}

#if GTK_API_VERSION == 3

static GtkSizeRequestMode
gtk_libvlc_media_player_get_request_mode (GtkWidget *widget)
{
	return GTK_SIZE_REQUEST_HEIGHT_FOR_WIDTH;
}

static void
gtk_libvlc_media_player_get_preferred_width (
	GtkWidget *widget,
    gint *minimum_width,
    gint *natural_width)
{
	if(minimum_width){
		*minimum_width = 240;
	}
	if(natural_width){
		*natural_width = 640;
	}
}

static void
gtk_libvlc_media_player_get_preferred_height_for_width (
	GtkWidget *widget,
	gint width,
	gint *minimum_height,
	gint *natural_height)
{
	float ratio = 16/9;
	if(minimum_height){
		*minimum_height = 160;
	}
	if(natural_height){
		*natural_height = (int)(width * ratio);
	}
}

#else

static void
gtk_libvlc_media_player_size_request(GtkWidget *widget, GtkRequisition *requisition)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(widget));
	g_return_if_fail(requisition != NULL);

	requisition->width = 240;
	requisition->height = 160;
}

#endif

static void
gtk_libvlc_media_player_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(widget));
	g_return_if_fail(allocation != NULL);

	GdkWindow* pWindow = NULL;
	gboolean bIsRealized;

#if GTK_API_VERSION == 3
	gtk_widget_set_allocation(widget, allocation);
	pWindow = gtk_widget_get_window(widget);

	bIsRealized = gtk_widget_get_realized(widget);
#else
	widget->allocation = *allocation;
	pWindow = widget->window;

	bIsRealized = (GTK_WIDGET_REALIZED(widget));
#endif

	if(bIsRealized){
		gdk_window_move_resize(pWindow,
		                       allocation->x, allocation->y,
		                       allocation->width, allocation->height);
	}
}

static void
gtk_libvlc_media_player_realize(GtkWidget *widget)
{
	GtkLibvlcMediaPlayer *libvlc_mediaplayer = GTK_LIBVLC_MEDIA_PLAYER (widget);

	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(widget));

#if GTK_API_VERSION == 3
	gboolean bHasWindow = gtk_widget_get_has_window(widget);
#else
	gboolean bHasWindow = !GTK_WIDGET_NO_WINDOW (widget);
#endif

	if (!bHasWindow){
		GTK_WIDGET_CLASS (gtk_libvlc_media_player_parent_class)->realize (widget);
	}else{

		GdkWindow *window, *parent;
		GdkWindowAttr attributes;
		guint attributes_mask;
		GtkAllocation allocation;
#if GTK_API_VERSION == 3
		GtkStyleContext *context;
#else
		GtkStyle *style;
#endif

#if GTK_API_VERSION == 3
        gtk_widget_set_realized(widget, TRUE);
        gtk_widget_get_allocation(widget, &allocation);
#else
        GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);
        memcpy(&allocation, &(widget->allocation), sizeof(GtkAllocation));
#endif
		
#if GTK_API_VERSION == 3
		gtk_widget_set_realized (widget, TRUE);
		gtk_widget_get_allocation (widget, &allocation);
#else
		GtkStyle* pStyle;
		GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);
		allocation = widget->allocation;
		pStyle = widget->style;
#endif

		attributes.x = allocation.x;
		attributes.y = allocation.y;
		attributes.width = allocation.width;
		attributes.height = allocation.height;		
		attributes.wclass = GDK_INPUT_OUTPUT;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;
		attributes.visual = gtk_widget_get_visual (widget);
#if GTK_API_VERSION == 3
		attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;
        gtk_widget_set_has_window(widget, TRUE);
#else
		attributes.colormap = gtk_widget_get_colormap (widget);
		attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
        GTK_WIDGET_SET_FLAGS(widget, !GTK_NO_WINDOW);
#endif

		parent = gtk_widget_get_parent_window(widget);
		window = gdk_window_new(parent, &attributes, attributes_mask);
#if GTK_API_VERSION == 3
        gtk_widget_set_window(widget, window);
#else
        widget->window = window;
#endif
		
		gdk_window_set_user_data(window, libvlc_mediaplayer);

		// Setting the style will set the background to black by default
#if GTK_API_VERSION == 3
        gdk_window_set_background_pattern(window, NULL);
        context = gtk_widget_get_style_context(widget);
        gtk_style_context_set_background(context, window);
#else
        style = gtk_widget_get_style(widget);
        widget->style = gtk_style_attach(widget->style, widget->window);
        //gtk_style_set_background(style, window, GTK_STATE_NORMAL);
        //gdk_window_set_back_pixmap(window, NULL, TRUE);
#endif
	}
}

static void
gtk_libvlc_media_player_finalize (GObject *object)
{
	GtkLibvlcMediaPlayer *self;
	GError *error = NULL;

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	g_return_if_fail(object != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(object));

	self = GTK_LIBVLC_MEDIA_PLAYER(object);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	priv->play_next_at_end = FALSE;

	// Release the media player
#ifndef LIBVLC_DEPRECATED_PLAYLIST
	if(priv->libvlc_mediaplayer != NULL){
		// Detach events on the media player
		libvlc_event_manager_t *em;
#ifdef LIBVLC_OLD_VLCEXCEPTION
		em = libvlc_media_player_event_manager (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);

		libvlc_event_detach (em, libvlc_MediaPlayerNothingSpecial, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerOpening, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerBuffering, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerPlaying, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerPaused, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerStopped, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerEndReached, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
		libvlc_event_detach (em, libvlc_MediaPlayerEncounteredError, on_vlc_event, self, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);

		// Stop the current media
		libvlc_media_player_stop (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, &error, &_vlcexcep);
#else
		em = libvlc_media_player_event_manager (priv->libvlc_mediaplayer);
		raise_error(self, &error, NULL);

		libvlc_event_detach (em, libvlc_MediaPlayerNothingSpecial, on_vlc_event, self);
		raise_error(self, &error, NULL);
		libvlc_event_detach (em, libvlc_MediaPlayerOpening, on_vlc_event, self);
		raise_error(self, &error, NULL);
		libvlc_event_detach (em, libvlc_MediaPlayerBuffering, on_vlc_event, self);
		raise_error(self, &error, NULL);
		libvlc_event_detach (em, libvlc_MediaPlayerPlaying, on_vlc_event, self);
		raise_error(self, &error, NULL);
		libvlc_event_detach (em, libvlc_MediaPlayerPaused, on_vlc_event, self);
		raise_error(self, &error, NULL);
		libvlc_event_detach (em, libvlc_MediaPlayerStopped, on_vlc_event, self);
		raise_error(self, &error, NULL);
		libvlc_event_detach (em, libvlc_MediaPlayerEndReached, on_vlc_event, self);
		raise_error(self, &error, NULL);
		libvlc_event_detach (em, libvlc_MediaPlayerEncounteredError, on_vlc_event, self);
		raise_error(self, &error, NULL);

		// Stop the current media
		libvlc_media_player_stop (priv->libvlc_mediaplayer);
		raise_error(self, &error, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION

		libvlc_media_player_release (priv->libvlc_mediaplayer);
		priv->libvlc_mediaplayer = NULL;
	}
#endif // LIBVLC_DEPRECATED_PLAYLIST

	if(priv->current_options != NULL){
		g_strfreev (priv->current_options);
		priv->current_options = NULL;
	}

	// Free the media list
	gtk_libvlc_media_player_clear_media_list (self);

#ifndef LIBVLC_OLD_FULLSCREEN_MODE
	priv->pWigdetOriginalParent = NULL;
	if(priv->pWindowFullscreen){
		g_object_unref(priv->pWindowFullscreen);
		priv->pWindowFullscreen = NULL;
	}
#endif // LIBVLC_OLD_FULLSCREEN_MODE

	if(priv->pAccelGroup){
#if GTK_API_VERSION == 3
		g_object_unref (priv->pAccelGroup);
#else
		gtk_accel_group_unref (priv->pAccelGroup);
#endif
		priv->pAccelGroup = NULL;
	}

	G_OBJECT_CLASS (gtk_libvlc_media_player_parent_class)->finalize (object);

	if(error != NULL){
		g_error_free (error);
		error = NULL;
	}
}

static void
gtk_libvlc_media_player_class_init (GtkLibvlcMediaPlayerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass* parent_class = GTK_WIDGET_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkLibvlcMediaPlayerPrivate));

	object_class->finalize = gtk_libvlc_media_player_finalize;

	parent_class->realize = gtk_libvlc_media_player_realize;

#if GTK_API_VERSION == 3
	parent_class->get_request_mode = gtk_libvlc_media_player_get_request_mode;
	parent_class->get_preferred_width = gtk_libvlc_media_player_get_preferred_width;
	parent_class->get_preferred_height_for_width = gtk_libvlc_media_player_get_preferred_height_for_width;
#else
	parent_class->size_request = gtk_libvlc_media_player_size_request;
#endif
	parent_class->size_allocate = gtk_libvlc_media_player_size_allocate;
	
	g_signals [SIGNAL_EVENT_OCCURRED] = g_signal_new (
	    "event-occurred",
	    G_TYPE_FROM_CLASS (klass),
	    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	    G_STRUCT_OFFSET (GtkLibvlcMediaPlayerClass, event_occurred),
	    NULL, NULL,
	    g_cclosure_marshal_VOID__ENUM,
	    G_TYPE_NONE,
	    1,
	    G_TYPE_INT
	    );
}

static void
gtk_libvlc_media_player_initialize(GtkLibvlcMediaPlayer *self, GError **error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);

	libvlc_instance_t *libvlc_instance;
	libvlc_instance = (libvlc_instance_t *)
		gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, error);
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);


#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	gboolean bIsRealized = FALSE;
#if GTK_API_VERSION == 3
	bIsRealized = gtk_widget_get_realized(GTK_WIDGET(self));
#else
	bIsRealized = (GTK_WIDGET_FLAGS (self) & GTK_REALIZED) ? TRUE : FALSE;
#endif

	GdkWindow *pWindow;
#if GTK_API_VERSION == 3
	pWindow = gtk_widget_get_window(GTK_WIDGET(self));
#else
	pWindow = GTK_WIDGET(self)->window;
#endif

	if(priv->initialized == FALSE && bIsRealized){

#ifndef LIBVLC_OLD_FULLSCREEN_MODE
		gtk_widget_add_events (GTK_WIDGET(self), GDK_BUTTON_PRESS_MASK);
		g_signal_connect(self, "button-press-event", G_CALLBACK(on_buttonpress_event), NULL);
#endif // LIBVLC_OLD_FULLSCREEN_MODE

#if GTK_API_VERSION == 3
		XID xid = gdk_x11_window_get_xid(pWindow);
#else
		XID xid = gdk_x11_drawable_get_xid(pWindow);
#endif
		
#ifdef LIBVLC_DEPRECATED_PLAYLIST
		libvlc_video_set_parent(libvlc_instance, xid, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
#else  

#ifdef LIBVLC_OLD_VLCEXCEPTION
		priv->libvlc_mediaplayer = libvlc_media_player_new (libvlc_instance, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);

#ifdef LIBVLC_OLD_SET_DRAWABLE
		libvlc_media_player_set_drawable (priv->libvlc_mediaplayer, xid,
		                                  &_vlcexcep);
#else
		libvlc_media_player_set_xwindow (priv->libvlc_mediaplayer, xid,
		                                 &_vlcexcep);
#endif // LIBVLC_OLD_SET_DRAWABLE
		raise_error(self, error, &_vlcexcep);

		// Attach events on the media player
		libvlc_event_manager_t *em;
		em = libvlc_media_player_event_manager (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerNothingSpecial, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerOpening, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerBuffering, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerPlaying, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerPaused, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerStopped, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerEndReached, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_event_attach (em, libvlc_MediaPlayerEncounteredError, on_vlc_event, self, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		//libvlc_event_attach (em, libvlc_MediaPlayerPositionChanged, on_vlc_event, self, &_vlcexcep);
		//raise_error(self, error, &_vlcexcep);

#else
		priv->libvlc_mediaplayer = libvlc_media_player_new (libvlc_instance);
		raise_error(self, error, NULL);

		libvlc_media_player_set_xwindow (priv->libvlc_mediaplayer, xid);
		raise_error(self, error, NULL);

		// Attach events on the media player
		libvlc_event_manager_t *em;
		em = libvlc_media_player_event_manager (priv->libvlc_mediaplayer);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerNothingSpecial, on_vlc_event, self);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerOpening, on_vlc_event, self);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerBuffering, on_vlc_event, self);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerPlaying, on_vlc_event, self);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerPaused, on_vlc_event, self);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerStopped, on_vlc_event, self);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerEndReached, on_vlc_event, self);
		raise_error(self, error, NULL);
		libvlc_event_attach (em, libvlc_MediaPlayerEncounteredError, on_vlc_event, self);
		raise_error(self, error, NULL);
		//libvlc_event_attach (em, libvlc_MediaPlayerPositionChanged, on_vlc_event, self, &_vlcexcep);
		//raise_error(self, error, &_vlcexcep);

#endif // LIBVLC_OLD_VLCEXCEPTION

#endif // LIBVLC_DEPRECATED_PLAYLIST

		priv->initialized = TRUE;	
	}
}

static void
gtk_libvlc_media_player_play_media(GtkLibvlcMediaPlayer *self, GtkLibvlcMedia *media, gchar **options, GError** error)
{
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = (libvlc_instance_t *)
		gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, error);
	g_return_if_fail(libvlc_instance != NULL);

	GtkLibvlcMediaPlayerPrivate *priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif

	int nb_options;
	int nb_m_options = 0; // Number of media options
	int nb_mp_options = 0; // Number of media player options
	gchar** list_options = NULL;
	int i;
	gchar* szListOptions = NULL;

	gtk_libvlc_media_player_stop (self, error);

	// Delete options attached to the media player
	if(priv->current_options != options){
		if(priv->current_options != NULL){
			g_strfreev (priv->current_options);
			priv->current_options = NULL;
		}
		if(options == NULL){
			priv->current_options = NULL;
		}else{
			priv->current_options = g_strdupv (options);
		}
	}

	// Merge options in only one tab
	gchar** media_options;
	media_options = (gchar**)gtk_libvlc_media_get_options(media);
	if(media_options != NULL){
		nb_m_options += g_strv_length(media_options);
	}
	if(priv->current_options != NULL){
		nb_mp_options += g_strv_length(priv->current_options);
	}
	nb_options = nb_m_options + nb_mp_options;
	if(nb_options > 0){
		list_options = (gchar**)g_malloc((nb_options+1) * sizeof(gchar*));
		list_options[nb_options] = NULL;
		int step = 0;
		// We put first the media option
		for(i=0; i<nb_m_options; i++){
			list_options[step+i] = g_strdup(media_options[i]);
		}
		step += nb_m_options;
		// After we put the media player option
		for(i=0; i<nb_mp_options; i++){
			list_options[step+i] = g_strdup(priv->current_options[i]);
		}
		szListOptions = g_strjoinv(" ", list_options);
	}
	
	g_log(GTK_LIBVLC_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Playing %s\n", media->mrl);

	if(szListOptions){
		g_log(GTK_LIBVLC_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Using vlc options [%s]\n", szListOptions);
		g_free(szListOptions);
		szListOptions = NULL;
	}

	// Play the media
#ifdef LIBVLC_DEPRECATED_PLAYLIST
	if(libvlc_playlist_items_count (libvlc_instance, &_vlcexcep) > 0){
		libvlc_playlist_delete_item(libvlc_instance, 0, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
		libvlc_playlist_clear(libvlc_instance, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
	}
	if(list_options != NULL){
		libvlc_playlist_add_extended(libvlc_instance, media->mrl, NULL,
		                             nb_options, (const char**)list_options, &_vlcexcep);
	}else{
		libvlc_playlist_add (libvlc_instance, media->mrl, NULL, &_vlcexcep);		
	}
	raise_error(self, error, &_vlcexcep);

	if (libvlc_playlist_items_count (libvlc_instance, &_vlcexcep) > 0){;
		libvlc_playlist_play (libvlc_instance, -1, 0, 
		                      NULL, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
	}
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_media_t *m;
	m = libvlc_media_new (libvlc_instance, media->mrl, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);

	if(list_options != NULL){
		int i=0;
		for(i=0; i<nb_options; i++){
			libvlc_media_add_option(m, g_strdup(list_options[i]), &_vlcexcep);
			raise_error(self, error, &_vlcexcep);
		}
	}	

	libvlc_media_player_set_media (priv->libvlc_mediaplayer, m, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_media_release (m);

	// Event on the media
	libvlc_event_manager_t *em;
	m = libvlc_media_player_get_media(priv->libvlc_mediaplayer, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	em = libvlc_media_event_manager (m, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_event_attach (em, libvlc_MediaSubItemAdded, on_vlc_event, self, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);

	libvlc_media_player_play (priv->libvlc_mediaplayer, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
#else 
	libvlc_media_t *m;
	m = libvlc_media_new_location (libvlc_instance, media->mrl);
	raise_error(self, error, NULL);

	if(list_options != NULL){
		int i=0;
		for(i=0; i<nb_options; i++){
			libvlc_media_add_option(m, g_strdup(list_options[i]));
			raise_error(self, error, NULL);
		}
	}	

	libvlc_media_player_set_media (priv->libvlc_mediaplayer, m);
	raise_error(self, error, NULL);
	libvlc_media_release (m);

	// Event on the media
	libvlc_event_manager_t *em;
	m = libvlc_media_player_get_media(priv->libvlc_mediaplayer);
	raise_error(self, error, NULL);
	em = libvlc_media_event_manager (m);
	raise_error(self, error, NULL);
	libvlc_event_attach (em, libvlc_MediaSubItemAdded, on_vlc_event, self);
	raise_error(self, error, NULL);

	libvlc_media_player_play (priv->libvlc_mediaplayer);
	raise_error(self, error, NULL);
#endif

#endif

	// Free the options tab
	if(list_options){
		g_strfreev(list_options);
		list_options=NULL;
	}
}

static void
gtk_libvlc_media_player_set_current_path(GtkLibvlcMediaPlayer *self, GtkTreePath *path)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	if(priv->current_media != path){
		if(priv->current_media != NULL){
			gtk_tree_path_free(priv->current_media);	
		}
		if(path != NULL){
			priv->current_media = gtk_tree_path_copy(path);
		}else{
			priv->current_media = NULL;
		}
	}
}

#ifndef LIBVLC_DEPRECATED_PLAYLIST

static gboolean
idle_play_next_function(gpointer ptrdata){
	GtkLibvlcMediaPlayer *self;
	g_return_val_if_fail(ptrdata != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(ptrdata), FALSE);

	self = GTK_LIBVLC_MEDIA_PLAYER(ptrdata);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	GError* error = NULL;

	gtk_libvlc_media_player_play_next (self, priv->current_options, &error);

	if(error != NULL){
		g_error_free (error);
		error = NULL;
	}

	return FALSE;
}

static void 
on_vlc_event(const libvlc_event_t *event, void *user_data)
{
	//g_print("event %s\n", libvlc_event_type_name (event->type));

	GtkLibvlcMediaPlayer *self;
	g_return_if_fail(user_data != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(user_data));

	self = GTK_LIBVLC_MEDIA_PLAYER(user_data);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	if(event->type == libvlc_MediaSubItemAdded){		

		libvlc_media_t *new_m;
		new_m = event->u.media_subitem_added.new_child;

		if(new_m != NULL){
			
#ifdef LIBVLC_OLD_VLCEXCEPTION
			gchar* mrl = libvlc_media_get_mrl(new_m,
			                                  &_vlcexcep);
#else
			gchar* mrl = libvlc_media_get_mrl(new_m);
#endif // LIBVLC_OLD_VLCEXCEPTION
			
			GtkLibvlcMedia *new_media;
			new_media = gtk_libvlc_media_new(mrl);

			GtkTreeIter iter1;
			GtkTreeIter iter2;

			gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter1, priv->current_media);

			if(priv->is_media_parsed == FALSE){
				gtk_tree_store_append (self->media_list, &iter2, &iter1);
				gtk_tree_store_set (GTK_TREE_STORE(self->media_list), &iter2,
				                    GTK_LIBVLC_MODEL_MEDIA_COLUMN, new_media, -1);
			}
		}		
	}	

	if(event->type == libvlc_MediaPlayerEndReached){
		if(priv->play_next_at_end == TRUE){
			g_idle_add (idle_play_next_function, (gpointer)self);
		}
	}

	// Emit signal
	gboolean bEmitSignal = TRUE;
	GtkLibvlcEventType gtkevent;

	switch(event->type){
		case libvlc_MediaPlayerNothingSpecial:
			gtkevent = GTK_LIBVLC_EVENT_MP_NOTHINGSPECIAL;
			break;
		case libvlc_MediaPlayerOpening:
			gtkevent = GTK_LIBVLC_EVENT_MP_OPENING;
			break;
		case libvlc_MediaPlayerBuffering:
			gtkevent = GTK_LIBVLC_EVENT_MP_BUFFERING;
			break;
		case libvlc_MediaPlayerPlaying:
			gtkevent = GTK_LIBVLC_EVENT_MP_PLAYING;
			break;
		case libvlc_MediaPlayerPaused:
			gtkevent = GTK_LIBVLC_EVENT_MP_PAUSED;
			break;
		case libvlc_MediaPlayerStopped:
			gtkevent = GTK_LIBVLC_EVENT_MP_STOPPED;
			break;
		case libvlc_MediaPlayerForward:
			gtkevent = GTK_LIBVLC_EVENT_MP_FORWARD;
			break;
		case libvlc_MediaPlayerBackward:
			gtkevent = GTK_LIBVLC_EVENT_MP_BACKWARD;
			break;
		case libvlc_MediaPlayerEndReached:
			gtkevent = GTK_LIBVLC_EVENT_MP_ENDREACHED;
			break;
		case libvlc_MediaPlayerEncounteredError:
			gtkevent = GTK_LIBVLC_EVENT_MP_ENCOUTEREDERROR;
			break;
		default:
			// Signal not handled
			bEmitSignal = FALSE;
	}

	if(bEmitSignal){
		g_signal_emit (
			G_OBJECT (self),
			g_signals [SIGNAL_EVENT_OCCURRED],
			0, gtkevent
			);
	}
}

#endif // LIBVLC_DEPRECATED_PLAYLIST

#ifndef LIBVLC_OLD_FULLSCREEN_MODE
static gboolean
on_buttonpress_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	GtkLibvlcMediaPlayer* self = (GtkLibvlcMediaPlayer*) widget;

	GError* error = NULL;

	gtk_widget_grab_focus (widget);

	// If double click, toggle fullscreen mode
	if (event->type==GDK_2BUTTON_PRESS && event->button == 1) {
		gtk_libvlc_media_player_toggle_fullscreen (self, &error);
	}

	if(error){
		g_print("error: %s", error->message);
		g_error_free (error);
		error = NULL;
	}
	
	return TRUE;
}
#endif // LIBVLC_OLD_FULLSCREEN_MODE

GtkWidget *
gtk_libvlc_media_player_new (GtkLibvlcInstance* libvlc_instance, GError** error)
{
	g_return_val_if_fail(libvlc_instance != NULL, NULL);
	g_return_val_if_fail(GTK_IS_LIBVLC_INSTANCE(libvlc_instance), NULL);

	GtkLibvlcMediaPlayer *self = NULL;

#if GTK_API_VERSION == 3
	self = (GtkLibvlcMediaPlayer*)g_object_new (GTK_TYPE_LIBVLC_MEDIA_PLAYER, NULL);
#else
	self = (GtkLibvlcMediaPlayer*)gtk_type_new (GTK_TYPE_LIBVLC_MEDIA_PLAYER);
#endif
	
	self->libvlc_instance = libvlc_instance;
	g_object_ref(G_OBJECT(self->libvlc_instance));

	self->media_list = gtk_tree_store_new(GTK_LIBVLC_MODEL_NB_COLUMN, GTK_TYPE_LIBVLC_MEDIA);

#if GTK_API_VERSION == 3
	gtk_widget_set_can_focus(GTK_WIDGET(self), TRUE);
#else
	GTK_WIDGET_SET_FLAGS(GTK_WIDGET(self), GTK_CAN_FOCUS);
#endif

#if GTK_API_VERSION == 3
	// Set the background color
	GdkRGBA color;
	color.red = 0.0F;
	color.green = 0.0F;
	color.blue = 0.0F;
	color.alpha = 1.0F;

	gtk_widget_override_background_color (GTK_WIDGET(self), GTK_STATE_FLAG_NORMAL, &color);
	gtk_widget_override_background_color (GTK_WIDGET(self), GTK_STATE_FLAG_ACTIVE, &color);
	gtk_widget_override_background_color (GTK_WIDGET(self), GTK_STATE_FLAG_PRELIGHT, &color);
	gtk_widget_override_background_color (GTK_WIDGET(self), GTK_STATE_FLAG_SELECTED, &color);
	gtk_widget_override_background_color (GTK_WIDGET(self), GTK_STATE_FLAG_INSENSITIVE, &color);
	gtk_widget_override_background_color (GTK_WIDGET(self), GTK_STATE_FLAG_INCONSISTENT, &color);
	gtk_widget_override_background_color (GTK_WIDGET(self), GTK_STATE_FLAG_FOCUSED, &color);
#endif

	return GTK_WIDGET(self);
}

void
gtk_libvlc_media_player_add_media (GtkLibvlcMediaPlayer *self, GtkLibvlcMedia *media)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Add the media in the media list
	GtkTreeIter iter;
	gtk_tree_store_append (self->media_list, &iter, NULL);
	gtk_tree_store_set (GTK_TREE_STORE(self->media_list), &iter,
	                    GTK_LIBVLC_MODEL_MEDIA_COLUMN, media, -1);
}

GtkLibvlcMedia*
gtk_libvlc_media_player_get_current_media (GtkLibvlcMediaPlayer *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), NULL);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	return gtk_libvlc_media_player_get_media_from_path(self, priv->current_media);
}

GtkLibvlcMedia*
gtk_libvlc_media_player_get_media_from_path (GtkLibvlcMediaPlayer *self, GtkTreePath *path)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), NULL);
	g_return_val_if_fail(path != NULL, NULL);

	// Get the media from the path
	GtkLibvlcMedia *media;
	GtkTreeIter iter;
	if(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter, path)){
		gtk_tree_model_get (GTK_TREE_MODEL(self->media_list),
		                    &iter, 0, &media, -1);
		return media;
	}

	return NULL;
}

void
gtk_libvlc_media_player_clear_media_list(GtkLibvlcMediaPlayer *self)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	gtk_tree_store_clear (self->media_list);

	gtk_libvlc_media_player_set_current_path(self, NULL);
}

void
gtk_libvlc_media_player_play (GtkLibvlcMediaPlayer *self, gchar **options, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

	if(pError == NULL){
		// Play the current media or the first if no current media
		GtkTreePath *path = priv->current_media;
		if(path == NULL){
			path = gtk_tree_path_new_first();
			gtk_libvlc_media_player_set_current_path(self, path);
			gtk_tree_path_free(path);
		}
		gtk_libvlc_media_player_play_media_at_path(self, priv->current_media, options, &pError);
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
}

void
gtk_libvlc_media_player_play_media_at_path (GtkLibvlcMediaPlayer *self, GtkTreePath *path, gchar **options, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	g_return_if_fail(path != NULL);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

	if(pError == NULL){
		gtk_libvlc_media_player_set_current_path(self, path);

		GtkLibvlcMedia *media;
		media = gtk_libvlc_media_player_get_media_from_path (self, priv->current_media);

		GtkTreeIter iter;
		g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter, priv->current_media) == TRUE);
		if(gtk_tree_model_iter_has_child(GTK_TREE_MODEL(self->media_list), &iter)){
			priv->is_media_parsed = TRUE;
		}else{
			priv->is_media_parsed = FALSE;
		}

		// Play the media
		gtk_libvlc_media_player_play_media (self, media, options, &pError);
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
}

void
gtk_libvlc_media_player_play_next (GtkLibvlcMediaPlayer *self, gchar **options, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

	if(pError == NULL){
		GtkTreePath* path = NULL;
		if(priv->current_media != NULL){

			GtkTreeIter iter1;
			GtkTreeIter iter2;
			g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter1, priv->current_media) == TRUE);
			if(gtk_tree_model_iter_has_child(GTK_TREE_MODEL(self->media_list), &iter1)){
				gtk_tree_model_iter_children(GTK_TREE_MODEL(self->media_list), &iter2, &iter1);
				path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter2);
			}else{
				if(gtk_tree_model_iter_next (GTK_TREE_MODEL(self->media_list), &iter1)){
					path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
				}else{
					gtk_tree_model_get_iter (GTK_TREE_MODEL(self->media_list), &iter1, priv->current_media);
					if(gtk_tree_model_iter_parent (GTK_TREE_MODEL(self->media_list), &iter2, &iter1)){
						if(gtk_tree_model_iter_next (GTK_TREE_MODEL(self->media_list), &iter2)){
							path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter2);
						}else{
							if(priv->loop){
								gtk_tree_model_get_iter_first(GTK_TREE_MODEL(self->media_list), &iter1);
								path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
							}
						}
					}else{
						if(priv->loop){
							gtk_tree_model_get_iter_first(GTK_TREE_MODEL(self->media_list), &iter1);
							path = gtk_tree_model_get_path(GTK_TREE_MODEL(self->media_list), &iter1);
						}
					}
				}
			}

			// Play the next media
			if(path != NULL){
				gtk_libvlc_media_player_play_media_at_path(self, path, options, &pError);
			}
		}else{
			// Play the first media
			if(priv->loop){
				gtk_libvlc_media_player_play(self, options, &pError);
			}
		}
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
}

void
gtk_libvlc_media_player_pause (GtkLibvlcMediaPlayer *self, GError **error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	if(pError == NULL){
#ifdef LIBVLC_DEPRECATED_PLAYLIST
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_if_fail(libvlc_instance != NULL);

		libvlc_playlist_pause (libvlc_instance, &_vlcexcep);
		raise_error(self, &pError, &_vlcexcep);
#else
#ifdef LIBVLC_OLD_VLCEXCEPTION
		libvlc_media_player_pause (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, &pError, &_vlcexcep);
#else
		libvlc_media_player_pause (priv->libvlc_mediaplayer);
		raise_error(self, &pError, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION
#endif // LIBVLC_DEPRECATED_PLAYLIST
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
}

gboolean
gtk_libvlc_media_player_can_pause (GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), FALSE);
	g_return_val_if_fail(self->libvlc_instance != NULL, FALSE);
	if(error != NULL){
		g_return_val_if_fail(*error == NULL, FALSE);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_val_if_fail(priv->initialized == TRUE, FALSE);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	gboolean ret = FALSE;

	if(pError == NULL){

#ifdef LIBVLC_DEPRECATED_PLAYLIST
		ret = TRUE;
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
		ret = libvlc_media_player_can_pause (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, &pError, &_vlcexcep);
#else
		ret = libvlc_media_player_can_pause (priv->libvlc_mediaplayer);
		raise_error(self, &pError, NULL);

#endif // LIBVLC_OLD_VLCEXCEPTION	

#endif // LIBVLC_DEPRECATED_PLAYLIST
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}

	return ret;

}

void
gtk_libvlc_media_player_stop (GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	if(pError == NULL){
		// Stop the current playing
#ifdef LIBVLC_DEPRECATED_PLAYLIST
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_if_fail(libvlc_instance != NULL);

		if (libvlc_playlist_isplaying (libvlc_instance, &_vlcexcep)) {
			raise_error(self, &pError, &_vlcexcep);
			libvlc_playlist_stop (libvlc_instance, &_vlcexcep);
			raise_error(self, &pError, &_vlcexcep);
		}
#else
		if(priv->libvlc_mediaplayer != NULL){
#ifdef LIBVLC_OLD_VLCEXCEPTION
			libvlc_media_player_stop (priv->libvlc_mediaplayer, &_vlcexcep);
			raise_error(self, &pError, &_vlcexcep);
#else
			libvlc_media_player_stop (priv->libvlc_mediaplayer);
			raise_error(self, &pError, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION
		}
#endif // LIBVLC_DEPRECATED_PLAYLIST
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
}

void
gtk_libvlc_media_player_set_volume (GtkLibvlcMediaPlayer *self, gdouble volume, GError** error)
{

	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(volume >= 0 && volume <= LIBVLC_MAX_VOLUME_POWER);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

	if(pError == NULL){

#ifdef LIBVLC_OLD_VLCEXCEPTION
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_if_fail(libvlc_instance != NULL);

		libvlc_exception_t _vlcexcep;
		libvlc_exception_init (&_vlcexcep);

		libvlc_audio_set_volume (libvlc_instance, 
		                         (gint)(volume*100), &_vlcexcep);    
		raise_error(self, error, &_vlcexcep);
#else
		libvlc_audio_set_volume (priv->libvlc_mediaplayer, 
		                         (gint)(volume*100));    
		raise_error(self, error, NULL);

#endif // LIBVLC_OLD_VLCEXCEPTION
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
	
}

gdouble
gtk_libvlc_media_player_get_volume (GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, 0.0);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), 0.0);
	g_return_val_if_fail(self->libvlc_instance != NULL, 0.0);
	if(error != NULL){
		g_return_val_if_fail(*error == NULL, 0.0);
	}

	GError* pError = NULL;
	gint volume = 0.0;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_val_if_fail(priv->initialized == TRUE, 0.0);

	if(pError == NULL){
#ifdef LIBVLC_OLD_VLCEXCEPTION
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_val_if_fail(libvlc_instance != NULL, 0.0);

		libvlc_exception_t _vlcexcep;
		libvlc_exception_init (&_vlcexcep);

		volume = libvlc_audio_get_volume (libvlc_instance, &_vlcexcep);
		raise_error(self, &pError, &_vlcexcep);
#else
		gint volume;
		volume = libvlc_audio_get_volume (priv->libvlc_mediaplayer);
		raise_error(self, &pError, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}

	return (gdouble)volume/100;
}

void
gtk_libvlc_media_player_set_mute (GtkLibvlcMediaPlayer *self, gboolean mute, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

	if(pError == NULL){

#ifdef LIBVLC_OLD_VLCEXCEPTION
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_if_fail(libvlc_instance != NULL);

		libvlc_exception_t _vlcexcep;
		libvlc_exception_init (&_vlcexcep);

		libvlc_audio_set_mute (libvlc_instance, (mute == TRUE ? 1 : 0), &_vlcexcep);    
		raise_error(self, error, &_vlcexcep);
#else
		libvlc_audio_set_mute (priv->libvlc_mediaplayer, (mute == TRUE ? 1 : 0));    
		raise_error(self, error, NULL);

#endif // LIBVLC_OLD_VLCEXCEPTION
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
}

gboolean
gtk_libvlc_media_player_get_mute (GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), FALSE);
	g_return_val_if_fail(self->libvlc_instance != NULL, FALSE);
	if(error != NULL){
		g_return_val_if_fail(*error == NULL, FALSE);
	}

	GError* pError = NULL;
	gboolean mute = FALSE;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_val_if_fail(priv->initialized == TRUE, FALSE);

	if(pError == NULL){
#ifdef LIBVLC_OLD_VLCEXCEPTION
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_val_if_fail(libvlc_instance != NULL, FALSE);

		libvlc_exception_t _vlcexcep;
		libvlc_exception_init (&_vlcexcep);

		mute = (libvlc_audio_get_mute (libvlc_instance, &_vlcexcep) == 1 ? TRUE : FALSE);
		raise_error(self, &pError, &_vlcexcep);
#else
		mute = (libvlc_audio_get_mute (priv->libvlc_mediaplayer) == 1 ? TRUE : FALSE);
		raise_error(self, &pError, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}

	return mute;
}

void
gtk_libvlc_media_player_set_fullscreen (GtkLibvlcMediaPlayer *self, gboolean fullscreen, GError **error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	if(pError == NULL){
#ifdef LIBVLC_OLD_VLCEXCEPTION
		libvlc_exception_t _vlcexcep;
		libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

		// Create the media player if not initialized
		gtk_libvlc_media_player_initialize (self, &pError);
		g_return_if_fail(priv->initialized == TRUE);

#ifdef LIBVLC_DEPRECATED_PLAYLIST
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_if_fail(libvlc_instance != NULL);

		libvlc_input_t *input_t;
		if (libvlc_playlist_isplaying (libvlc_instance, &_vlcexcep)) {
			input_t = libvlc_playlist_get_input(libvlc_instance,
			                                    &_vlcexcep);
			raise_error(self, &pError, &_vlcexcep);
			libvlc_set_fullscreen(input_t, fullscreen, &_vlcexcep);
			raise_error(self, &pError, &_vlcexcep);
			libvlc_input_free(input_t);
		}
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
		if(priv->libvlc_mediaplayer != NULL){
			libvlc_set_fullscreen (priv->libvlc_mediaplayer, fullscreen, &_vlcexcep);
			raise_error(self, &pError, &_vlcexcep);
		}
#else

		if(fullscreen){
			if(!priv->isModeFullscreen && gtk_libvlc_media_player_is_playing(self, &pError)){
#ifndef LIBVLC_OLD_FULLSCREEN_MODE
				if(priv->pWindowFullscreen == NULL){
					priv->pWindowFullscreen = gtk_window_new (GTK_WINDOW_TOPLEVEL);
					// We attach the accel accel groups
					gtk_window_add_accel_group (GTK_WINDOW(priv->pWindowFullscreen), priv->pAccelGroup);
				}
				priv->pWigdetOriginalParent = gtk_widget_get_parent (GTK_WIDGET(self));

				GtkWidget *pTopLevel = gtk_widget_get_toplevel (priv->pWigdetOriginalParent);
				if (gtk_widget_is_toplevel (pTopLevel)) {
					gtk_window_get_position (GTK_WINDOW(pTopLevel), &priv->root_x, &priv->root_y); 
					gtk_widget_hide(pTopLevel);
				}
					
       			gtk_widget_show (priv->pWindowFullscreen);

				gtk_widget_reparent (GTK_WIDGET(self), priv->pWindowFullscreen);
				gtk_window_fullscreen(GTK_WINDOW(priv->pWindowFullscreen));

#endif // LIBVLC_OLD_FULLSCREEN_MODE
				
				priv->isModeFullscreen = TRUE;
			}
		}else{
			if(priv->isModeFullscreen){
#ifndef LIBVLC_OLD_FULLSCREEN_MODE
				gtk_window_unfullscreen(GTK_WINDOW(priv->pWindowFullscreen));
				gtk_widget_reparent (GTK_WIDGET(self), priv->pWigdetOriginalParent);

				GtkWidget *pTopLevel = gtk_widget_get_toplevel (priv->pWigdetOriginalParent);
				if (gtk_widget_is_toplevel (pTopLevel)) {
					gtk_widget_show(pTopLevel);
					// Restore the previous position of the windows that was erased by gtk_widget_hide
					gtk_window_move(GTK_WINDOW(pTopLevel), priv->root_x, priv->root_y); 
				}

				
				gtk_widget_hide (priv->pWindowFullscreen);
#endif // LIBVLC_OLD_FULLSCREEN_MODE
				
				priv->isModeFullscreen = FALSE;
			}
		}

		/*
		priv->pWigdetOriginalParent = NULL;
		if(priv->pWindowFullscreen){
			g_object_unref(pWindowFullscreen);
			priv->pWindowFullscreen = NULL;
		}
		*/

#endif // LIBVLC_OLD_VLCEXCEPTION

#endif // LIBVLC_DEPRECATED_PLAYLIST
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
	
}

gboolean
gtk_libvlc_media_player_get_fullscreen (GtkLibvlcMediaPlayer *self)
{
	g_return_val_if_fail(self != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), FALSE);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	return priv->isModeFullscreen;
}

void
gtk_libvlc_media_player_toggle_fullscreen (GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	if(gtk_libvlc_media_player_get_fullscreen(self)){
		gtk_libvlc_media_player_set_fullscreen (self, FALSE, error);
	}else{
		gtk_libvlc_media_player_set_fullscreen (self, TRUE, error);
	}
}

void
gtk_libvlc_media_player_set_accel_group (GtkLibvlcMediaPlayer *self,
                                         GtkAccelGroup *accel_group)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	if(priv->pWindowFullscreen != NULL && priv->pAccelGroup != NULL){
		gtk_window_remove_accel_group (GTK_WINDOW(priv->pWindowFullscreen), priv->pAccelGroup);
	}
	
	if(priv->pAccelGroup){
#if GTK_API_VERSION == 3
		g_object_unref (priv->pAccelGroup);
#else
		gtk_accel_group_unref (priv->pAccelGroup);
#endif
		priv->pAccelGroup = NULL;
	}

	if(priv->pWindowFullscreen != NULL && priv->pAccelGroup != NULL){
		gtk_window_add_accel_group (GTK_WINDOW(priv->pWindowFullscreen), priv->pAccelGroup);
	}

	priv->pAccelGroup = accel_group;
}

gboolean
gtk_libvlc_media_player_is_playing (GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), FALSE);
	g_return_val_if_fail(self->libvlc_instance != NULL, FALSE);
	if(error != NULL){
		g_return_val_if_fail(*error == NULL, FALSE);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	gboolean bRes = FALSE; 
	
#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	int res;

	if(pError == NULL){
#ifdef LIBVLC_DEPRECATED_PLAYLIST
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_if_fail(libvlc_instance != NULL);

		res = libvlc_playlist_isplaying(libvlc_instance, &_vlcexcep);
		raise_error(self, &pError, &_vlcexcep);
		bRes = (res == 1 ? TRUE : FALSE);
#else
#ifdef LIBVLC_DONT_HAVE_ISPLAYING
		bRes = (gtk_libvlc_media_player_get_state(self, &pError)==GTK_LIBVLC_STATE_PLAYING ? TRUE : FALSE);
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
		res = libvlc_media_player_is_playing (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, &pError, &_vlcexcep);
		bRes = (res == 1 ? TRUE : FALSE);
#else
		res = libvlc_media_player_is_playing (priv->libvlc_mediaplayer);
		raise_error(self, &pError, NULL);
		bRes = (res == 1 ? TRUE : FALSE);
#endif

#endif // LIBVLC_DONT_HAVE_ISPLAYING

#endif // LIBVLC_DEPRECATED_PLAYLIST
	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}

	return bRes;
}

GtkLibvlcState
gtk_libvlc_media_player_get_state (GtkLibvlcMediaPlayer *self, GError** error)
{
	GtkLibvlcState gtkstate = GTK_LIBVLC_STATE_NOTHING_SPECIAL;
	
	g_return_val_if_fail(self != NULL, gtkstate);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), gtkstate);
	g_return_val_if_fail(self->libvlc_instance != NULL, gtkstate);
	if(error != NULL){
		g_return_val_if_fail(*error == NULL, gtkstate);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_val_if_fail(priv->initialized == TRUE, gtkstate);

	if(pError == NULL){

#ifdef LIBVLC_OLD_VLCEXCEPTION
		libvlc_exception_t _vlcexcep;
		libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

#ifdef LIBVLC_DEPRECATED_PLAYLIST
		libvlc_instance_t *libvlc_instance;
		libvlc_instance = (libvlc_instance_t *)
			gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, &pError);
		g_return_val_if_fail(libvlc_instance != NULL, gtkstate);

		libvlc_input_t *input_t;
		if(libvlc_instance != NULL){
			if (libvlc_playlist_isplaying (libvlc_instance, &_vlcexcep)) {
				input_t = libvlc_playlist_get_input(libvlc_instance,
				                                    &_vlcexcep);
				raise_error(self, &pError, &_vlcexcep);
				int state = libvlc_input_get_state(input_t, &_vlcexcep);
				raise_error(self, &pError, &_vlcexcep);
				switch(state){
					case 0 :
						gtkstate = GTK_LIBVLC_STATE_NOTHING_SPECIAL;
						break;
					case 1 :
						gtkstate = GTK_LIBVLC_STATE_OPENING;
						break;
					case 2 :
						gtkstate = GTK_LIBVLC_STATE_BUFFERING;	
						break;
					case 3 :
						gtkstate = GTK_LIBVLC_STATE_PLAYING;
						break;
					case 4 :
						gtkstate = GTK_LIBVLC_STATE_PAUSED;
						break;
					case 5 :
						gtkstate = GTK_LIBVLC_STATE_STOPPED;
						break;
					case 6 :
						gtkstate = GTK_LIBVLC_STATE_ENDED;
						break;
					case 7 :
						gtkstate = GTK_LIBVLC_STATE_ERROR;
						break;
				}
				libvlc_input_free(input_t);
			}
		}
#else
		g_return_val_if_fail(priv->libvlc_mediaplayer != NULL, gtkstate);

		libvlc_state_t state;

#ifdef LIBVLC_OLD_VLCEXCEPTION
		state = libvlc_media_player_get_state (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, &pError, &_vlcexcep);
#else
		state = libvlc_media_player_get_state (priv->libvlc_mediaplayer);
		raise_error(self, &pError, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION	

		switch(state){
			case libvlc_NothingSpecial :
				gtkstate = GTK_LIBVLC_STATE_NOTHING_SPECIAL;
				break;
			case libvlc_Opening :
				gtkstate = GTK_LIBVLC_STATE_OPENING;
				break;
			case libvlc_Buffering :
				gtkstate = GTK_LIBVLC_STATE_BUFFERING;	
				break;
			case libvlc_Playing :
				gtkstate = GTK_LIBVLC_STATE_PLAYING;
				break;
			case libvlc_Paused :
				gtkstate = GTK_LIBVLC_STATE_PAUSED;
				break;
			case libvlc_Stopped :
				gtkstate = GTK_LIBVLC_STATE_STOPPED;
				break;
			case libvlc_Ended :
				gtkstate = GTK_LIBVLC_STATE_ENDED;
				break;
			case libvlc_Error:
				gtkstate = GTK_LIBVLC_STATE_ERROR;
				break;
		}
#endif // LIBVLC_DEPRECATED_PLAYLIST

	}

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
	
	return gtkstate;

}

const gchar*
gtk_libvlc_media_player_state_tostring (GtkLibvlcState state)
{
	switch(state){
		case GTK_LIBVLC_STATE_NOTHING_SPECIAL :
			return "GTK_LIBVLC_STATE_NOTHING_SPECIAL";
		case GTK_LIBVLC_STATE_OPENING :
			return "GTK_LIBVLC_STATE_OPENING";
		case GTK_LIBVLC_STATE_BUFFERING :
			return "GTK_LIBVLC_STATE_BUFFERING";
		case GTK_LIBVLC_STATE_PLAYING :
			return "GTK_LIBVLC_STATE_PLAYING";
		case GTK_LIBVLC_STATE_PAUSED :
			return "GTK_LIBVLC_STATE_PAUSED";
		case GTK_LIBVLC_STATE_STOPPED :
			return "GTK_LIBVLC_STATE_STOPPED";
		case GTK_LIBVLC_STATE_ENDED :
			return "GTK_LIBVLC_STATE_ENDED";
		case GTK_LIBVLC_STATE_ERROR:
			return "GTK_LIBVLC_STATE_ERROR";
	}
	return "";
}

glong
gtk_libvlc_media_player_get_length(GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, 0);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), 0);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	glong length = 0;

#ifdef LIBVLC_DEPRECATED_PLAYLIST
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = (libvlc_instance_t *)
		gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, error);
	g_return_val_if_fail(libvlc_instance != NULL, 0);

	libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
	                                    &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	length = libvlc_input_get_length(input_t, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_input_free(input_t);
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	if(media != NULL){
		length = libvlc_media_player_get_length (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
	}
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer);
	raise_error(self, error, NULL);
	if(media != NULL){
		length = libvlc_media_player_get_length (priv->libvlc_mediaplayer);
		raise_error(self, error, NULL);
	}
#endif // LIBVLC_OLD_VLCEXCEPTION

#endif // LIBVLC_DEPRECATED_PLAYLIST
	return length;
}

glong
gtk_libvlc_media_player_get_time(GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, 0);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), 0);
	
	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	glong time = 0;

#ifdef LIBVLC_DEPRECATED_PLAYLIST
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = (libvlc_instance_t *)
		gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, error);
	g_return_val_if_fail(libvlc_instance != NULL, 0);

	libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
	                                    &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	time = libvlc_input_get_time(input_t, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_input_free(input_t);
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	if(media != NULL){
		time = libvlc_media_player_get_time (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
	}
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer);
	raise_error(self, error, NULL);
	if(media != NULL){
		time = libvlc_media_player_get_time (priv->libvlc_mediaplayer);
		raise_error(self, error, NULL);
	}
#endif // LIBVLC_OLD_VLCEXCEPTION	

#endif // LIBVLC_DEPRECATED_PLAYLIST
	return time;
}

void
gtk_libvlc_media_player_set_time(GtkLibvlcMediaPlayer *self, glong time, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION	

#ifdef LIBVLC_DEPRECATED_PLAYLIST
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = (libvlc_instance_t *)
		gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, error);
	g_return_if_fail(libvlc_instance != NULL);

	libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
	                                    &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_input_set_time(input_t, time, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_input_free(input_t);
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_media_player_set_time (priv->libvlc_mediaplayer, time, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
#else
	libvlc_media_player_set_time (priv->libvlc_mediaplayer, time);
	raise_error(self, error, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION

#endif // LIBVLC_DEPRECATED_PLAYLIST
}

gfloat
gtk_libvlc_media_player_get_position(GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, 0.0);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), 0.0);
	
	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	gfloat pos = 0.0;

#ifdef LIBVLC_DEPRECATED_PLAYLIST
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = (libvlc_instance_t *)
		gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, error);
	g_return_val_if_fail(libvlc_instance != NULL, 0.0);

	libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
	                                    &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	pos = libvlc_input_get_position(input_t, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_input_free(input_t);
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	if(media != NULL){
		pos = libvlc_media_player_get_position (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
	}
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer);
	raise_error(self, error, NULL);
	if(media != NULL){
		pos = libvlc_media_player_get_position (priv->libvlc_mediaplayer);
		raise_error(self, error, NULL);
	}
#endif // LIBVLC_OLD_VLCEXCEPTION	

#endif // LIBVLC_DEPRECATED_PLAYLIST
	return pos;

}

void
gtk_libvlc_media_player_set_position(GtkLibvlcMediaPlayer *self, gfloat position, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	
	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	g_return_if_fail(position>=0.0 && position<=1.0);	

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

#ifdef LIBVLC_DEPRECATED_PLAYLIST
	libvlc_instance_t *libvlc_instance;
	libvlc_instance = (libvlc_instance_t *)
		gtk_libvlc_instance_get_libvlc_instance(self->libvlc_instance, error);
	g_return_if_fail(libvlc_instance != NULL);

	libvlc_input_t *input_t;
	input_t = libvlc_playlist_get_input(libvlc_instance,
	                                    &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_input_set_position(input_t, position, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	libvlc_input_free(input_t);

#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_media_player_set_position (priv->libvlc_mediaplayer, position, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
#else
	libvlc_media_player_set_position (priv->libvlc_mediaplayer, position);
	raise_error(self, error, NULL);
#endif // LIBVLC_OLD_VLCEXCEPTION

#endif // LIBVLC_DEPRECATED_PLAYLIST
}

gboolean
gtk_libvlc_media_player_is_seekable (GtkLibvlcMediaPlayer *self, GError** error)
{
	g_return_val_if_fail(self != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), FALSE);

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

	gboolean ret = FALSE;

#ifdef LIBVLC_DEPRECATED_PLAYLIST
	ret = TRUE;
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer, &_vlcexcep);
	raise_error(self, error, &_vlcexcep);
	if(media != NULL){
		ret = libvlc_media_player_is_seekable (priv->libvlc_mediaplayer, &_vlcexcep);
		raise_error(self, error, &_vlcexcep);
	}
#else
	libvlc_media_t* media;
	media = libvlc_media_player_get_media (priv->libvlc_mediaplayer);
	raise_error(self, error, NULL);
	if(media != NULL){
		ret = libvlc_media_player_is_seekable (priv->libvlc_mediaplayer);
		raise_error(self, error, NULL);
	}
#endif // LIBVLC_OLD_VLCEXCEPTION

#endif // LIBVLC_DEPRECATED_PLAYLIST
	return ret;
}

void
gtk_libvlc_media_player_set_deinterlace (GtkLibvlcMediaPlayer *self, const gchar* mode, GError** error)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));
	g_return_if_fail(self->libvlc_instance != NULL);
	if(error != NULL){
		g_return_if_fail(*error == NULL);
	}

	GError* pError = NULL;

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	// Create the media player if not initialized
	gtk_libvlc_media_player_initialize (self, &pError);
	g_return_if_fail(priv->initialized == TRUE);

#ifdef LIBVLC_OLD_VLCEXCEPTION
	libvlc_exception_t _vlcexcep;
	libvlc_exception_init (&_vlcexcep);
#endif // LIBVLC_OLD_VLCEXCEPTION

#ifdef LIBVLC_DEPRECATED_PLAYLIST
	pError = g_error_new (GTK_LIBVLC_ERROR,
	                      GTK_LIBVLC_ERROR_LIBVLC,
	                      "Deinterlace is not supported for your LibVLC version.");
#else

#ifdef LIBVLC_OLD_VLCEXCEPTION
	pError = g_error_new (GTK_LIBVLC_ERROR,
	                      GTK_LIBVLC_ERROR_LIBVLC,
	                      "Deinterlace is not supported for your LibVLC version.");
	// libvlc_video_set_deinterlace  (priv->libvlc_mediaplayer, mode, &_vlcexcep);
	// raise_error(self, error, &_vlcexcep);
#else
	if(gtk_libvlc_media_player_is_playing (self, &pError)){
		libvlc_video_set_deinterlace (priv->libvlc_mediaplayer, mode);
		raise_error(self, error, NULL);
	}
#endif // LIBVLC_OLD_VLCEXCEPTION

#endif // LIBVLC_DEPRECATED_PLAYLIST

	if(pError){
		if(error){
			*error = pError;
		}else{
			g_error_free (pError);
			pError = NULL;
		}
	}
}

void
gtk_libvlc_media_player_set_play_next_at_end (GtkLibvlcMediaPlayer *self, gboolean b)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	priv->play_next_at_end = b;	
}

void
gtk_libvlc_media_player_set_loop (GtkLibvlcMediaPlayer *self, gboolean b)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self));

	GtkLibvlcMediaPlayerPrivate* priv;
	priv = GTK_LIBVLC_MEDIA_PLAYER_PRIVATE(self);

	priv->loop = b;	
}

GtkLibvlcInstance*
gtk_libvlc_media_player_get_instance (GtkLibvlcMediaPlayer *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_LIBVLC_MEDIA_PLAYER(self), NULL);
	return self->libvlc_instance;
}
