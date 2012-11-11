## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(FREETUXTV_CFLAGS) \
	-I$(top_srcdir)/lib/

AM_CFLAGS =\
	 -Wall

bin_PROGRAMS = freetuxtv

freetuxtv_SOURCES = \
	gtk-builder-dialog.c \
	gtk-builder-dialog.h \
	gtk-builder-object.c \
	gtk-builder-object.h \
	gtk-builder-widget.c \
	gtk-builder-widget.h \
	gtk-builder-window.c \
	gtk-builder-window.h \
	gtk-date-time-picker.c \
	gtk-date-time-picker.h \
	gtk-progress-dialog.c \
	gtk-progress-dialog.h \
	freetuxtv-app.h \
	freetuxtv-cclosure-marshal.c \
	freetuxtv-cclosure-marshal.h \
	freetuxtv-cellrenderer-channelslist.c \
	freetuxtv-cellrenderer-channelslist.h \
	freetuxtv-cellrenderer-recordingslist.c \
	freetuxtv-cellrenderer-recordingslist.h \
	freetuxtv-channel-infos.c \
	freetuxtv-channel-infos.h \
	freetuxtv-channels-group-infos.c \
	freetuxtv-channels-group-infos.h \
	freetuxtv-channels-list.c \
	freetuxtv-channels-list.h \
	freetuxtv-db-sync.c \
	freetuxtv-db-sync.h \
	freetuxtv-fileutils.c \
	freetuxtv-fileutils.h \
	freetuxtv-gladexml.h \
	freetuxtv-i18n.h \
	freetuxtv-models.c \
	freetuxtv-models.h \
	freetuxtv-player-error-dialog.c \
	freetuxtv-player-error-dialog.h \
	freetuxtv-recording-infos.c \
	freetuxtv-recording-infos.h \
	freetuxtv-recordings-list.c \
	freetuxtv-recordings-list.h \
	freetuxtv-tv-channel-infos.c \
	freetuxtv-tv-channel-infos.h \
	freetuxtv-tv-channels-list.c \
	freetuxtv-tv-channels-list.h \
	freetuxtv-utils.c \
	freetuxtv-utils.h \
	freetuxtv-window-add-channels-group.c \
	freetuxtv-window-add-channels-group.h \
	freetuxtv-window-add-recording.c \
	freetuxtv-window-add-recording.h \
	freetuxtv-window-channel-properties.c \
	freetuxtv-window-channel-properties.h \
	freetuxtv-window-main.c \
	freetuxtv-window-main.h \
	freetuxtv-window-tv-channels-database.c \
	freetuxtv-window-tv-channels-database.h \
	lib-gmmkeys.c \
	lib-gmmkeys.h \
	lib-m3uparser.c \
	lib-m3uparser.h \
	main.c

freetuxtv_LDFLAGS = 

freetuxtv_LDADD = $(FREETUXTV_LDFLAGS) \
	../lib/libvlc-gtk/libvlc-gtk.la \
	../lib/libdbevolution/libdbevolution.la
