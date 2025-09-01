/*
 * DatabaseManager.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: ebeuque
 */

#include <QCoreApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QVariant>

#include <Toolkit/QtCompat.h>

#include "DatabaseManager.h"
#include "DatabaseEvolution.h"

#define SQLITE_CORRUPT 11
#define SQLITE_CORRUPT_2 26

DatabaseManagerInitParams::DatabaseManagerInitParams()
{
	m_bUseReferenceConnection = false;
	m_bDoEvolution = true;
	m_bAsynchrounous = false;
	m_bCheckFail = true;
	m_pHandler = NULL;
}

DatabaseManagerInitParams::~DatabaseManagerInitParams()
{

}

DatabaseManager::DatabaseManager()
{
	m_iDatabaseStatusFlags = 0;

	m_bSupportTransactions = false;
	m_bAsynchronous = false;
	m_bLockTransaction = false;

	m_pRefConnection = NULL;
	m_pHandler = NULL;

	qDebug("[Database] List of plugins paths: %s", qPrintable(QCoreApplication::libraryPaths().join(";")));
	qDebug("[Database] List of QtSQL drivers: %s", qPrintable(QSqlDatabase::drivers().join(";")));
}

DatabaseManager::~DatabaseManager()
{

}

bool DatabaseManager::init(const DatabaseManagerInitParams& params)
{
	bool bRes = false;
	
	m_iDatabaseStatusFlags = 0;
	m_pHandler = params.m_pHandler;

	m_szEngine = params.m_szEngine;
	DatabaseConnection* pConnection = new DatabaseConnection(this, params.m_szConnectionName);

	if(params.m_bUseReferenceConnection){
		m_pRefConnection = pConnection;
	}

	if(pConnection){
		if(pConnection->isValid()){

			// update the connection params
			updateParams(params.m_dbParams);
			pConnection->updateConnectionParams(params.m_dbParams);

			qDebug("[Database] Using connection name %s with database name: %s",
					qPrintable(pConnection->connectionName()), qPrintable(params.m_dbParams.m_szDatabaseName));

			m_bAsynchronous = params.m_bAsynchrounous;
			m_szDatabaseName = params.m_dbParams.m_szDatabaseName;
			m_szDbEvolutionFilePath = params.m_szDbEvolutionFilePath;
			bRes = check(pConnection, params.m_bDoEvolution, (m_pRefConnection != NULL));
			if(!bRes && !params.m_bCheckFail){
				bRes = true;
			}
		}

		if(!m_pRefConnection || !bRes){
			m_pRefConnection = NULL;
			delete pConnection;
			pConnection = NULL;
		}
	}

	return bRes;
}

void DatabaseManager::dispose()
{
	if(m_pRefConnection){
		m_pRefConnection->close();

		delete m_pRefConnection;
		m_pRefConnection = NULL;
	}
}

QString DatabaseManager::engine() const
{
	return m_szEngine;
}

bool DatabaseManager::isEngineSQLite() const
{
	return (m_szEngine == "QSQLITE");
}

bool DatabaseManager::isSupportingTransaction() const
{
	return m_bSupportTransactions;
}

bool DatabaseManager::isAsynchronous() const
{
	return m_bAsynchronous;
}

void DatabaseManager::updateParams(const DatabaseConnectionParams& params)
{
	m_lockDB.lockForWrite();
	m_connectionParams = params;
	m_lockDB.unlock();
}

