
lib_LTLIBRARIES = \
	libvlc-gtk.la

libvlc_gtk_la_SOURCES = \
	gtk-libvlc-include.h \
	gtk-libvlc-instance.h \
	gtk-libvlc-instance.c \
	gtk-libvlc-media.h \
	gtk-libvlc-media.c \
	gtk-libvlc-media-player.h \
	gtk-libvlc-media-player.c \
	gtk-libvlc-private.h

libvlc_gtk_la_LDFLAGS = \
	-version-info $(LIBVLCGTK_LT_VERSION) \
	-no-undefined \
	$(AM_LDFLAGS)

libvlc_gtk_la_LIBADD = \
	$(LIBVLCGTK_LDFLAGS)

libvlcgtkincludedir = $(includedir)/libvlc-gtk
libvlcgtkinclude_HEADERS = \
	gtk-libvlc-include.h \
	gtk-libvlc-instance.h \
	gtk-libvlc-media.h \
	gtk-libvlc-media-player.h

AM_CPPFLAGS = \
	-I$(top_srcdir)/lib/ \
	$(LIBVLCGTK_CFLAGS)
