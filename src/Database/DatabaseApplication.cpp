//
// Created by ebeuque on 08/06/2022.
//

#include <Common/Database/DatabaseManager.h>
#include <Common/Database/DatabaseConnection.h>

#include "Database/DatabaseInstance.h"

#include "DatabaseApplication.h"

#define DATABASE_NAME_MAIN "freetuxtv.db"

DatabaseApplication::DatabaseApplication()
{
	m_iInitFlags = 0;
}

DatabaseApplication::~DatabaseApplication()
{

}

void DatabaseApplication::setDatabaseDirectory(const QDir& dirConfigDatabase)
{
	m_dirConfigDatabase = dirConfigDatabase;
	m_szMainDatabaseName = DatabaseConnectionParams::databaseNameFromFile(m_dirConfigDatabase, DATABASE_NAME_MAIN);
}

bool DatabaseApplication::init()
{
	bool bRes = true;
	bool bInitOK;

	// Initializing main database
	{
		qDebug("[Database] Initializing main database");
		DatabaseManagerInitParams dbmInitParams;
		dbmInitParams.m_szEngine = "QSQLITE";
		dbmInitParams.m_dbParams.m_szDatabaseName = m_szMainDatabaseName;
		dbmInitParams.m_szDbEvolutionFilePath = ":/sqlite3-create-tables.sql";
		dbmInitParams.m_szConnectionName = "qt_main_connection";
		dbmInitParams.m_pHandler = this;
#ifdef UNITTEST
		dbmInitParams.m_bAsynchrounous = true;
#endif
		bInitOK = databaseManagerLocal()->init(dbmInitParams);
		if(bInitOK){
			m_iInitFlags |= DBMain;
		}else{
			bRes = false;
			qCritical("[Database] Cannot initialize main database");
		}
	}

	return bRes;
}

void DatabaseApplication::dispose()
{
	int iCleanFlags = 0;

	// Dispose/clean main database
	if(m_iInitFlags & DBMain){
		qDebug("[Database] Disposing main database");
		//databaseManagerLocal()->dispose();
		m_iInitFlags &= ~DBMain;
	}
}