//
// Created by ebeuque on 08/06/2022.
//

#ifndef FREETUXTV_DATABASE_DATABASEAPPLICATION_H
#define FREETUXTV_DATABASE_DATABASEAPPLICATION_H

#include <QObject>

#include <QDir>

#include <Common/Database/DatabaseEvolution.h>

class DatabaseApplication : public DatabaseEvolutionHandler {
public:
	DatabaseApplication();
	virtual ~DatabaseApplication();

	enum {
		DBMain = 0x01,
		DBAll = DBMain
	};

	void setDatabaseDirectory(const QDir& dirConfigDatabase);

	bool init();
	void dispose();
	void clean();

private:
	QDir m_dirConfigDatabase;

	QString m_szMainDatabaseName;

	int m_iInitFlags;
};


#endif //FREETUXTV_DATABASE_DATABASEAPPLICATION_H
