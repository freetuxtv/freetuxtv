/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * FreetuxTV is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FreetuxTV is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <gdk/gdkx.h>

#include "freetuxtv-window-main.h"
#include "freetuxtv-window-add-recording.h"
#include "freetuxtv-window-add-channels-group.h"
#include "freetuxtv-window-tv-channels-database.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-utils.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-tv-channels-list.h"
#include "freetuxtv-recordings-list.h"
#include "freetuxtv-db-sync.h"
#include "freetuxtv-models.h"

static void
windowmain_add_accelerator (GtkAccelGroup *p_accel_group,
                            const gchar *accelerator, const gchar *accel_path,
                            GCallback callback, gpointer user_data);

static void
on_windowmain_trayicon_activate(GtkStatusIcon *status_icon, gpointer user_data);

static void
on_windowmain_trayicon_popupmenu(GtkStatusIcon *status_icon, guint button,
                                 guint activate_time, gpointer user_data);

static gboolean
on_windowmain_deleteevent (GtkWidget *widget, GdkEvent *event, gpointer *data);

static void
on_windowmain_menuitempreferences_activate (GtkMenuItem *menuitem,
                                            gpointer user_data);

static void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
                                          gpointer user_data);

static void
on_windowmain_menuitemupdatetvchannels_activate (GtkMenuItem *menuitem,
                                                 gpointer user_data);

static void 
on_windowmain_menuitemmute_activate (GtkMenuItem *menuitem,
                                     gpointer user_data);

static void
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
                                     gpointer user_data);

static void
on_windowmain_menuitemaboutdialog_activate (GtkMenuItem *menuitem,
                                            gpointer user_data);

static void
on_windowmain_menuitemdeinterlace_change (GtkMenuItem *menuitem,
                                          const gchar* mode,
                                          gpointer user_data);

static void
on_windowmain_menuitemdeinterlacedisable_activate (GtkMenuItem *menuitem,
                                                   gpointer user_data);

static void
on_windowmain_menuitemdeinterlaceblend_activate (GtkMenuItem *menuitem,
                                                 gpointer user_data);

static void
on_windowmain_menuitemdeinterlacebob_activate (GtkMenuItem *menuitem,
                                               gpointer user_data);

static void
on_windowmain_menuitemdeinterlacediscard_activate (GtkMenuItem *menuitem,
                                                   gpointer user_data);

static void
on_windowmain_menuitemdeinterlacelinear_activate (GtkMenuItem *menuitem,
                                                  gpointer user_data);

static void
on_windowmain_menuitemdeinterlacemean_activate (GtkMenuItem *menuitem,
                                                gpointer user_data);

static void
on_windowmain_menuitemdeinterlacex_activate (GtkMenuItem *menuitem,
                                             gpointer user_data);

static gboolean
on_windowmain_valuechanged (GtkRange *range, GtkScrollType scroll,
                            gdouble value, gpointer user_data);

static void
on_windowmain_buttonclearfilter_clicked (GtkButton *button,
                                         gpointer user_data);

static void
on_windowmain_buttongotocurrent_clicked (GtkButton *button,
                                         gpointer user_data);

static void
on_windowmain_buttonprevious_clicked (GtkButton *button,
                                      gpointer user_data);

static void
on_windowmain_buttonnext_clicked (GtkButton *button,
                                  gpointer user_data);

static void
on_windowmain_buttonstop_clicked (GtkButton *button,
                                  gpointer user_data);

static void
on_windowmain_buttonrecord_clicked (GtkButton *button,
                                    gpointer user_data);

static void
on_windowmain_buttonplaypause_clicked (GtkButton *button,
                                       gpointer user_data);

static void
on_windowmain_buttonfullscreen_clicked (GtkButton *button,
                                        gpointer user_data);

static void
on_windowmain_buttonminimode_clicked (GtkButton *button,
                                      gpointer user_data);

static void
on_windowmain_entryfilter_changed (GtkEntry *entry,
                                   gpointer user_data);

static gboolean
on_windowmain_entryfilter_focusin (GtkWidget *widget, GdkEventFocus *event,
                                   gpointer user_data);

static gboolean
on_windowmain_entryfilter_focusout (GtkWidget *widget, GdkEventFocus *event,
                                    gpointer user_data);

static void
on_windowmain_volumecontrol_value_changed (GtkRange *range,
                                           gpointer user_data);

static void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
                                            gpointer user_data);

static void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
                                           gpointer user_data);

static void
on_dialogpreferences_directoryrecordings_changed (GtkFileChooser *chooser,
                                                  gpointer user_data);

static void
on_dialogpreferences_radiotranscoding_toggled (GtkToggleButton *togglebutton,
                                               gpointer user_data);

static void
on_dialogpreferences_transcodingformats_changed (GtkComboBox *widget,
                                                 gpointer user_data);

static void
on_dialogpreferences_radioproxy_toggled (GtkToggleButton *togglebutton,
                                         gpointer user_data);

static void
on_dialogpreferences_checkuseauth_toggled (GtkToggleButton *togglebutton,
                                     	   gpointer user_data);

static void
on_dialogpreferences_response (GtkDialog *dialog,
                               gint response_id,
                               gpointer   user_data);

static void
on_dialoggroupproperties_response (GtkDialog *dialog,
                                   gint response_id,
                                   gpointer user_data);

static void
on_aboutdialog_response (GtkDialog *dialog,
                         gint response_id,
                         gpointer user_data);

static void
dialogpreferences_update_view(FreetuxTVApp *app);

