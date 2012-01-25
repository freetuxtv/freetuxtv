# Internationalization of XML file
xml_in_files = channels_groups.xml.in
xmldir = $(pkgdatadir)
xml_DATA = $(xml_in_files:.xml.in=.xml)
@INTLTOOL_XML_NOMERGE_RULE@

# Other data file to install
freetuxtvdata_DATA = \
	sqlite3-create-tables.sql \
	tv_channels.xml \
	splashscreen.png

freetuxtvdatadir = \
	$(pkgdatadir)

EXTRA_DIST = \
	$(freetuxtvdata_DATA) \
	$(xml_in_files)

DISTCLEANFILES = \
    $(xml_DATA)

SUBDIRS = \
	icons\
	menu \
	images \
	ui

# Remove data directory on uninstall
#uninstall-local:
#-rm -r $(freetuxtvdatadir)
