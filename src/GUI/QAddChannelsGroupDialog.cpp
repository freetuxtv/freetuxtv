/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTreeView>
#include <QFormLayout>
#include <QRadioButton>

#include "QAddChannelsGroupDialog.h"

QAddChannelsGroupDialog::QAddChannelsGroupDialog(QWidget* pParent)
{
	m_iAllowedType = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_ALL;

	resize(800, 600);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	QBoxLayout* pTmpLayout;
	QWidget* pTmpWidget;

	{
		m_pTabWidget = new QTabWidget(pParent);
		pMainLayout->addWidget(m_pTabWidget);

		pTmpWidget = createTabExistingChannelsGroup(m_pTabWidget);
		m_pTabWidget->addTab(pTmpWidget, tr("Existing channels group"));

		pTmpWidget = createTabCustomChannelsGroup(m_pTabWidget);
		m_pTabWidget->addTab(pTmpWidget, tr("Custom channels group"));

		pTmpWidget = createTabSpecialGroup(m_pTabWidget);
		m_pTabWidget->addTab(pTmpWidget, tr("Special group"));
	}

	{
		pTmpLayout = new QHBoxLayout();
		pMainLayout->addLayout(pTmpLayout);

		pTmpLayout->addStretch();

		m_pButtonCancel = new QPushButton(tr("Cancel"));
		pTmpLayout->addWidget(m_pButtonCancel);

		m_pButtonValid = new QPushButton(tr("Add"));
		pTmpLayout->addWidget(m_pButtonValid);
	}
}

QAddChannelsGroupDialog::~QAddChannelsGroupDialog()
{

}

QWidget* QAddChannelsGroupDialog::createTabExistingChannelsGroup(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QBoxLayout* pTmpLayout;

	QWidget* pTmpWidget;

	// Channels groups file
	{
		pTmpLayout = new QHBoxLayout();
		pMainLayout->addLayout(pTmpLayout);

		pTmpLayout->addWidget(new QLabel(tr("Channels groups file:")));

		m_pLineEditChannelsGroupFile = new QLineEdit();
		m_pLineEditChannelsGroupFile->setText("https://raw.githubusercontent.com/freetuxtv/freetuxtv/master/data/channels_groups.xml");
		pTmpLayout->addWidget(m_pLineEditChannelsGroupFile);

		m_pButtonChannelsGroupListRefresh = new QPushButton(tr("Refresh"));
		pTmpLayout->addWidget(m_pButtonChannelsGroupListRefresh);
	}

	// Info text
	{
		pTmpWidget = new QLabel(tr("Select one or several channels group you want to add and click on the add button."));
		pMainLayout->addWidget(pTmpWidget);
	}

	// Notifications
	{
		m_pTreeViewChannelsGroup = new QTreeView();
		m_pTreeViewChannelsGroup->setSelectionMode(QTreeView::MultiSelection);
		pMainLayout->addWidget(m_pTreeViewChannelsGroup);
	}

	return pMainWidget;
}


QWidget* QAddChannelsGroupDialog::createTabCustomChannelsGroup(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox* pGroupBox;
	QBoxLayout* pTmpLayout;
	QFormLayout* pTmpFormLayout;

	QWidget* pTmpWidget;
	QLabel* pTmpLabel;

	// Channels group infos
	{
		pTmpFormLayout = new QFormLayout();
		pMainLayout->addLayout(pTmpFormLayout);

		m_pLineEditGroupName = new QLineEdit();
		m_pLineEditGroupName->setToolTip(tr("Name of the group which will be added"));
		pTmpFormLayout->addRow(tr("Group's name:"), m_pLineEditGroupName);

		m_pLineEditPlaylistURI = new QLineEdit();
		m_pLineEditPlaylistURI->setToolTip(tr("Playlist's URI of the group which will be added"));
		pTmpFormLayout->addRow(tr("Playlist's URI:"), m_pLineEditPlaylistURI);
	}

	// Process
	{
		pGroupBox = new QGroupBox(tr("Processing of channel's name"));
		pMainLayout->addWidget(pGroupBox);

		pTmpFormLayout = new QFormLayout();
		pGroupBox->setLayout(pTmpFormLayout);

		m_pLineEditDeleteBefore = new QLineEdit();
		m_pLineEditDeleteBefore->setToolTip(tr("Text or regexp which must be deleted at the end of the channel's name"));
		pTmpFormLayout->addRow(tr("Delete this text before the name:"), m_pLineEditDeleteBefore);

		m_pLineEditDeleteBehind = new QLineEdit();
		m_pLineEditDeleteBehind->setToolTip(tr("Text or regexp which must be deleted at the beginning of the channel's name"));
		pTmpFormLayout->addRow(tr("Delete this text behind the name:"), m_pLineEditDeleteBehind);

		pTmpLabel = new QLabel("You can easily use regular expressions to delete some text in the channel's name of the group. For example, if in your playlist, you have a name like \"11 - NT1\" or \"12 - NRJ 12\", FreetuxTV is able to remove the channel's number before the name, by using the regular expression \"[0-9]* - \".");
		pTmpLabel->setWordWrap(true);
		pTmpFormLayout->addRow(pTmpLabel);
	}

	pMainLayout->addStretch();

	return pMainWidget;
}