static gboolean
on_accel_playpause (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                    GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_tooglefullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                           GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_unfullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                       GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_volumeup (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                   GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_volumedown (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
    GdkModifierType modifier, gpointer user_data);

static void
on_channels_group_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data);

void
windowmain_init(FreetuxTVApp *app)
{
	GtkWidget *widget;
	GtkWidget *pMenuBar = NULL;

	app->widget.pTrayIcon = gtk_status_icon_new_from_icon_name ("freetuxtv");
	gtk_status_icon_set_visible(app->widget.pTrayIcon, TRUE);
	g_signal_connect(G_OBJECT(app->widget.pTrayIcon), "activate",
	                 G_CALLBACK(on_windowmain_trayicon_activate), app);
	g_signal_connect(G_OBJECT(app->widget.pTrayIcon), "popup-menu",
	                 G_CALLBACK(on_windowmain_trayicon_popupmenu), app);
	
	// Window accelerators
	app->widget.pAccelGroup = gtk_accel_group_new ();
	windowmain_add_accelerator (app->widget.pAccelGroup, "F11", "<FreetuxTV>/Fullscreen",
	                            G_CALLBACK (on_accel_tooglefullscreen), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "f", "<FreetuxTV>/Fullscreen",
	                            G_CALLBACK (on_accel_tooglefullscreen), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "Escape", "<FreetuxTV>/Unfullscreen",
	                            G_CALLBACK (on_accel_unfullscreen), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "space", "<FreetuxTV>/PlayPause",
	                            G_CALLBACK (on_accel_playpause), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "<Control>Up", "<FreetuxTV>/VolumeUp",
	                            G_CALLBACK (on_accel_volumeup), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "<Control>Down", "<FreetuxTV>/VolumeDown",
	                            G_CALLBACK (on_accel_volumedown), app);

	// Initialize menu bar	
	pMenuBar = gtk_menu_bar_new ();
	GtkWidget *pMenu = NULL;
	GtkWidget *pMenuItem = NULL;
	GtkWidget *pSubmenu = NULL;

	GSList* pRadioGroup = NULL;

	// Menu : FreetuxTV
	pMenuItem = gtk_menu_item_new_with_mnemonic (_("_FreetuxTV"));
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenuBar), pMenuItem);
	pMenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (pMenuItem), pMenu);
	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES, NULL);	
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitempreferences_activate),
	                 app);
	pMenuItem = gtk_separator_menu_item_new();	
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);	
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemquit_activate),
	                 app);
	// End Menu : FreetuxTV

	// Menu : Channels
	pMenuItem = gtk_menu_item_new_with_mnemonic (_("_Channels"));
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenuBar), pMenuItem);
	pMenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (pMenuItem), pMenu);
	pMenuItem = gtk_menu_item_new_with_mnemonic (_("_Add a group"));
	gtk_widget_set_tooltip_text (GTK_WIDGET(pMenuItem),
	                             _("Add/create a channels group from different sources"));
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemgroupsadd_activate),
	                 app);
	pMenuItem = gtk_menu_item_new_with_mnemonic (_("_Synchronize TV channels database"));
	gtk_widget_set_tooltip_text (GTK_WIDGET(pMenuItem),
	                             _("Synchronize the TV channels list in the local database from the XML database"));
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemupdatetvchannels_activate),
	                 app);
	// End Menu : Channels

	// Menu : Video
	pMenuItem = gtk_menu_item_new_with_mnemonic (_("_Video"));
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenuBar), pMenuItem);
	pMenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (pMenuItem), pMenu);
	// SubMenu : Deinterlace
	pMenuItem = gtk_menu_item_new_with_mnemonic (_("_Deinterlace"));
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	pSubmenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (pMenuItem), pSubmenu);
	
	pMenuItem = gtk_radio_menu_item_new_with_label (pRadioGroup, _("Disable"));
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemdeinterlacedisable_activate),
	                 app);
	pRadioGroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(pMenuItem));
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
	app->widget.pMenuDeinterlaceDisable = pMenuItem;

	pMenuItem = gtk_separator_menu_item_new();
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
 
	pMenuItem = gtk_radio_menu_item_new_with_label (pRadioGroup, _("Blend"));
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemdeinterlaceblend_activate),
	                 app);
	pRadioGroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(pMenuItem));
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
	app->widget.pMenuDeinterlaceBlend = pMenuItem;
	
	pMenuItem = gtk_radio_menu_item_new_with_label (pRadioGroup, _("Bob"));
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemdeinterlacebob_activate),
	                 app);
	pRadioGroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(pMenuItem));
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
	app->widget.pMenuDeinterlaceBob = pMenuItem;
	
	pMenuItem = gtk_radio_menu_item_new_with_label (pRadioGroup, _("Discard"));
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemdeinterlacediscard_activate),
	                 app);
	pRadioGroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(pMenuItem));
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
	app->widget.pMenuDeinterlaceDiscard = pMenuItem;
	
	pMenuItem = gtk_radio_menu_item_new_with_label (pRadioGroup, _("Linear"));
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemdeinterlacelinear_activate),
	                 app);
	pRadioGroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(pMenuItem));
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
	app->widget.pMenuDeinterlaceLinear = pMenuItem;
	
	pMenuItem = gtk_radio_menu_item_new_with_label (pRadioGroup, _("Mean"));
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemdeinterlacemean_activate),
	                 app);
	pRadioGroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(pMenuItem));
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
	app->widget.pMenuDeinterlaceMean = pMenuItem;
	
	pMenuItem = gtk_radio_menu_item_new_with_label (pRadioGroup, _("X"));
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemdeinterlacex_activate),
	                 app);
	pRadioGroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(pMenuItem));
	gtk_menu_shell_append (GTK_MENU_SHELL (pSubmenu), pMenuItem);
	app->widget.pMenuDeinterlaceX = pMenuItem;
	// End SubMenu : Deinterlace
	// End Menu : Video

	// Menu : Help
	pMenuItem = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenuBar), pMenuItem);
	pMenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (pMenuItem), pMenu);
	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);	
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemaboutdialog_activate),
	                 app);
	// End Menu : Help

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_menubox");
	gtk_box_pack_start (GTK_BOX (widget), GTK_WIDGET (pMenuBar), TRUE, TRUE, 0);
	gtk_widget_show_all(pMenuBar);

	// Initialize signals for windowmain
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain");
	g_signal_connect(G_OBJECT(widget),
	                 "delete-event",
	                 G_CALLBACK(on_windowmain_deleteevent),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonclearfilter");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonclearfilter_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_entryfilter");
	g_signal_connect(G_OBJECT(widget),
	                 "changed",
	                 G_CALLBACK(on_windowmain_entryfilter_changed),
	                 app);
	g_signal_connect(G_OBJECT(widget),
	                 "focus-in-event",
	                 G_CALLBACK(on_windowmain_entryfilter_focusin),
	                 app);
	g_signal_connect(G_OBJECT(widget),
	                 "focus-out-event",
	                 G_CALLBACK(on_windowmain_entryfilter_focusout),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_scaletime");
	g_signal_connect(G_OBJECT(widget),
	                 "change-value",
	                 G_CALLBACK(on_windowmain_valuechanged),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttongotocurrent");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttongotocurrent_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonprevious");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonprevious_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonnext");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonnext_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonplaypause");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonplaypause_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonstop");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonstop_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonrecord");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonrecord_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
	                 "value-changed",
	                 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonfullscreen");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonfullscreen_clicked),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_buttonminimode");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowmain_buttonminimode_clicked),
	                 app);

	// Initialize signals for windowminimode
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	g_signal_connect(G_OBJECT(widget),
	                 "delete-event",
	                 G_CALLBACK(on_windowmain_deleteevent),
	                 app);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode_buttonnormalmode");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowminimode_buttonnormalmode_clicked),
	                 app);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode_buttonstayontop");
	g_signal_connect(G_OBJECT(widget),
	                 "clicked",
	                 G_CALLBACK(on_windowminimode_buttonstayontop_clicked),
	                 app);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
	                 "value-changed",
	                 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
	                 app);

	// Initialize signals for dialogpreferences
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
	                        "gtk-cancel", GTK_RESPONSE_CANCEL,
	                        "gtk-apply", GTK_RESPONSE_APPLY, NULL);

	g_signal_connect(G_OBJECT(widget),
	                 "response",
	                 G_CALLBACK(on_dialogpreferences_response),
	                 app);
	g_signal_connect(G_OBJECT(widget),
	                 "delete-event",
	                 G_CALLBACK(gtk_widget_hide_on_delete),
	                 NULL);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences_directoryrecordings");	
	g_signal_connect(G_OBJECT(widget),
	                 "selection-changed",
	                 G_CALLBACK(on_dialogpreferences_directoryrecordings_changed),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences_radiotranscodingno");	
	g_signal_connect(G_OBJECT(widget),
	                 "toggled",
	                 G_CALLBACK(on_dialogpreferences_radiotranscoding_toggled),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences_radiotranscodingpredefinedformats");	
	g_signal_connect(G_OBJECT(widget),
	                 "toggled",
	                 G_CALLBACK(on_dialogpreferences_radiotranscoding_toggled),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences_transcodingformats");	
	g_signal_connect(G_OBJECT(widget),
	                 "changed",
	                 G_CALLBACK(on_dialogpreferences_transcodingformats_changed),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences_radioproxyno");	
	g_signal_connect(G_OBJECT(widget),
	                 "toggled",
	                 G_CALLBACK(on_dialogpreferences_radioproxy_toggled),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences_radioproxymanual");	
	g_signal_connect(G_OBJECT(widget),
	                 "toggled",
	                 G_CALLBACK(on_dialogpreferences_radioproxy_toggled),
	                 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialogpreferences_checkuseauth");	
	g_signal_connect(G_OBJECT(widget),
	                 "toggled",
	                 G_CALLBACK(on_dialogpreferences_checkuseauth_toggled),
	                 app);

	// Initialize signals for dialoggroupproperties
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "dialoggroupproperties");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
	                        "gtk-cancel", GTK_RESPONSE_CANCEL, "gtk-apply", GTK_RESPONSE_APPLY, NULL);	
	g_signal_connect(G_OBJECT(widget),
	                 "response",
	                 G_CALLBACK(on_dialoggroupproperties_response),
	                 app);
	g_signal_connect(G_OBJECT(widget),
	                 "delete-event",
	                 G_CALLBACK(gtk_widget_hide_on_delete),
	                 NULL);

	// Initialize signals for aboutdialog
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                "aboutdialog");

	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(widget), VERSION);

	g_signal_connect(G_OBJECT(widget),
	                 "response",
	                 G_CALLBACK(on_aboutdialog_response),
	                 NULL);
	g_signal_connect(G_OBJECT(widget),
	                 "delete-event",
	                 G_CALLBACK(gtk_widget_hide_on_delete),
	                 NULL);

	windowmain_enable_accelerators(app, TRUE);
}

void
windowmain_dispose(FreetuxTVApp *app)
{
	windowmain_enable_accelerators(app, FALSE);

#if GTK_API_VERSION == 3
	g_object_unref (app->widget.pAccelGroup);
#else
	gtk_accel_group_unref (app->widget.pAccelGroup);
#endif
	app->widget.pAccelGroup = NULL;

	if(app->widget.pTrayIcon){
		g_object_unref(app->widget.pTrayIcon);
		app->widget.pTrayIcon = NULL;
	}
}

void
windowmain_display_buttons (FreetuxTVApp *app, FreetuxTVWindowMode mode)
{
	GtkWidget *widget;
	gboolean sensitive;
	GtkWidget *image = NULL;
	// Button previous
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonprevious");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			if(app->current.pPathChannel != NULL){
				sensitive = TRUE;		
			}else{
				sensitive = FALSE;	
			}
			break;
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;
			break;
		default:
			sensitive = FALSE;
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);

	// Button next
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonnext");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			if(app->current.pPathChannel != NULL){
				sensitive = TRUE;		
			}else{
				sensitive = FALSE;	
			}
			break;
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;		
			break;
		default:
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);

	// Button play/pause
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonplaypause");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			if(app->current.pPathChannel == NULL){
				sensitive = FALSE;
			}else{
				sensitive = TRUE;
			}
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_BUTTON);
			break;
		case WINDOW_MODE_PAUSED :
			sensitive = TRUE;
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_BUTTON);
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON);
			break;
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON);
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
	if(image){
		gtk_button_set_image (GTK_BUTTON(widget), image);
	}

	// Button stop
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonstop");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
		case WINDOW_MODE_RECORDING :
			sensitive = TRUE;		
			break;
		default:
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);

	// Button record
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonrecord");
	switch(mode){
		case WINDOW_MODE_STOPPED :
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;		
			break;
		default:
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
}

