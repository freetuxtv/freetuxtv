## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = lib po data src

freetuxtvdocdir = ${pkgdatadir}/doc
freetuxtvdoc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(freetuxtvdoc_DATA) \
	$(INTLTOOL_FILES) \
	autogen.sh

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(freetuxtvdocdir)
