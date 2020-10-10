//
// Created by ebeuque on 10/10/2020.
//

#ifndef FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
#define FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H

#include <QObject>

class QAddChannelsGroupDialog;

class QAddChannelsGroupDialogController : public QObject
{
	Q_OBJECT
public:
	void init(QAddChannelsGroupDialog* pPreferencesDialog);
	void dispose();

private slots:
	void onValidButtonClicked();

private:
	QAddChannelsGroupDialog* m_pAddChannelsGroupDialog;
};


#endif //FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