void
windowmain_show_error (FreetuxTVApp *app, gchar *msg)
{
	GtkWidget *windowmain;
	GtkWidget* dialog;

	windowmain = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                   "windowmain");

	dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
	                                GTK_DIALOG_MODAL, 
	                                GTK_MESSAGE_ERROR,
	                                GTK_BUTTONS_OK,
	                                msg, NULL);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void
windowmain_show_gerror (FreetuxTVApp *app, GError* error)
{
	g_return_if_fail(error != NULL);
	windowmain_show_error (app, error->message);
}

void
windowmain_statusbar_push (FreetuxTVApp *app, gchar *context, gchar *msg)
{
	int context_id;

	GtkWidget *statusbar;
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                  "windowmain_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
	                                          context);
	gtk_statusbar_push (GTK_STATUSBAR(statusbar), 
	                    context_id,
	                    msg);
	
	while (g_main_context_iteration(NULL, FALSE)){}
}


void
windowmain_statusbar_pop (FreetuxTVApp *app, gchar *context)
{	
	int context_id;

	GtkWidget *statusbar;
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                  "windowmain_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
	                                          context);
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
	                   context_id);

	while (g_main_context_iteration(NULL, FALSE)){}
}

void
windowmain_update_statusbar_infos (FreetuxTVApp *app)
{
	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_channelscount");

	gint channels_count;
	channels_count = channels_list_get_channels_count (app);

	gchar *str;
	str = g_strdup_printf("%d", channels_count);
	gtk_label_set_text(GTK_LABEL(widget), str);
	g_free(str);	
}

