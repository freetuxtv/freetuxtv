/*
 * DatabaseConnection.h
 *
 *  Created on: 22 juil. 2014
 *      Author: ebeuque
 */

#ifndef COMMON_DATABASE_DATABASECONNECTION_H_
#define COMMON_DATABASE_DATABASECONNECTION_H_

#include <QString>
#include <QDir>
#include <QMutex>

#ifdef QT_DEBUG
#define DATABASE_CONNECTION_TIMER
#endif

#ifdef DATABASE_CONNECTION_TIMER
#include <QElapsedTimer>
#endif

class DatabaseManager;

class QSqlDatabase;
class QSqlDriver;

class DatabaseConnectionParams
{
public:
	DatabaseConnectionParams();
	virtual ~DatabaseConnectionParams();

	void setDatabaseNameFromFile(const QDir& appUserDir, const QString& szFileName);
	static QString databaseNameFromFile(const QDir& appUserDir, const QString& szFileName);

public:
	QString m_szDatabaseHostName;
	int m_iDatabaseHostPort;
	QString m_szDatabaseUserName;
	QString m_szDatabasePassword;
	QString m_szDatabaseName;

	bool m_bUseCache;
};

class DatabaseConnection
{
public:
	DatabaseConnection(DatabaseManager* pDBM, const QString& szConnectionName);
	virtual ~DatabaseConnection();

	bool isValid() const;

	QSqlDatabase* db() const;
	QSqlDatabase& query_db() const;
	QSqlDriver* driver() const;
	QString driverName() const;
	QString connectionName() const;

	DatabaseManager* getDatabaseManager() const;
	bool isSQLite() const;
	bool isMySQL() const;

	void updateConnectionParams(const DatabaseConnectionParams& params);

	bool open(const char* szLog = NULL);
	void close(const char* szLog = NULL);

	bool beginTransaction();
	bool endTransaction(bool bCommit);

private:
	DatabaseManager* m_pDBM;
	QSqlDatabase* m_db;

	QString m_szConnectionName;

#ifdef DATABASE_CONNECTION_TIMER
	QElapsedTimer m_timer;
#endif

	QMutex m_mutex; // Mutex for open counter
	int m_iOpenCounter;
};

#endif /* COMMON_DATABASE_DATABASECONNECTION_H_ */
