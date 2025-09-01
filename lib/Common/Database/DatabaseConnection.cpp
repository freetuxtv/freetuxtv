/*
 * DatabaseConnection.cpp
 *
 *  Created on: 22 juil. 2014
 *      Author: ebeuque
 */

#include <QSqlDatabase>
#include <QSqlQuery>

#include <inttypes.h>

#include "DatabaseConnection.h"

#include "DatabaseManager.h"

DatabaseConnectionParams::DatabaseConnectionParams()
{
	m_iDatabaseHostPort = 0;
	m_bUseCache = false;
}

DatabaseConnectionParams::~DatabaseConnectionParams()
{

}

void DatabaseConnectionParams::setDatabaseNameFromFile(const QDir& appUserDir, const QString& szFileName)
{
	m_szDatabaseName = databaseNameFromFile(appUserDir, szFileName);
}

QString DatabaseConnectionParams::databaseNameFromFile(const QDir& appUserDir, const QString& szFileName)
{
	QString path = appUserDir.filePath(szFileName);
	return QDir::toNativeSeparators(path);
}

DatabaseConnection::DatabaseConnection(DatabaseManager* pDBM, const QString& szConnectionName)
{
	m_pDBM = pDBM;
	// QSqlDatabase has memory leak: https://bugreports.qt-project.org/browse/QTBUG-27117
	if(szConnectionName.isEmpty()){
		m_db = new QSqlDatabase(QSqlDatabase::addDatabase(m_pDBM->engine()));
	}else{
		m_db = new QSqlDatabase(QSqlDatabase::addDatabase(m_pDBM->engine(), szConnectionName));
	}

	m_szConnectionName = m_db->connectionName();
	m_iOpenCounter = 0;
}

DatabaseConnection::~DatabaseConnection()
{
	if(m_db){
		delete m_db;
		m_db = NULL;
		QSqlDatabase::removeDatabase(m_szConnectionName);
	}
}

bool DatabaseConnection::isValid() const
{
	return m_db && m_db->isValid();
}


QSqlDatabase* DatabaseConnection::db() const
{
	return m_db;
}

QSqlDatabase& DatabaseConnection::query_db() const
{
	return *m_db;
}

QSqlDriver* DatabaseConnection::driver() const
{
	if(m_db){
		return m_db->driver();
	}
	return NULL;
}

QString DatabaseConnection::driverName() const
{
	if(m_db){
		return m_db->driverName();
	}
	return QString();
}

QString DatabaseConnection::connectionName() const
{
	return m_szConnectionName;
}


DatabaseManager* DatabaseConnection::getDatabaseManager() const
{
	return m_pDBM;
}

bool DatabaseConnection::isSQLite() const
{
	if(m_db){
		return (m_db->driverName() == "QSQLITE");
	}
	return false;
}

bool DatabaseConnection::isMySQL() const
{
	if(m_db){
		return (m_db->driverName() == "QMYSQL");
	}
	return false;
}

void DatabaseConnection::updateConnectionParams(const DatabaseConnectionParams& params)
{
	// Set up params
	m_db->setHostName(params.m_szDatabaseHostName);
	m_db->setPort(params.m_iDatabaseHostPort);
	m_db->setUserName(params.m_szDatabaseUserName);
	m_db->setPassword(params.m_szDatabasePassword);
	m_db->setDatabaseName(params.m_szDatabaseName);

	if(isSQLite()){
		if(params.m_bUseCache){
			m_db->setConnectOptions("QSQLITE_OPEN_URI;QSQLITE_ENABLE_SHARED_CACHE");
		}else{
			m_db->setConnectOptions("QSQLITE_OPEN_URI");
		}
	}
}