void
windowmain_timebar_update (FreetuxTVApp *app, glong time_ms, glong length_ms, gfloat position)
{
	GtkWidget *widget;

	gdouble time_s = time_ms/1000.0;
	gdouble length_s = length_ms/1000.0;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_labeltime");
	gtk_label_set_text(GTK_LABEL(widget), format_time2((glong)time_s));

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_labellength");
	gtk_label_set_text(GTK_LABEL(widget), format_time2((glong)length_s));	

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "adjustment_time");
	gtk_adjustment_set_value (GTK_ADJUSTMENT(widget), position);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_hboxtimebar");
	if(gtk_libvlc_media_player_is_seekable(app->player, NULL)){
		gtk_widget_set_sensitive(widget, TRUE);
	}else{
		gtk_widget_set_sensitive(widget, FALSE);
	}
}

void
windowmain_show_dialog_addrecordings(FreetuxTVApp *app, FreetuxTVChannelInfos* pChannelInfos, GError** error)
{
	g_return_if_fail (error != NULL);
	
	GtkWidget* pParent;
	pParent = (GtkWidget *) gtk_builder_get_object (app->gui, "windowmain");

	DBSync dbsync;
	
	// Create and show the window
	FreetuxTVWindowRecording* pWindowRecording;
	FreetuxTVRecordingInfos* pRecordingInfos = NULL;
	gint res;
	
	pWindowRecording = freetuxtv_window_recording_new (GTK_WINDOW(pParent), app, pChannelInfos);
	res = gtk_builder_dialog_run (GTK_BUILDER_DIALOG(pWindowRecording));

	if(res == GTK_RESPONSE_OK){
		pRecordingInfos = freetuxtv_window_recording_get_recording_infos (pWindowRecording);
	}

	// Destroy the window
	gtk_builder_widget_destroy(GTK_BUILDER_WIDGET(pWindowRecording));
	pWindowRecording = NULL;

	if(res == GTK_RESPONSE_OK){
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			freetuxtv_recording_infos_set_status(pRecordingInfos, FREETUXTV_RECORDING_STATUS_WAITING);

			recordings_list_add_recording (app, &dbsync, pRecordingInfos, error);
		}
		dbsync_close_db(&dbsync);

		if(*error == NULL){
			GTimeVal now;
			g_get_current_time (&now);
			gint64 now64 = g_time_val_to_int64(&now);

			if(freetuxtv_recording_infos_has_time(pRecordingInfos, now64)){
				freetuxtv_action_start_recording (app, pRecordingInfos, error);
			}
		}
	}

	if(pRecordingInfos){
		g_object_unref (pRecordingInfos);
		pRecordingInfos = NULL;
	}
}

