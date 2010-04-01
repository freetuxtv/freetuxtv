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

// Set default value for LIBVLC_VERSION_MAJOR
#ifndef LIBVLC_VERSION_MAJOR
#define LIBVLC_VERSION_MAJOR 0
#endif

// Set default value for LIBVLC_VERSION_MINOR
#ifndef LIBVLC_VERSION_MINOR
#define LIBVLC_VERSION_MINOR 9
#endif

// Set default value for LIBVLC_VERSION_REVISION
#ifndef LIBVLC_VERSION_REVISION
#define LIBVLC_VERSION_REVISION 0
#endif

// Max volume power for VLC
#define LIBVLC_MAX_VOLUME_POWER 2.0 // Represent a percentage (2.0 = 200%)

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
#include <vlc/libvlc.h> // LibVLC header for 0.8.x
#else
#include <vlc/vlc.h> // LibVLC header for >= 0.9.x
#endif

#endif /* _GTK_LIBVLC_INCLUDE_H_ */