bool DatabaseConnection::open(const char* szLog)
{
	bool bRes = false;

	m_mutex.lock();

	if(m_iOpenCounter == 0){
		if(szLog){
			qDebug("[Database] Opening connection %s (from %s)", qPrintable(m_szConnectionName), szLog);
		}else{
			qDebug("[Database] Opening connection %s", qPrintable(m_szConnectionName));
		}

#ifdef DATABASE_CONNECTION_TIMER
		m_timer.start();
#endif
		bRes = m_db->open();
		if(!bRes){
			if(szLog){
				qCritical("[Database] Unable to open connection %s (from %s)", qPrintable(m_szConnectionName), szLog);
			}else{
				qCritical("[Database] Unable to open connection %s", qPrintable(m_szConnectionName));
			}
			DatabaseManager::raiseError(this, m_db->lastError());
		}

		if(bRes && m_pDBM->isAsynchronous() && isSQLite()){
			QSqlQuery query(*m_db);
			bRes = query.exec("PRAGMA synchronous = OFF");
			if(!bRes){
				DatabaseManager::raiseError(this, query.lastError());
			}
		}

		if(bRes){
			m_iOpenCounter = 1;
		}
	}else{
		m_iOpenCounter++;
		qDebug("[Database] Using connection already open %s (counter: %d)", qPrintable(m_szConnectionName), m_iOpenCounter);
		bRes = true;
	}

	m_mutex.unlock();
	return bRes;
}

void DatabaseConnection::close(const char* szLog)
{
	m_mutex.lock();
	if(m_iOpenCounter == 1){
		m_db->close();
#ifdef DATABASE_CONNECTION_TIMER
		qint64 timeElapsed = m_timer.elapsed();
#endif
#ifdef DATABASE_CONNECTION_TIMER
		if(szLog){
			// PRIiMAX not supported by qDebug
			qDebug("[Database] Closed connection %s (%" PRIiMAX " ms) (from %s)", qPrintable(m_szConnectionName), (intmax_t)timeElapsed, szLog);
		}else{
			qDebug("[Database] Closed connection %s (%" PRIiMAX " ms)", qPrintable(m_szConnectionName), (intmax_t)timeElapsed);
		}
#else
		if(szLog){
			qDebug("[Database] Closed connection %s (from %s)", qPrintable(m_szConnectionName), szLog);
		}else{
			qDebug("[Database] Closed connection %s", qPrintable(m_szConnectionName));
		}
#endif
		m_iOpenCounter = 0;
	}else{
		m_iOpenCounter--;
		qDebug("[Database] Keeping connection open %s (counter: %d)", qPrintable(m_szConnectionName), m_iOpenCounter);
	}
	m_mutex.unlock();
}


bool DatabaseConnection::beginTransaction()
{
	bool bRes = false;
	m_pDBM->lockTransaction();
	if(m_pDBM->isSupportingTransaction()){
		bRes = m_db->transaction();
		if(!bRes){
			qCritical("[Database] Cannot create transaction for connection %s", qPrintable(m_szConnectionName));
			DatabaseManager::raiseError(this, m_db->lastError());
		}else{
			if(isMySQL()){
				QSqlQuery query(*m_db);
				bRes = query.exec("SET autocommit=0;");
				if(!bRes){
					DatabaseManager::raiseError(this, query.lastError());
				}
			}
		}
	}else{
		bRes = true;
	}
	if(!bRes){
		m_pDBM->unlockTransaction();
	}
	return bRes;
}

bool DatabaseConnection::endTransaction(bool bCommit)
{
	bool bRes = false;
	if(m_pDBM->isSupportingTransaction()){
		if(bCommit){
			// TMP_LOG
			qDebug("[Database] Commit transaction");
			bRes = m_db->commit();
			if(!bRes){
				qCritical("[Database] Cannot commit transaction for connection %s", qPrintable(m_szConnectionName));
				DatabaseManager::raiseError(this, m_db->lastError());
			}
		}else{
			// TMP_LOG
			qDebug("[Database] Rollback transaction");
			bRes = m_db->rollback();
			if(!bRes){
				qCritical("[Database] Cannot rollback transaction for connection %s", qPrintable(m_szConnectionName));
				DatabaseManager::raiseError(this, m_db->lastError());
			}
		}
	}else{
		bRes = true;
	}
	m_pDBM->unlockTransaction();
	return bRes;
}