void
windowmain_enable_accelerators(FreetuxTVApp *app, gboolean enable)
{
	g_return_if_fail(app != NULL);
	
	GtkWidget* widget;

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain");
	if(enable){
		if(!app->current.bIsAccelGroupActivated){
			gtk_window_add_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}else{
		if(app->current.bIsAccelGroupActivated){
			gtk_window_remove_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	if(enable){
		if(!app->current.bIsAccelGroupActivated){
			gtk_window_add_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}else{
		if(app->current.bIsAccelGroupActivated){
			gtk_window_remove_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}

	app->current.bIsAccelGroupActivated = enable;
}

static void
windowmain_add_accelerator (GtkAccelGroup *p_accel_group,
                           const gchar *accelerator, const gchar *accel_path,
                           GCallback callback, gpointer user_data)
{
	guint key;
	GdkModifierType mods;
	GClosure *closure = NULL;

	gtk_accelerator_parse (accelerator, &key, &mods);
	closure = g_cclosure_new (callback, user_data, NULL);
	gtk_accel_group_connect (p_accel_group, key, mods, GTK_ACCEL_VISIBLE, closure);
	gtk_accel_map_add_entry (accel_path, key, mods);
}

static void
on_windowmain_trayicon_activate(GtkStatusIcon *status_icon, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	GtkWidget *widget;
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                "windowmain");
	widget =  gtk_widget_get_toplevel(GTK_WIDGET(app->player));

	gboolean bIsVisible;

#if GTK_API_VERSION == 3
	bIsVisible = gtk_widget_get_visible(widget);
	if(bIsVisible){
		gtk_widget_set_visible(widget, FALSE);
	}else{
		gtk_widget_set_visible(widget, TRUE);
	}
#else
	bIsVisible = GTK_WIDGET_FLAGS (widget) & GTK_VISIBLE;
	if(bIsVisible){
		gtk_widget_hide(widget);
	}else{
		gtk_widget_show(widget);
	}
#endif
}

static void
on_windowmain_trayicon_popupmenu (GtkStatusIcon *status_icon, guint button,
                                  guint activate_time, gpointer user_data)
{
	GtkWidget *pMenu;
	GtkWidget *pMenuItem;
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	gboolean bVolumeDisabled;
	
	g_return_if_fail (status_icon != NULL);
	g_return_if_fail (GTK_IS_STATUS_ICON (status_icon));

	bVolumeDisabled = gtk_libvlc_media_player_get_mute (app->player, NULL);
	
	pMenu = gtk_menu_new();

	/* Create the menu items */
	pMenuItem = gtk_check_menu_item_new_with_label (_("Mute"));
#if GTK_API_VERSION == 3
#else
	gtk_check_menu_item_set_show_toggle (GTK_CHECK_MENU_ITEM(pMenuItem), FALSE);
#endif
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(pMenuItem), bVolumeDisabled);
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemmute_activate),
	                 app);
	gtk_widget_show(pMenuItem);

	pMenuItem = gtk_separator_menu_item_new();
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	gtk_widget_show(pMenuItem);
	
	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);	
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemquit_activate),
	                 app);
	gtk_widget_show(pMenuItem);
	
	gtk_widget_show(pMenu);
		
	gtk_menu_popup (GTK_MENU(pMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}


static gboolean
on_windowmain_deleteevent (GtkWidget *widget, GdkEvent *event, gpointer *data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;
	freetuxtv_quit (app, GTK_WINDOW(widget));
	return TRUE;
}

static void 
on_windowmain_menuitemmute_activate (GtkMenuItem *menuitem,
                                     gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GError* error = NULL;
	gboolean mute;

	mute = gtk_libvlc_media_player_get_mute (app->player, NULL);
	if(!error){
		gtk_libvlc_media_player_set_mute (app->player, !mute, NULL);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void 
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
                                     gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                "windowmain");
	
	freetuxtv_quit (app, GTK_WINDOW(widget));
}

static void
on_windowmain_menuitemaboutdialog_activate (GtkMenuItem *menuitem,
                                            gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GtkWidget *widget;
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                "aboutdialog");
	gtk_widget_show(widget);
}

static void
on_windowmain_buttonclearfilter_clicked (GtkButton *button,
                                         gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GtkWidget *entryfilter;
	entryfilter =  (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                     "windowmain_entryfilter");
	gtk_entry_set_text(GTK_ENTRY(entryfilter), "");

	GtkWidget *treeview;
	GtkTreeModel *model;
	treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                  "windowmain_treeviewchannelslist");

	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));

	gtk_tree_view_collapse_all (GTK_TREE_VIEW(treeview));
	channels_list_set_playing (app, app->current.pPathChannel);
}

static void
on_windowmain_buttongotocurrent_clicked (GtkButton *button,
                                         gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	channels_list_set_playing(app, app->current.pPathChannel);
}

