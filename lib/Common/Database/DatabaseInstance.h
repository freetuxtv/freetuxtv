/*
 * DatabaseInstance.h
 *
 *  Created on: 24 mars 2014
 *      Author: ebeuque
 */

#ifndef COMMON_DATABASE_DATABASEINSTANCE_H_
#define COMMON_DATABASE_DATABASEINSTANCE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "DatabaseController.h"

class DatabaseManager;
class DatabaseControllerFactory;

class DatabaseInstanceEntry
{
public:
	DatabaseInstanceEntry();
	virtual ~DatabaseInstanceEntry();

	void dispose();

public:
	DatabaseConnection* m_pConnection;
	DatabaseController* m_pDBC;
};

class DatabaseInstance {
public:
	DatabaseInstance();
	DatabaseInstance(const QString& szId);
	virtual ~DatabaseInstance();

	void dispose();

	void setConnectionAutoLock(bool bAutoLock);

	// Database manager access
	static DatabaseManager* dbm(const QString& szDBName);

	// Database controller access
	DatabaseController& dbc(const QString& szDBName);
	template<class DC>
	DC& dbcCast(const QString& szDBName)
	{
		return (DC&)dbc(szDBName);
	}

	static void setErrorHandler(DatabaseErrorHandler errorHandler);
	static void setDatabaseControllerFactory(DatabaseControllerFactory* pDCFactory);

private:
	static void disposeManagers();
	void disposeControllers();

private:
	QString m_szId;

	bool m_bConnectionAutoLock;

	QHash<QString, DatabaseInstanceEntry*> m_listEntries;

	static QHash<QString, DatabaseManager*> g_listManagers;
	static DatabaseErrorHandler g_errorHandler;
	static DatabaseControllerFactory* g_pDCFactory;

	static QString buildConnectionName(const QString& szName, const QString& szId);
};

#endif /* COMMON_DATABASE_DATABASEINSTANCE_H_ */
