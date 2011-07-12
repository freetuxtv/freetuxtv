
lib_LTLIBRARIES = \
	libdbevolution.la

libdbevolution_la_SOURCES = \
	db-evolution-instance.h \
	db-evolution-instance.c

libdbevolution_la_LDFLAGS = \
	-version-info $(LIBDBEVOLUTION_LT_VERSION) \
	-no-undefined \
	$(AM_LDFLAGS)

libdbevolution_la_LIBADD = \
	$(LIBDBEVOLUTION_LDFLAGS)

libdbevolutionincludedir = $(includedir)/libdbevolution
libdbevolutioninclude_HEADERS = \
	db-evolution-instance.h

AM_CPPFLAGS = \
	$(LIBDBEVOLUTION_CFLAGS)


## File created by the gnome-build tools