static void
on_windowmain_buttonprevious_clicked (GtkButton *button,
                                      gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_prev (app, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_buttonnext_clicked (GtkButton *button,
                                  gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_next (app, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_buttonstop_clicked (GtkButton *button,
                                  gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	if(app->current.is_recording){
		freetuxtv_action_stop_recording(app, app->current.recording.pRecordingInfo, &error);
	}else{
		freetuxtv_action_stop (app, &error);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_buttonrecord_clicked (GtkButton *button,
                                    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;
	FreetuxTVChannelInfos* pChannelInfos;
	
	// Show properties to the channel corresponding to the path
	pChannelInfos = channels_list_get_channel (app, app->current.pPathChannel);

	windowmain_show_dialog_addrecordings (app, pChannelInfos, &error);
	
	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_buttonplaypause_clicked (GtkButton *button,
                                       gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_playpause (app, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_buttonfullscreen_clicked (GtkButton *button,
                                        gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_set_fullscreen (app->player, TRUE, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_buttonminimode_clicked (GtkButton *button,
                                      gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;	

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain");
	gtk_widget_hide(widget);

	// Display the mini mode window
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	gtk_widget_show(widget);
	gtk_window_set_keep_above (GTK_WINDOW(widget),
	                           app->config.windowminimode_stayontop);

	gtk_window_resize (GTK_WINDOW(widget),
	                   app->config.windowminimode_width,
	                   app->config.windowminimode_height);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode_buttonstayontop");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget),
	                              app->config.windowminimode_stayontop);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);
}

static void
on_windowmain_entryfilter_changed (GtkEntry *entry, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *treeview;
	GtkTreeModel *model;
	treeview = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                 "windowmain_treeviewchannelslist");

	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));

	const gchar* text;
	text = gtk_entry_get_text (entry);

	if(g_ascii_strcasecmp(text, "") == 0){
		gtk_tree_view_collapse_all (GTK_TREE_VIEW(treeview));
		channels_list_set_playing (app, app->current.pPathChannel);
	}else{
		gtk_tree_view_expand_all (GTK_TREE_VIEW(treeview));
	}
}

static gboolean
on_windowmain_entryfilter_focusin (GtkWidget *widget, GdkEventFocus *event,
                                   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	windowmain_enable_accelerators (app, FALSE);
	return FALSE;
}

static gboolean
on_windowmain_entryfilter_focusout (GtkWidget *widget, GdkEventFocus *event,
                                    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	windowmain_enable_accelerators (app, TRUE);
	return FALSE;
}

static void
on_windowmain_volumecontrol_value_changed (GtkRange *range, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	app->config.volume = gtk_range_get_value (range);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume, NULL);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_menuitempreferences_activate (GtkMenuItem *menuitem,
                                            gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences");
	gtk_widget_show(widget);

	GtkTreeModel *model;
	GtkTreeIter iter;

	gchar* szTmp = NULL;

	// General
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_channelonstartup");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.channelonstartup);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_enablenotifications");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.enable_notifications);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_ignorediacritics");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.ignore_diacritics);

	// LibVLC
	switch(app->prefs.libvlcconfigfile_mode){
		case 0:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radioconfigfilenone");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case 1:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radioconfigfilecustom");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);				
			break;
		case 2:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radioconfigfilevlc");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);	
		default:
			break;
	}

	// Recordings
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_directoryrecordings");
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (widget),
	                                     app->prefs.directoryrecordings);

	switch(app->prefs.transcoding_mode){
		case 0:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radiotranscodingno");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case 1:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radiotranscodingpredefinedformats");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);				
			break;
		default:
			break;
	}

	model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_transcodeformat");
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_transcodingformats");
	gtk_tree_model_get_iter_from_string (model, &iter, app->prefs.transcoding_format);
	gtk_combo_box_set_active_iter (GTK_COMBO_BOX(widget), &iter);

	// Network
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_entrytimeout");
	szTmp = g_strdup_printf("%d", app->prefs.timeout);
	gtk_entry_set_text (GTK_ENTRY(widget), szTmp);
	g_free(szTmp);
	szTmp = NULL;

	switch(app->prefs.proxy.proxy_mode){
		case 0:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radioproxyno");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case 1:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radioproxymanual");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);				
			break;
		default:
			break;
	}

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_entryproxyserver");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_server);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_entryproxyport");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_port);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_entryproxyport");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_port);
	
	switch(app->prefs.proxy.proxy_mode){
		case G_PROXY_MODE_NONE:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radioproxyno");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case G_PROXY_MODE_MANUAL:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
			                                               "dialogpreferences_radioproxymanual");		
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);				
			break;
		default:
			break;
	}

	model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_proxytype");
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_comboboxproxytype");
	gtk_tree_model_get_iter_from_string (model, &iter, app->prefs.proxy.proxy_type);
	gtk_combo_box_set_active_iter (GTK_COMBO_BOX(widget), &iter);
	
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_checkuseauth");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.proxy.proxy_use_auth);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_entryproxyusername");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_username);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_entryproxypassword");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_password);
	
	
	dialogpreferences_update_view (app);

}

static void
on_channels_group_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Received signal for channels group added '%s'\n", pChannelsGroupInfos->name);

	FreetuxTVApp *app;
	app = freetuxtv_window_add_channels_group_get_app(pWindowAddChannelsGroup);

	GtkTreePath** ppTreePath = (GtkTreePath**)user_data;

	if(ppTreePath){
		gtk_tree_path_free (*ppTreePath);
		*ppTreePath = NULL;
	}
		
	channels_list_ui_add_channels_group (app, pChannelsGroupInfos, ppTreePath);
}

static void
on_channels_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Received signal for channels added in group '%s'\n", pChannelsGroupInfos->name);

	FreetuxTVApp *app;
	app = freetuxtv_window_add_channels_group_get_app(pWindowAddChannelsGroup);

	GtkTreePath** ppTreePath = (GtkTreePath**)user_data;

	if(ppTreePath){
		channels_list_reload_channels_of_channels_group (app, dbsync, *ppTreePath, error);
	}
}

static void
on_window_add_channels_group_destroy (GtkBuilderWidget *self, gpointer user_data)
{
	GtkTreePath** ppCurrentTreePath = (GtkTreePath**)user_data;

	if(ppCurrentTreePath){
		if(*ppCurrentTreePath){
			gtk_tree_path_free (*ppCurrentTreePath);
			*ppCurrentTreePath = NULL;
		}
		
		g_free(ppCurrentTreePath);
		ppCurrentTreePath = NULL;
	}
}

