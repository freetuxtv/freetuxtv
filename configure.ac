dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(freetuxtv, 0.6.0, http://code.google.com/p/freetuxtv/)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

dnl Set a version for libvlc-gtk
LIBVLCGTK_LT_VERSION=0:1:0
AC_SUBST(LIBVLCGTK_LT_VERSION)

dnl Set a version for libdbevolution
LIBDBEVOLUTION_LT_VERSION=0:1:0
AC_SUBST(LIBDBEVOLUTION_LT_VERSION)

dnl ***************************************************************************
dnl Checks for programs
dnl ***************************************************************************
dnl AC_ISC_POSIX
dnl AC_PROG_CC
dnl AM_PROG_CC_STDC
dnl AC_HEADER_STDC

AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl AM_PROG_LIBTOOL
AC_DISABLE_SHARED
AC_PROG_LIBTOOL

AM_PROG_CC_C_O

dnl Check for pkg-config
if pkg-config --version >/dev/null 2>&1; then
  dnl We have pkg-config, everything is cool.
  PKGCONFIG=yes
else
  PKGCONFIG=no
fi

dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
GETTEXT_PACKAGE=freetuxtv
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])


dnl ***************************************************************************
dnl Checks for libraries.
dnl ***************************************************************************

dnl Get LibVLC version
LIBVLC_VERSION=""
LIBVLC_PKGCONFIG=""
LIBVLC_FOUND=no
VLC_BIN=`which vlc | sed '/.*:$/d;s/.*: //;s/ /\'$'\n''/g' | sed q`
VLC_CONFIG=`which vlc-config | sed '/.*:$/d;s/.*: //;s/ /\'$'\n''/g' | sed q`
dnl Check first with pkg-config
if test "$PKGCONFIG" == "yes"; then
  if pkg-config --exists libvlc; then
    LIBVLC_VERSION=`pkg-config --modversion libvlc | sed 's/[[a-zA-Z\-]].*$//'`
    LIBVLC_PKGCONFIG="libvlc"
    LIBVLC_FOUND=yes
  fi
fi
dnl If not found try with vlc-config
if test "$LIBVLC_FOUND" == "no"; then
  if test -x "$VLC_CONFIG"; then
    LIBVLC_VERSION=`$VLC_CONFIG --version | sed 's/[[a-zA-Z\-]].*$//'`
    LIBVLC_FOUND=yes
  fi
fi
dnl Set variable for the config.h
if test "$LIBVLC_FOUND" == "yes"; then
  LIBVLC_VERSION_MAJOR=`echo $LIBVLC_VERSION | cut -d'.' -f1`
  LIBVLC_VERSION_MINOR=`echo $LIBVLC_VERSION | cut -d'.' -f2`
  LIBVLC_VERSION_REVISION=`echo $LIBVLC_VERSION | cut -d'.' -f3`
  echo "compiling with LibVLC version $LIBVLC_VERSION_MAJOR.$LIBVLC_VERSION_MINOR.$LIBVLC_VERSION_REVISION"
  AC_DEFINE_UNQUOTED(LIBVLC_VERSION_MAJOR, $LIBVLC_VERSION_MAJOR, [LibVLC version major used for complication])
  AC_DEFINE_UNQUOTED(LIBVLC_VERSION_MINOR, $LIBVLC_VERSION_MINOR, [Current LibVLC version minor])
  AC_DEFINE_UNQUOTED(LIBVLC_VERSION_REVISION, $LIBVLC_VERSION_REVISION, [Current LibVLC version revision])
  if test "$LIBVLC_PKGCONFIG" != ""; then
    LIBVLC_PKGCONFIG="libvlc >= $LIBVLC_VERSION_MAJOR.$LIBVLC_VERSION_MINOR.$LIBVLC_VERSION_REVISION"
  fi
else
  echo
  echo "**Error**: Cannot detect LibVLC version."
  echo "Do you have installed the libvlc development (libvlc-dev) package?"
  exit 1
fi

dnl Get libnotify version
LIBNOTIFY_VERSION=""
LIBNOTIFY_PKGCONFIG=""
LIBNOTIFY_FOUND=no
if test "$PKGCONFIG" == "yes"; then
  if pkg-config --exists libvlc; then
    LIBNOTIFY_VERSION=`pkg-config --modversion libnotify | sed 's/[[a-zA-Z\-]].*$//'`
    LIBNOTIFY_PKGCONFIG="libnotify"
    LIBNOTIFY_FOUND=yes
  fi
fi
dnl Set variable for the config.h
if test "$LIBNOTIFY_FOUND" == "yes"; then
  LIBNOTIFY_VERSION_MAJOR=`echo $LIBNOTIFY_VERSION | cut -d'.' -f1`
  LIBNOTIFY_VERSION_MINOR=`echo $LIBNOTIFY_VERSION | cut -d'.' -f2`
  LIBNOTIFY_VERSION_REVISION=`echo $LIBNOTIFY_VERSION | cut -d'.' -f3`
  echo "compiling with libnotify version $LIBNOTIFY_VERSION_MAJOR.$LIBNOTIFY_VERSION_MINOR.$LIBNOTIFY_VERSION_REVISION"
  AC_DEFINE_UNQUOTED(LIBNOTIFY_VERSION_MAJOR, $LIBNOTIFY_VERSION_MAJOR, [libnotify version major used for complication])
  AC_DEFINE_UNQUOTED(LIBNOTIFY_VERSION_MINOR, $LIBNOTIFY_VERSION_MINOR, [Current libnotify version minor])
  AC_DEFINE_UNQUOTED(LIBNOTIFY_VERSION_REVISION, $LIBNOTIFY_VERSION_REVISION, [Current libnotify version revision])
  if test "$LIBNOTIFY_PKGCONFIG" != ""; then
    LIBNOTIFY_PKGCONFIG="libnotify >= $LIBNOTIFY_VERSION_MAJOR.$LIBNOTIFY_VERSION_MINOR.$LIBNOTIFY_VERSION_REVISION"
  fi