QWidget* QAddChannelsGroupDialog::createTabSpecialGroup(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox* pGroupBox;
	QBoxLayout* pTmpLayout;
	QFormLayout* pTmpFormLayout;

	QWidget* pTmpWidget;
	QLabel* pTmpLabel;

	// Channels group infos
	{
		pTmpFormLayout = new QFormLayout();
		pMainLayout->addLayout(pTmpFormLayout);

		m_pLineEditSpecialGroupName = new QLineEdit();
		m_pLineEditSpecialGroupName->setToolTip(tr("Name of the group which will be added"));
		pTmpFormLayout->addRow(tr("Group's name:"), m_pLineEditSpecialGroupName);
	}

	// Process
	{
		pGroupBox = new QGroupBox(tr("Special group type:"));
		pMainLayout->addWidget(pGroupBox);

		pTmpLayout = new QVBoxLayout();
		pGroupBox->setLayout(pTmpLayout);

		m_pRadioButtonFavourites = new QRadioButton(tr("Favourites"));
		pTmpLayout->addWidget( m_pRadioButtonFavourites);
	}

	pMainLayout->addStretch();

	return pMainWidget;
}

QPushButton* QAddChannelsGroupDialog::getButtonCancel() const
{
	return m_pButtonCancel;
}

QPushButton* QAddChannelsGroupDialog::getButtonValid() const
{
	return m_pButtonValid;
}


QLineEdit* QAddChannelsGroupDialog::getLineEditChannelsGroupFile() const
{
	return m_pLineEditChannelsGroupFile;
}

QPushButton* QAddChannelsGroupDialog::getButtonChannelsGroupListRefresh() const
{
	return m_pButtonChannelsGroupListRefresh;
}

QTreeView* QAddChannelsGroupDialog::getTreeViewChannelsGroupList() const
{
	return m_pTreeViewChannelsGroup;
}

void QAddChannelsGroupDialog::setAllowedType(int iAllowedType)
{
	m_iAllowedType= iAllowedType;
/*
	// Show the group allowed
	if((m_iAllowedType & FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_EXISTING)
	    == FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_EXISTING){
		m_pTabWidget->removeTab();
	}else{
		gtk_notebook_set_page_visible(notebook, 0, FALSE);
	}
	if((m_iAllowedType & FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_CUSTOM)
	    == FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_CUSTOM){
		gtk_notebook_set_page_visible(notebook, 1, TRUE);
	}else{
		gtk_notebook_set_page_visible(notebook, 1, FALSE);
	}
	if((m_iAllowedType & FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_FAVOURITES)
	    == FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_FAVOURITES){
		gtk_notebook_set_page_visible(notebook, 2, TRUE);
	}else{
		gtk_notebook_set_page_visible(notebook, 2, FALSE);
	}
 	*/
}
/*

static void
db_on_group_added (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error)
{
	g_return_if_fail(app != NULL);
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(pChannelsGroupInfos != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;
	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	
	g_return_if_fail(pWindowAddChannelsGroup != NULL);
	g_return_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup));

	// Send signal that a group is added
	if(*error == NULL){
		g_signal_emit (
		    G_OBJECT (pWindowAddChannelsGroup),
		    g_signals [SIGNAL_CHANNELS_GROUP_ADDED],
		    0, pChannelsGroupInfos, dbsync, error
		    );
	}
}

static void
db_on_channels_added (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error)
{
	g_return_if_fail(app != NULL);
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(pChannelsGroupInfos != NULL);
	g_return_if_fail(error != NULL);

	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;
	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	
	g_return_if_fail(pWindowAddChannelsGroup != NULL);
	g_return_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup));

	// Send signal that a group is added
	if(*error == NULL){
		g_signal_emit (
		    G_OBJECT (pWindowAddChannelsGroup),
		    g_signals [SIGNAL_CHANNELS_ADDED],
		    0, pChannelsGroupInfos, dbsync, error
		    );
	}
}

FreetuxTVApp*
freetuxtv_window_add_channels_group_get_app(FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup)
{
	g_return_val_if_fail(pWindowAddChannelsGroup != NULL, NULL);
	g_return_val_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup), NULL);
	
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);
	
	return priv->app;
}

static gboolean
do_idle_destroy_window (gpointer user_data)
{
	g_return_val_if_fail(GTK_IS_BUILDER_WIDGET(user_data), FALSE);
	gtk_builder_widget_destroy (GTK_BUILDER_WIDGET(user_data));

	return FALSE;
}

static void
on_buttonclose_clicked (GtkButton *button, gpointer user_data)
{
	// We do the destruction after the button clicked event is finished
	g_idle_add (do_idle_destroy_window, user_data);
}
*/