static void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
                                          gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;

	GtkWindow* pWindow;
	GtkWidget* pParent;
	pParent = (GtkWidget *) gtk_builder_get_object (app->gui, "windowmain");
	
	pWindowAddChannelsGroups = freetuxtv_window_add_channels_group_new (
	    GTK_WINDOW(pParent), app, &error);

	if(error == NULL){
		pWindow = gtk_builder_window_get_top_window (GTK_BUILDER_WINDOW(pWindowAddChannelsGroups));
		gtk_widget_show(GTK_WIDGET(pWindow));

		GtkTreePath** ppCurrentTreePath = g_new0 (GtkTreePath*, 1);

		g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "channels-group-added",
			G_CALLBACK(on_channels_group_added), ppCurrentTreePath);
		g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "channels-added",
			G_CALLBACK(on_channels_added), ppCurrentTreePath);
		g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "destroy",
			G_CALLBACK(on_window_add_channels_group_destroy), ppCurrentTreePath);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_menuitemupdatetvchannels_activate (GtkMenuItem *menuitem,
                                                 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;
	
	GtkWindow* pWindow;
	GtkWidget* pParent;
	pParent = (GtkWidget *) gtk_builder_get_object (app->gui, "windowmain");

	FreetuxTVWindowTVChannelsDatabase* pWindowTVChannelsDatabase;
	pWindowTVChannelsDatabase = freetuxtv_window_tv_channels_database_new(GTK_WINDOW(pParent), app);

	if(error == NULL){
		pWindow = gtk_builder_window_get_top_window (GTK_BUILDER_WINDOW(pWindowTVChannelsDatabase));
		gtk_widget_show(GTK_WIDGET(pWindow));
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_menuitemdeinterlace_change (GtkMenuItem *menuitem,
                                          const gchar* mode,
                                          gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;
	DBSync dbsync;
	FreetuxTVChannelInfos* pChannel;

	if(gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(menuitem))){
		freetuxtv_action_deinterlace (app, mode, &error);

		if(app->current.pPathChannel){
			if(error == NULL){
				dbsync_open_db (&dbsync, &error);
			}

			if(error == NULL){
				pChannel = channels_list_get_channel (app, app->current.pPathChannel);
				dbsync_update_channel_deinterlace_mode (&dbsync, pChannel, (gchar*)mode, &error);		
			}

			dbsync_close_db(&dbsync);
		}
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_windowmain_menuitemdeinterlacedisable_activate (GtkMenuItem *menuitem,
                                                   gpointer user_data)
{
	on_windowmain_menuitemdeinterlace_change (menuitem, NULL, user_data);
}

static void
on_windowmain_menuitemdeinterlacebob_activate (GtkMenuItem *menuitem,
                                                    gpointer user_data)
{
	on_windowmain_menuitemdeinterlace_change (menuitem, "bob", user_data);
}

static void
on_windowmain_menuitemdeinterlaceblend_activate (GtkMenuItem *menuitem,
                                                    gpointer user_data)
{
	on_windowmain_menuitemdeinterlace_change (menuitem, "blend", user_data);
}

static void
on_windowmain_menuitemdeinterlacediscard_activate (GtkMenuItem *menuitem,
                                                   gpointer user_data)
{
	on_windowmain_menuitemdeinterlace_change (menuitem, "discard", user_data);
}

static void
on_windowmain_menuitemdeinterlacelinear_activate (GtkMenuItem *menuitem,
                                                    gpointer user_data)
{
	on_windowmain_menuitemdeinterlace_change (menuitem, "linear", user_data);
}

static void
on_windowmain_menuitemdeinterlacemean_activate (GtkMenuItem *menuitem,
                                                gpointer user_data)
{
	on_windowmain_menuitemdeinterlace_change (menuitem, "mean", user_data);
}

static void
on_windowmain_menuitemdeinterlacex_activate (GtkMenuItem *menuitem,
                                                  gpointer user_data)
{
	on_windowmain_menuitemdeinterlace_change (menuitem, "x", user_data);
}

static gboolean
on_windowmain_valuechanged (GtkRange *range, GtkScrollType scroll,
                            gdouble value, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	//g_print("change %f\n", value); // TODO
	if(!gtk_libvlc_media_player_is_playing(app->player, &error)){
		gtk_libvlc_media_player_play(app->player, NULL, &error);
	}
	if(value >= 0.0 && value <= 1.0){
		gtk_libvlc_media_player_set_position(app->player, value, &error);
	}

	if(error != NULL){
		g_error_free (error);
		error = NULL;
	}
	return FALSE;
}

static void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
                                            gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	gtk_window_get_size (GTK_WINDOW(widget),
	                     &app->config.windowminimode_width,
	                     &app->config.windowminimode_height);

	gtk_widget_hide (widget);

	// Display the normal mode window
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain");
	gtk_widget_show (widget);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);
}

static void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
                                           gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GtkWidget *widget;
	gboolean bActive;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	    "windowminimode_buttonstayontop");
	bActive = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget));
	
	if(bActive){
		app->config.windowminimode_stayontop = TRUE;	
	}else{
		app->config.windowminimode_stayontop = FALSE;
	}

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	gtk_window_set_keep_above (GTK_WINDOW(widget),
	                           app->config.windowminimode_stayontop);
}

static void
on_dialogpreferences_directoryrecordings_changed (GtkFileChooser *chooser,
                                                  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);	
}

static void
on_dialogpreferences_radiotranscoding_toggled (GtkToggleButton *togglebutton,
                                               gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_transcodingformats_changed (GtkComboBox *widget,
                                                 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);	
}

