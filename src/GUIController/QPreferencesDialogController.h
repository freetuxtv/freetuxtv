//
// Created by ebeuque on 06/10/2020.
//

#ifndef FREETUXTV_QPREFERENCESDIALOGCONTROLLER_H
#define FREETUXTV_QPREFERENCESDIALOGCONTROLLER_H

#include <QObject>

class QPreferencesDialog;

class Preferences;

class QPreferencesDialogController : public QObject
{
	Q_OBJECT
public:
	QPreferencesDialogController();
	virtual ~QPreferencesDialogController();

public:
	void init(QPreferencesDialog* pPreferencesDialog, Preferences* pPreferences);
	void dispose();

private slots:
	void onValidButtonClicked();

private:
	void loadPreferences(Preferences* pPreferences);

private:
	QPreferencesDialog* m_pPreferencesDialog;

	Preferences* m_pPreferences;
};


#endif //FREETUXTV_QPREFERENCESDIALOGCONTROLLER_H
