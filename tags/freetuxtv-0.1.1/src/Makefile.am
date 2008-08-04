bin_PROGRAMS = freetuxtv

freetuxtv_SOURCES = \
  freetuxtv-app.h \
  freetuxtv-channel.c \
  freetuxtv-channel.h \
  freetuxtv-channels-group.c \
  freetuxtv-channels-group.h \
  freetuxtv-channels-list.c \
  freetuxtv-channels-list.h \
  freetuxtv-player.c \
  freetuxtv-player.h \
  freetuxtv-window-main.c \
  freetuxtv-window-main.h \
  lib-m3uparser.c \
  lib-m3uparser.h \
  main.c

# For libglade2 signals
freetuxtv_LDFLAGS = -export-dynamic

freetuxtv_LDADD = $(DEPS_LIBS)

AM_CPPFLAGS = $(DEPS_CFLAGS) \
              -DLOCALE_DIR=\""$(datadir)/locale"\"
