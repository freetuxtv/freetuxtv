//
// Created by ebeuque on 15/10/2020.
//

#ifndef FREETUXTV_APPLICATION_H
#define FREETUXTV_APPLICATION_H

#include <QString>

#include <QStandardItemModel>

class Preferences;

class Application
{
public:
	Application();
	virtual ~Application();

	void setDataDir(const QString& szDataDir);
	const QString& getDataDir() const;

	Preferences* getPreferences();

	QStandardItemModel* getChannelListModel();

private:
	QString m_szDataDir;

	Preferences* m_pPreferences;

	QStandardItemModel m_modelChannelsList;
};


#endif //FREETUXTV_APPLICATION_H
