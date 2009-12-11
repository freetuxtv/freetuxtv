SUBDIRS = images menu

xml_in_files = channels_groups.xml.in
xmldir = $(datadir)/freetuxtv
xml_DATA = $(xml_in_files:.xml.in=.xml)
@INTLTOOL_XML_NOMERGE_RULE@

freetuxtvdatadir = $(datadir)/freetuxtv
freetuxtvdata_DATA = sqlite3-create-tables.sql channels_logos.xml freetuxtv.glade

EXTRA_DIST = \
        $(freetuxtvdata_DATA) \
        $(xml_in_files)

DISTCLEANFILES = \
        $(xml_DATA)
