/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */

#ifndef GTK_LIBVLC_INCLUDE_H
#define GTK_LIBVLC_INCLUDE_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

// Set default value for LIBVLC_VERSION_MAJOR
#ifndef LIBVLC_VERSION_MAJOR
#define LIBVLC_VERSION_MAJOR = 0
#endif

// Set default value for LIBVLC_VERSION_MINOR
#ifndef LIBVLC_VERSION_MINOR
#define LIBVLC_VERSION_MAJOR = 9
#endif

#if LIBVLC_VERSION_MAJOR == 0 && LIBVLC_VERSION_MINOR == 8
#include <vlc/libvlc.h> // LibVLC header for 0.8.x
#else
#include <vlc/vlc.h> // LibVLC header for >=0.9.x
#endif

#endif /* GTK_LIBVLC_INCLUDE_H */
