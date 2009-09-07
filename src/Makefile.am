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
  freetuxtv-i18n.h \
  freetuxtv-logos-list.c \
  freetuxtv-logos-list.h \
  freetuxtv-recordings-list.c \
  freetuxtv-recordings-list.h \
  freetuxtv-utils.c \
  freetuxtv-utils.h \
  freetuxtv-window-main.c \
  freetuxtv-window-main.h \
  gtk-libvlc-include.h \
  gtk-libvlc-instance.c \
  gtk-libvlc-instance.h \
  gtk-libvlc-media.c \
  gtk-libvlc-media.h \
  gtk-libvlc-media-player.c \
  gtk-libvlc-media-player.h \
  lib-gmmkeys.c \
  lib-gmmkeys.h \
  lib-m3uparser.c \
  lib-m3uparser.h \
  main.c

# For libglade2 signals
freetuxtv_LDFLAGS = -export-dynamic

freetuxtv_LDADD = $(DEPS_LIBS)

AM_CPPFLAGS = $(DEPS_CFLAGS) \
              -DLOCALE_DIR=\""$(datadir)/locale"\"
