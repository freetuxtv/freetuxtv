//
// Created by ebeuque on 15/10/2020.
//

#ifndef FREETUXTV_APPLICATION_H
#define FREETUXTV_APPLICATION_H

#include <QString>

class Preferences;

class Application
{
public:
	Application();
	virtual ~Application();

	void setDataDir(const QString& szDataDir);
	const QString& getDataDir() const;

	Preferences* getPreferences();

private:
	QString m_szDataDir;

	Preferences* m_pPreferences;
};


#endif //FREETUXTV_APPLICATION_H
