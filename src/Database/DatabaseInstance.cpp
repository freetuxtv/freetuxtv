//
// Created by ebeuque on 20/01/2021.
//

#include <QSqlError>

#include "Global/UserPaths.h"

#include "DatabaseInstance.h"

DatabaseInstance::DatabaseInstance(const QString& szName)
{
	m_szConnectionName = "qtconntection_" + szName;
	m_db = QSqlDatabase::addDatabase("QSQLITE", m_szConnectionName);
	QString szDatabaseName = UserPaths::getUserDatabasePath();
	m_db.setDatabaseName(UserPaths::getUserDatabasePath());
	qCritical("[Database] Using database name %s", qPrintable(szDatabaseName));
}

DatabaseInstance::~DatabaseInstance()
{

}

QSqlDatabase& DatabaseInstance::db()
{
	return m_db;
}

bool DatabaseInstance::open()
{
	bool bRes;
	bRes = m_db.open();
	if(!bRes){
		QSqlError sqlError = m_db.lastError();
		qCritical("[Database] Unable to open database '%s': %s", qPrintable(m_szConnectionName), qPrintable(sqlError.text()));
	}
	return bRes;
}

void DatabaseInstance::close()
{
	m_db.close();
}