//
// Created by ebeuque on 10/08/2022.
//

#ifndef COMMON_DATABASE_DATABASECONTROLLER_H
#define COMMON_DATABASE_DATABASECONTROLLER_H

#include <QString>
#include <QDir>

#include "DatabaseErrorHandler.h"

class QSqlDatabase;
class QSqlError;
class QSqlQuery;

class DatabaseManager;
class DatabaseConnection;

class DatabaseController {
public:
	DatabaseController(DatabaseConnection* dbc, DatabaseErrorHandler errorHandler);
	virtual ~DatabaseController();

public:
	QSqlDatabase& db() const;

	// Open/Close database
	bool open(const char* szLog = NULL);
	void close(const char* szLog = NULL);

	// Error handling
	void raiseError(const QSqlError& error);

	// Transaction
	bool beginTransaction();
	bool endTransaction(bool bCommit);

	// Database features
	bool updateJournalModeWAL();
	bool compactDatabase();

	// Update database
	int getUpdateVersion(const QString& szTable, const QString& szField);
	bool updatePaths(const QString& szTable, const QString& szField, const QDir& dirOld, const QDir& dirNew);

	// Generic utility function
	int rowCount(const QString& szTable, const QString& szWhere = QString());

protected:
	bool isTableExists(const QString& szTable, const QString& szDb = QString());

	// Delete id list by range
	bool deleteIdList(const QString& szTable, const QList<unsigned int>& listId, const QString& szLabel);
	bool deleteIdList(const QString& szTable, const QString& szField, const QList<unsigned int>& listId, const QString& szCondition, const QString& szLabel);

	// Build list
	QString joinListId(const QList<int>& listId);

private:
	DatabaseConnection* m_dbc;
	DatabaseErrorHandler m_errorHandler;
};


#endif // COMMON_DATABASE_DATABASECONTROLLER_H
