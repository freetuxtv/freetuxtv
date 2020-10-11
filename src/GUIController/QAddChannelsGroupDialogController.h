//
// Created by ebeuque on 10/10/2020.
//

#ifndef FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
#define FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H

#include <QObject>
#include <QStandardItemModel>

#include "Model/Preferences.h"

class QError;

class QAddChannelsGroupDialog;

class QAddChannelsGroupDialogController : public QObject
{
	Q_OBJECT
public:
	void init(QAddChannelsGroupDialog* pPreferencesDialog, Preferences* pPreferences);
	void dispose();

private slots:
	void onValidButtonClicked();
	void onChannelsGroupListRefreshButtonClicked();

signals:
	void channelsGroupAdded();
	void channelsAdded();

private:
	bool loadChannelsGroupListFromFile(QStandardItemModel* pModel, QError *pError);

private:
	QAddChannelsGroupDialog* m_pAddChannelsGroupDialog;
	Preferences* m_pPreferences;

	QStandardItemModel* m_pChannelsGroupListModel;
};


#endif //FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
