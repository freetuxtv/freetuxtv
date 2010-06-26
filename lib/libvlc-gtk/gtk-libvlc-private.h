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

#include "gtk-libvlc-include.h"

// List of options avaliable for each branche
// LIBVLC_OLD_INSTANCE : The branche use the old libvlc_instance_t interface
// LIBVLC_OLD_VLCEXCEPTION : The branche use the deprecated libvlc_exception_t object
// LIBVLC_DEPRECATED_PLAYLIST : The branche use the old playlist system
// LIBVLC_OLD_SET_DRAWABLE : The branche use the libvlc_media_player_set_drawable function
// LIBVLC_DONT_HAVE_ISPLAYING : The branche don't support libvlc_media_player_is_playing function

// Define the option for each branche
#if LIBVLC_VERSION_MAJOR >= 1

#if LIBVLC_VERSION_MAJOR >= 1

#elif LIBVLC_VERSION_MAJOR == 0

// Branche 1.0.x
#define LIBVLC_OLD_VLCEXCEPTION

#endif

#elif LIBVLC_VERSION_MAJOR == 0

#if LIBVLC_VERSION_MINOR == 9

// Branche 0.9.x
#define LIBVLC_OLD_VLCEXCEPTION
#define LIBVLC_OLD_SET_DRAWABLE
#define LIBVLC_DONT_HAVE_ISPLAYING

#elif LIBVLC_VERSION_MINOR == 8

// Branche 0.8.x
#define LIBVLC_OLD_INSTANCE
#define LIBVLC_OLD_VLCEXCEPTION
#define LIBVLC_DEPRECATED_PLAYLIST

#else
// LIBVLC must be at least 0.8.6
#error "LIBVLC version is not supported"

#endif
#endif

#ifdef LIBVLC_OLD_INSTANCE
#include <vlc/libvlc.h> // LibVLC header for 0.8.x
#else
#include <vlc/vlc.h> // LibVLC header for >= 0.9.x
#endif