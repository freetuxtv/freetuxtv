SUBDIRS = libvlc-gtk

bin_PROGRAMS = freetuxtv

freetuxtv_SOURCES = \
  freetuxtv-app.h \
  freetuxtv-cellrenderer-channelslist.c \
  freetuxtv-cellrenderer-channelslist.h \
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
  freetuxtv-i18n.h \
  freetuxtv-logos-list.c \
  freetuxtv-logos-list.h \
  freetuxtv-models.c \
  freetuxtv-models.h \
  freetuxtv-recordings-list.c \
  freetuxtv-recordings-list.h \
  freetuxtv-utils.c \
  freetuxtv-utils.h \
  freetuxtv-window-main.c \
  freetuxtv-window-main.h \
  lib-gmmkeys.c \
  lib-gmmkeys.h \
  lib-m3uparser.c \
  lib-m3uparser.h \
  main.c

freetuxtv_CPPFLAGS = \
  $(DEPS_CFLAGS) \
	-I$(srcdir)/libvlc-gtk \
  -DLOCALE_DIR=\""$(datadir)/locale"\"

# For libglade2 signals
freetuxtv_LDFLAGS = -export-dynamic

freetuxtv_LDADD = $(DEPS_LIBS) \
  libvlc-gtk/libvlc-gtk.la