bool DatabaseManager::check(DatabaseConnection* pConnection, bool bDoEvolution, bool bNoConnectionClose)
{
	bool bRes = false;
	if(pConnection->open()){

		// Check if transaction is supported
		m_bSupportTransactions = pConnection->driver()->hasFeature(QSqlDriver::Transactions);
		if(!m_bSupportTransactions){
			qWarning("[Database] Driver cannot use transaction for driver: %s", qPrintable(pConnection->driverName()));
		}
		if(isEngineSQLite()){
			m_bLockTransaction = true;

			QSqlQuery query(*pConnection->db());

			// Check database integrity
			bRes = query.exec("PRAGMA integrity_check");
			if(bRes){
				if(query.next()){
					bRes = (query.value(0).toString() == "ok");
					if(!bRes){
						m_iDatabaseStatusFlags |= DatabaseCorrupted;
						qCritical("[Database] Database is corrupted");
					}
				}else{
					bRes = false;
					qCritical("[Database] Cannot check database integrity");
				}
			}

			// Check sqlite version
			if(bRes){
				bRes = query.exec("SELECT sqlite_version()");
				if(bRes){
					if(query.next()){
						qDebug("[Database] SQLite version: %s", qPrintable(query.value(0).toString()));
					}else{
						qCritical("[Database] No sqlite version");
					}
				}
			}

		}else{
			bRes = true;
		}

		// Do database creation/evolution
		if(bRes){
			if(bDoEvolution && !m_szDbEvolutionFilePath.isEmpty()){
				DatabaseEvolution dbEvolution(m_szDbEvolutionFilePath, *pConnection->db(), pConnection);
				dbEvolution.setHandler(m_pHandler);

				bool bDatabaseExists = false;
				if(isEngineSQLite()){
					QFile file(m_szDatabaseName);
					if(file.exists()){
						bDatabaseExists = dbEvolution.hasVersion();
					}
				}else{
					QString szVersion;
					bDatabaseExists = dbEvolution.hasVersion();
				}

				if(pConnection->beginTransaction()){
					if(bDatabaseExists){
						bRes = dbEvolution.doEvolution();
						if(!bRes){
							qCritical("[Database] Cannot upgrade the database");
						}
					}else{
						bRes = dbEvolution.doCreation();
						if(!bRes){
							qCritical("[Database] Cannot create the database");
						}
					}
					pConnection->endTransaction(bRes);
				}
			}
		}
		if(!bNoConnectionClose){
			pConnection->close();
		}
	}

	return bRes;
}

DatabaseConnection* DatabaseManager::getNewConnection(const QString& szConnectionName)
{
	DatabaseConnection* pConnection = new DatabaseConnection(this, szConnectionName);
	if(pConnection){
		pConnection->updateConnectionParams(m_connectionParams);
		if(pConnection->isValid()){
			return pConnection;
		}
	}
	if(pConnection){
		delete pConnection;
		pConnection = NULL;
	}
	return NULL;
}

bool DatabaseManager::hasDatabaseStatus(DatabaseStatus iDatabaseStatus) const
{
	return (m_iDatabaseStatusFlags & iDatabaseStatus);
}

void DatabaseManager::raiseError(DatabaseConnection* pConnection, const QSqlError& error)
{
	DatabaseManager* pDBM = pConnection->getDatabaseManager();
	if(pDBM->isEngineSQLite()){
#ifdef USE_QSENATIVEERRORCODE
		switch(error.nativeErrorCode().toInt()){
#else
		switch(error.number()){
#endif
		case SQLITE_CORRUPT:
		case SQLITE_CORRUPT_2:
			pDBM->m_iDatabaseStatusFlags |= DatabaseCorrupted;
			break;
		default:
			break;
		}
	}
#ifdef USE_QSENATIVEERRORCODE
	qCritical("[Database] Error %s on %s : %s",
			  qPrintable(error.nativeErrorCode()),
			  qPrintable(pConnection->connectionName()),
			  qPrintable(error.text()));
#else
	qCritical("[Database] Error %d on %s : %s", error.number(), qPrintable(pConnection->connectionName()), qPrintable(error.text()));
#endif
}

void DatabaseManager::lockTransaction()
{
	if(m_bLockTransaction){
		qDebug("[Database] Locking database for transaction %s", qPrintable(m_szDatabaseName));
		m_lockTransaction.lock();
		qDebug("[Database] Locked database for transaction %s", qPrintable(m_szDatabaseName));
	}
}

void DatabaseManager::unlockTransaction()
{
	if(m_bLockTransaction){
		// TMP_LOG
		qDebug("[Database] Try to unlock database for transaction %s", qPrintable(m_szDatabaseName));
		m_lockTransaction.unlock();
		qDebug("[Database] Unlocking database for transaction %s", qPrintable(m_szDatabaseName));
	}
}
