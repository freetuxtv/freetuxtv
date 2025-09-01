/*
 * DatabaseManager.h
 *
 *  Created on: 27 juil. 2012
 *      Author: ebeuque
 */

#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <QSqlDatabase>
#include <QDir>
#include <QSqlError>
#include <QMutex>
#include <QReadWriteLock>

#include <libqdbevolution/QDbEvolution.h>

#include "DatabaseEvolution.h"
#include "DatabaseConnection.h"


class DatabaseManagerInitParams
{
public:
	DatabaseManagerInitParams();
	virtual ~DatabaseManagerInitParams();

public:
	QString m_szEngine;
	QString m_szConnectionName;

	bool m_bUseReferenceConnection;
	bool m_bCheckFail;
	QString m_szDbEvolutionFilePath;
	bool m_bDoEvolution;

	bool m_bAsynchrounous;

	DatabaseEvolutionHandler* m_pHandler;

	DatabaseConnectionParams m_dbParams;
};

/*
 *
 */
class DatabaseManager
{
public:
	DatabaseManager();
	virtual ~DatabaseManager();

	enum DatabaseStatus {
		DatabaseCorrupted = 0x01,
	};

	bool init(const DatabaseManagerInitParams& params);
	void dispose();

	QString engine() const;
	bool isEngineSQLite() const;
	bool isSupportingTransaction() const;
	bool isAsynchronous() const;

	void updateParams(const DatabaseConnectionParams& params);
	DatabaseConnection* getNewConnection(const QString& szConnectionName);

	bool hasDatabaseStatus(DatabaseStatus iDatabaseStatus) const;
	static void raiseError(DatabaseConnection* pConnection, const QSqlError& error);

	void lockTransaction();
	void unlockTransaction();

	bool check(DatabaseConnection* pConnection, bool bDoEvolution = false, bool bNoConnectionClose = false);

private:
	QReadWriteLock m_lockDB; // Lock database for params update

	int m_iDatabaseStatusFlags;

	QString m_szEngine;
	QString m_szDatabaseName;
	QString m_szDbEvolutionFilePath;

	bool m_bSupportTransactions;
	bool m_bLockTransaction;
	QMutex m_lockTransaction;
	bool m_bAsynchronous;

	DatabaseConnectionParams m_connectionParams;

	DatabaseEvolutionHandler* m_pHandler;
	DatabaseConnection* m_pRefConnection;
};

#endif /* DATABASEMANAGER_H_ */
