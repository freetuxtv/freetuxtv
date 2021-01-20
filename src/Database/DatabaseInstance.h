//
// Created by ebeuque on 20/01/2021.
//

#ifndef FREETUXTV_DATABASEINSTANCE_H
#define FREETUXTV_DATABASEINSTANCE_H

#include <QSqlDatabase>

class DatabaseInstance
{
public:
	DatabaseInstance(const QString& szName);
	virtual ~DatabaseInstance();

public:
	QSqlDatabase& db();

	bool open();
	void close();

private:
	QSqlDatabase m_db;
	QString m_szConnectionName;
};


#endif //FREETUXTV_DATABASEINSTANCE_H