static void
on_dialogpreferences_radioproxy_toggled (GtkToggleButton *togglebutton,
                                         gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_checkuseauth_toggled (GtkToggleButton *togglebutton,
                                     	   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_response (GtkDialog *dialog,
                               gint response_id,
                               gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	GtkWidget* widget;
	gboolean bIsOk = TRUE;
	const char* text;

	GtkTreeModel *model;
	GtkTreeIter iter;

	
	if(response_id == GTK_RESPONSE_APPLY){
		// Get prefs general
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_channelonstartup");
		app->prefs.channelonstartup = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_enablenotifications");
		app->prefs.enable_notifications = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_ignorediacritics");
		app->prefs.ignore_diacritics = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		// Get prefs libvlc
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioconfigfilenone");	
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.libvlcconfigfile_mode = 0;	
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioconfigfilecustom");		
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.libvlcconfigfile_mode = 1;	
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioconfigfilevlc");		
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.libvlcconfigfile_mode = 2;
		}
		

		// Get prefs recording
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_directoryrecordings");
		g_free(app->prefs.directoryrecordings);
		app->prefs.directoryrecordings = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radiotranscodingno");	
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.transcoding_mode = 0;	
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radiotranscodingpredefinedformats");		
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.transcoding_mode = 1;	
		}

		model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_transcodeformat");
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_transcodingformats");
		gtk_combo_box_get_active_iter (GTK_COMBO_BOX(widget), &iter);
		app->prefs.transcoding_format = gtk_tree_model_get_string_from_iter (model, &iter);

		// Get prefs network
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entrytimeout");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		app->prefs.timeout = atoi(text);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioproxyno");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.proxy.proxy_mode = G_PROXY_MODE_NONE;	
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioproxymanual");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.proxy.proxy_mode = G_PROXY_MODE_MANUAL;
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyserver");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_server){
			g_free(app->prefs.proxy.proxy_server);
			app->prefs.proxy.proxy_server = NULL;
		}
		app->prefs.proxy.proxy_server = g_strdup(text);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyport");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_port){
			g_free(app->prefs.proxy.proxy_port);
			app->prefs.proxy.proxy_port = NULL;
		}
		app->prefs.proxy.proxy_port = g_strdup(text);

		model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_proxytype");
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_comboboxproxytype");
		gtk_combo_box_get_active_iter (GTK_COMBO_BOX(widget), &iter);
		app->prefs.proxy.proxy_type = gtk_tree_model_get_string_from_iter (model, &iter);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_checkuseauth");
		app->prefs.proxy.proxy_use_auth = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyusername");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_username){
			g_free(app->prefs.proxy.proxy_username);
			app->prefs.proxy.proxy_username = NULL;
		}
		app->prefs.proxy.proxy_username = g_strdup(text);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxypassword");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_password){
			g_free(app->prefs.proxy.proxy_password);
			app->prefs.proxy.proxy_password = NULL;
		}
		app->prefs.proxy.proxy_password = g_strdup(text);
	}

	if(bIsOk){
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
on_dialoggroupproperties_response (GtkDialog *dialog,
                                   gint response_id,
                                   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gchar *errmsg = NULL;

	FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;

	if(response_id == GTK_RESPONSE_APPLY){
		GtkWidget *groupname;
		GtkWidget *groupuri;
		GtkWidget *bregex;
		GtkWidget *eregex;

		const gchar *sgroupname;
		const gchar *sgroupuri;
		const gchar *sbregex;
		const gchar *seregex;

		groupname = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                                  "dialoggroupproperties_name");
		groupuri = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                                 "dialoggroupproperties_uri");
		bregex = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialoggroupproperties_bregex");
		eregex = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialoggroupproperties_eregex");

		sgroupname = gtk_entry_get_text(GTK_ENTRY(groupname));
		sgroupuri = gtk_entry_get_text(GTK_ENTRY(groupuri));
		sbregex = gtk_entry_get_text(GTK_ENTRY(bregex));
		seregex = gtk_entry_get_text(GTK_ENTRY(eregex));

		pChannelsGroupInfos = (FreetuxTVChannelsGroupInfos *)g_object_get_data (G_OBJECT(dialog), "ChannelsGroup");

		/* Check the field */
		if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
			errmsg = g_strdup_printf(_("Please enter the group's name !"));
		}

		if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST){
			if(g_ascii_strcasecmp(sgroupuri,"") == 0 && errmsg == NULL){
				errmsg = g_strdup_printf(_("Please enter the group's URI !"));
			}
		}

		if(errmsg != NULL){
			windowmain_show_error (app, errmsg);
		}else{

			if(g_ascii_strcasecmp(sbregex,"") == 0 ){
				bregex = NULL;
			}
			if(g_ascii_strcasecmp(seregex,"") == 0 ){
				seregex = NULL;
			}
			
			GtkTreePath *path_group;
			path_group = (GtkTreePath *)g_object_get_data (G_OBJECT(dialog), "PathChannelsGroup");

			freetuxtv_channels_group_infos_set_name (pChannelsGroupInfos, (gchar*)sgroupname);

			if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST){
				freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos, (gchar*)sgroupuri);
				freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos, (gchar*)sbregex, (gchar*)seregex);
			}

			channels_list_update_channels_group (app, path_group, pChannelsGroupInfos);

			g_object_set_data (G_OBJECT(dialog), "ChannelsGroup", NULL);
			g_object_set_data (G_OBJECT(dialog), "PathChannelsGroup", NULL);

			gtk_widget_hide(GTK_WIDGET(dialog));	
		}

		g_free(errmsg);
	}
	if (response_id == GTK_RESPONSE_CANCEL){
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
on_aboutdialog_response (GtkDialog *dialog,
                         gint response_id,
                         gpointer user_data)
{
	gtk_widget_hide(GTK_WIDGET(dialog));
}


static void
dialogpreferences_update_view(FreetuxTVApp *app)
{
	GtkWidget* widget;
	gboolean bIsManualProxy = FALSE;
	gboolean bUseAuth = FALSE;

	// Refresh recording string
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                   "textbuffer_previewtranscoding");
	gchar *text;
	text = get_recording_options(app, "%filename%", TRUE, NULL);
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(widget), text, -1);
	g_free(text);

	// Refresh the network view
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_radioproxymanual");
	if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
		bIsManualProxy = TRUE;
	}else{
		bIsManualProxy = FALSE;
	}
		
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_checkuseauth");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))){
		bUseAuth = TRUE;
	}else{
		bUseAuth = FALSE;
	}

	if(bIsManualProxy){
		// Disable all widget
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyserver");
		gtk_widget_set_sensitive (widget, TRUE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyport");
		gtk_widget_set_sensitive (widget, TRUE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_comboboxproxytype");
		gtk_widget_set_sensitive (widget, TRUE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_checkuseauth");
		gtk_widget_set_sensitive (widget, TRUE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyusername");
		gtk_widget_set_sensitive (widget, bUseAuth);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxypassword");
		gtk_widget_set_sensitive (widget, bUseAuth);

	}else{
		// Disable all widget
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyserver");
		gtk_widget_set_sensitive (widget, FALSE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyport");
		gtk_widget_set_sensitive (widget, FALSE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_comboboxproxytype");
		gtk_widget_set_sensitive (widget, FALSE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_checkuseauth");
		gtk_widget_set_sensitive (widget, FALSE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyusername");
		gtk_widget_set_sensitive (widget, FALSE);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxypassword");
		gtk_widget_set_sensitive (widget, FALSE);
	}
}

static gboolean
on_accel_playpause (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                     GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_playpause (app, &error);
	
	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_tooglefullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                           GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_toggle_fullscreen (app->player, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_unfullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                       GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_set_fullscreen (app->player, FALSE, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_volumeup (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                   GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;
	GtkWidget* widget;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_volumecontrol");

	app->config.volume = app->config.volume + 0.10;
	if(app->config.volume >= 1.0){
		app->config.volume = 1.0;
	}
	gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume, &error);


	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_volumedown (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                     GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;
	GtkWidget* widget;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_volumecontrol");

	app->config.volume = app->config.volume - 0.10; 
	if(app->config.volume <= 0.0){
		app->config.volume = 0.0;
	}
	gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume, &error);
	

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}
