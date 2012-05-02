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

#ifndef _GTK_LIBVLC_INCLUDE_H_
#define _GTK_LIBVLC_INCLUDE_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

// Max volume power for VLC
#define LIBVLC_MAX_VOLUME_POWER 2.0 // Represent a percentage (2.0 = 200%)

// Define handle to de
typedef gpointer LIBVLC_INSTANCE_HANDLE;

#define GTK_LIBVLC_ERROR gtk_libvlc_error_quark ()
typedef enum
{
	GTK_LIBVLC_ERROR_LIBVLC
} GtkLibvlcError;

GQuark
gtk_libvlc_error_quark ();

// Enum
typedef enum {
	// Media player event
	GTK_LIBVLC_EVENT_MP_NOTHINGSPECIAL,
	GTK_LIBVLC_EVENT_MP_OPENING,
	GTK_LIBVLC_EVENT_MP_BUFFERING,
	GTK_LIBVLC_EVENT_MP_PLAYING,
	GTK_LIBVLC_EVENT_MP_PAUSED,
	GTK_LIBVLC_EVENT_MP_STOPPED,
	GTK_LIBVLC_EVENT_MP_FORWARD,
	GTK_LIBVLC_EVENT_MP_BACKWARD,
	GTK_LIBVLC_EVENT_MP_ENDREACHED,
	GTK_LIBVLC_EVENT_MP_ENCOUTEREDERROR,
} GtkLibvlcEventType;

#endif /* _GTK_LIBVLC_INCLUDE_H_ */
