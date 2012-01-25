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

#ifndef _GTK_LIBVLC_PRIVATE_H_
#define _GTK_LIBVLC_PRIVATE_H_

#include "gtk-libvlc-include.h"

#define GTK_LIBVLC_LOG_DOMAIN			"LibVLC-Gtk"

// Set default value for LIBVLC_VERSION_MAJOR
#ifndef LIBVLC_VERSION_MAJOR
#error "LIBVLC_VERSION_MAJOR must be defined."
#endif

// Set default value for LIBVLC_VERSION_MINOR
#ifndef LIBVLC_VERSION_MINOR
#error "LIBVLC_VERSION_MINOR must be defined."
#endif

// Set default value for LIBVLC_VERSION_REVISION
#ifndef LIBVLC_VERSION_REVISION
#error "LIBVLC_VERSION_REVISION must be defined."
#endif

// List of options avaliable for each branche
// LIBVLC_OLD_INSTANCE : The branche use the old libvlc_instance_t interface
// LIBVLC_OLD_VLCEXCEPTION : The branche use the deprecated libvlc_exception_t object
// LIBVLC_DEPRECATED_PLAYLIST : The branche use the old playlist system
// LIBVLC_OLD_SET_DRAWABLE : The branche use the libvlc_media_player_set_drawable function
// LIBVLC_DONT_HAVE_ISPLAYING : The branche don't support libvlc_media_player_is_playing function
// LIBVLC_OLD_FULLSCREEN_MODE : The branche don't need a top level window for fullscreen mode

// Define the option for each branche
#if LIBVLC_VERSION_MAJOR >= 1


#if LIBVLC_VERSION_MINOR >= 1
// Current branche

#elif LIBVLC_VERSION_MINOR == 0
// Branche 1.0.x
#define LIBVLC_OLD_VLCEXCEPTION
#define LIBVLC_OLD_FULLSCREEN_MODE

#endif // LIBVLC_VERSION_MINOR >= 1

#elif LIBVLC_VERSION_MAJOR == 0

#if LIBVLC_VERSION_MINOR == 9
// Branche 0.9.x
#define LIBVLC_OLD_VLCEXCEPTION
#define LIBVLC_OLD_SET_DRAWABLE
#define LIBVLC_DONT_HAVE_ISPLAYING
#define LIBVLC_OLD_FULLSCREEN_MODE

#elif LIBVLC_VERSION_MINOR == 8
// Branche 0.8.x
#define LIBVLC_OLD_INSTANCE
#define LIBVLC_OLD_VLCEXCEPTION
#define LIBVLC_DEPRECATED_PLAYLIST
#define LIBVLC_OLD_FULLSCREEN_MODE

#else
// LIBVLC must be at least 0.8.6
#error "LIBVLC version is not supported"

#endif // LIBVLC_VERSION_MINOR == 9

#endif // LIBVLC_VERSION_MAJOR >= 1

#ifdef LIBVLC_OLD_INSTANCE
// LibVLC header for 0.8.x
#include <vlc/libvlc.h>
#else
// LibVLC header for >= 0.9.x
#include <vlc/vlc.h>
#endif

#endif // _GTK_LIBVLC_PRIVATE_H_
