//
// Created by ebeuque on 06/10/2020.
//

#ifndef FREETUXTV_QPREFERENCESDIALOGCONTROLLER_H
#define FREETUXTV_QPREFERENCESDIALOGCONTROLLER_H

#include <QObject>

class QPreferencesDialog;

class QPreferencesDialogController : public QObject
{
public:
	QPreferencesDialogController();
	virtual ~QPreferencesDialogController();

public:
	void init(QPreferencesDialog* pPreferencesDialog);
	void dispose();

private:
	QPreferencesDialog* m_pPreferencesDialog;
};


#endif //FREETUXTV_QPREFERENCESDIALOGCONTROLLER_H
