bin_PROGRAMS = freetuxtv

freetuxtv_SOURCES = \
  freetuxtv-callbacks.c \
  freetuxtv-callbacks.h \
  freetuxtv-channel.c \
  freetuxtv-channel.h \
  freetuxtv-channels-group.c \
  freetuxtv-channels-group.h \
  freetuxtv-channels-list.c \
  freetuxtv-channels-list.h \
  freetuxtv-channels-list-window.c \
  freetuxtv-channels-list-window.h \
  freetuxtv-logos-list-window.c \
  freetuxtv-logos-list-window.h \
  freetuxtv-main-window.c \
  freetuxtv-main-window.h \
  freetuxtv-player.c \
  freetuxtv-player.h \
  lib-m3uparser.c \
  lib-m3uparser.h \
  main.c

# For libglade2 signals
freetuxtv_LDFLAGS = -export-dynamic

freetuxtv_LDADD = $(DEPS_LIBS)

AM_CPPFLAGS = $(DEPS_CFLAGS)
