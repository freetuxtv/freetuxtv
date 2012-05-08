
# UI file to install
freetuxtvui_DATA = \
	addchannelsgroups.glade \
	addrecording.glade \
	channelproperties.glade \
	freetuxtv.glade
freetuxtvuidir = $(pkgdatadir)/ui

EXTRA_DIST = \
	$(freetuxtvui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(freetuxtvuidir)
