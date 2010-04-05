xml_in_files = channels_groups.xml.in
xmldir = $(pkgdatadir)
xml_DATA = $(xml_in_files:.xml.in=.xml)
@INTLTOOL_XML_NOMERGE_RULE@

freetuxtvdata_DATA = \
	sqlite3-create-tables.sql\
	channels_logos.xml\
	freetuxtv.glade

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
	images
