dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(freetuxtv, 0.5.0, http://code.google.com/p/freetuxtv/)

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

dnl Use "pkg-config --modversion module" to find version.
PKG_CHECK_MODULES(FREETUXTV, gtk+-2.0 >= 2.12 
                             glib-2.0 >= 2.16.0
                             gio-2.0 >= 2.16.0
                             dbus-glib-1 >= 0.74
                             libnotify >= 0.4
                             sqlite3 >= 3.4
                             libcurl >= 7.18)

PKG_CHECK_MODULES(LIBVLCGTK, gtk+-2.0 >= 2.12
                             $LIBVLC_PKGCONFIG)
                             
PKG_CHECK_MODULES(LIBDBEVOLUTION, glib-2.0 >= 2.16.0)

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
        lib/libvlc-gtk/Makefile
        lib/libdbevolution/Makefile
])