else
  echo
  echo "**Error**: Cannot detect libnotify version."
  echo "Do you have installed the libnotify development (libnotify-dev) package?"
  exit 1
fi

dnl Checking librarie with pkg-config
dnl Use "pkg-config --modversion module" to find version.

dnl Detect the gtk version to use
PKG_CHECK_MODULES([GTK3], [gtk+-3.0],
   [
    GTK_CFLAGS="$GTK3_CFLAGS"
    GTK_LIBS="$GTK3_LIBS"
    AC_DEFINE(HAVE_GTK3, 1, [Have GTK+ 3.0])
   ],
   [PKG_CHECK_MODULES([GTK2], [gtk+-2.0 >= 2.12],
       [
        GTK_CFLAGS="$GTK2_CFLAGS"
    	GTK_LIBS="$GTK2_LIBS"
       	AC_DEFINE([HAVE_GTK2], [1], [Have GTK+ 2.0])
       ])
   ])

PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.16.0)
PKG_CHECK_MODULES(GOBJECT, gobject-2.0 >= 2.16.0)
PKG_CHECK_MODULES(GIO, gio-2.0 >= 2.16.0)
PKG_CHECK_MODULES(DBUS_GLIB, dbus-glib-1 >= 0.74)

PKG_CHECK_MODULES(LIBNOTIFY, libnotify >= 0.4)
PKG_CHECK_MODULES(SQLITE3, sqlite3 >= 3.4)
PKG_CHECK_MODULES(LIBCURL, libcurl >= 7.18)

if test "$LIBVLC_PKGCONFIG" != ""; then
	PKG_CHECK_MODULES(LIBVLC, $LIBVLC_PKGCONFIG)
fi

LIBDBEVOLUTION_CFLAGS="$GTK_CFLAGS $GLIB_CFLAGS $GOBJECT_CFLAGS"
LIBDBEVOLUTION_LDFLAGS="$GTK_LIBS $GLIB_LIBS $GOBJECT_LIBS"
AC_SUBST(LIBDBEVOLUTION_CFLAGS)
AC_SUBST(LIBDBEVOLUTION_LDFLAGS)
echo "LIBDBEVOLUTION_CFLAGS " $LIBDBEVOLUTION_CFLAGS
echo "LIBDBEVOLUTION_LDFLAGS " $LIBDBEVOLUTION_LDFLAGS
                             
LIBVLCGTK_CFLAGS="$GTK_CFLAGS $LIBVLC_CFLAGS"
LIBVLCGTK_LDFLAGS="$GTK_LIBS $LIBVLC_LIBS"
AC_SUBST(LIBVLCGTK_CFLAGS)
AC_SUBST(LIBVLCGTK_LDFLAGS)
echo "LIBVLCGTK_CFLAGS " $LIBVLCGTK_CFLAGS
echo "LIBVLCGTK_LDFLAGS " $LIBVLCGTK_LDFLAGS
                             
FREETUXTV_CFLAGS="$GTK_CFLAGS $GLIB_CFLAGS $GIO_CFLAGS $DBUS_GLIB_CFLAGS $LIBNOTIFY_CFLAGS $SQLITE3_CFLAGS $LIBCURL_CFLAGS"
FREETUXTV_LDFLAGS="$GTK_LIBS $GLIB_LIBS $GIO_LIBS $DBUS_GLIB_LIBS $LIBNOTIFY_LIBS $SQLITE3_LIBS $LIBCURL_LIBS"
AC_SUBST(FREETUXTV_CFLAGS)
AC_SUBST(FREETUXTV_LDFLAGS)
echo "FREETUXTV_CFLAGS " $FREETUXTV_CFLAGS
echo "FREETUXTV_LDFLAGS " $FREETUXTV_LDFLAGS

AC_CHECK_LIB(vlc, libvlc_new)
AC_CHECK_LIB(gthread-2.0, g_thread_init)
AC_CHECK_LIB(sqlite3, sqlite3_open)
AC_CHECK_LIB(curl, curl_easy_init)
AC_CHECK_LIB(notify, notify_notification_new)

dnl ***************************************************************************
dnl Checks for header files.
dnl ***************************************************************************
AC_HEADER_STDC
AC_CHECK_HEADERS(stdio.h string.h stdlib.h)

dnl ***************************************************************************
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ***************************************************************************
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_FUNC_MALLOC
AC_FUNC_REALLOC

dnl ***************************************************************************
dnl Checks for library functions.
dnl ***************************************************************************
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strcspn])
AC_CHECK_FUNCS([strncpy])
AC_CHECK_FUNCS([strcmp])

AC_OUTPUT([
        Makefile
        po/Makefile.in
        src/Makefile
        data/Makefile
        data/icons/Makefile
        data/images/Makefile
        data/images/channels/Makefile
        data/images/groups/Makefile
        data/menu/Makefile
        data/ui/Makefile
        lib/Makefile
        lib/libdbevolution/Makefile
        lib/libvlc-gtk/Makefile
])
