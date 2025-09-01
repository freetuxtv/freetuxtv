/*
 * DatabaseInstance.cpp
 *
 *  Created on: 24 mars 2014
 *      Author: ebeuque
 */

#include <Toolkit/SingletonManager.h>

#include <Common/Database/DatabaseManager.h>
#include <Common/Database/DatabaseControllerFactory.h>

#include "DatabaseInstance.h"

QHash<QString, DatabaseManager*> DatabaseInstance::g_listManagers = QHash<QString, DatabaseManager*>();
DatabaseErrorHandler DatabaseInstance::g_errorHandler = 0;
DatabaseControllerFactory* DatabaseInstance::g_pDCFactory = nullptr;

DatabaseInstanceEntry::DatabaseInstanceEntry()
{
	m_pConnection = nullptr;
	m_pDBC = nullptr;
}

DatabaseInstanceEntry::~DatabaseInstanceEntry()
{

}

void DatabaseInstanceEntry::dispose()
{
	if(m_pDBC){
		delete m_pDBC;
		m_pDBC = NULL;
	}
	if(m_pConnection){
		delete  m_pConnection;
		m_pConnection = NULL;
	}
}

DatabaseInstance::DatabaseInstance()
{
	m_bConnectionAutoLock = false;
}

DatabaseInstance::DatabaseInstance(const QString& szId)
{
	m_szId = szId;
	m_bConnectionAutoLock = false;
}

DatabaseInstance::~DatabaseInstance()
{
	dispose();
}

void DatabaseInstance::dispose()
{
	disposeControllers();
}

void DatabaseInstance::setConnectionAutoLock(bool bAutoLock)
{
	m_bConnectionAutoLock = bAutoLock;
}

DatabaseManager* DatabaseInstance::dbm(const QString& szDBName)
{
	DatabaseManager* pDatabaseManager = NULL;

	if(g_listManagers.empty()){
		SingletonManager::addDisposeCallback(disposeManagers);
	}

	if(!g_listManagers.contains(szDBName)){
		pDatabaseManager = new DatabaseManager();
		g_listManagers.insert(szDBName, pDatabaseManager);
	}else{
		pDatabaseManager = g_listManagers[szDBName];
	}
	return pDatabaseManager;
}

void DatabaseInstance::disposeManagers()
{
	QHash<QString, DatabaseManager*>::iterator iter;
	iter = g_listManagers.begin();
	while(iter != g_listManagers.end())
	{
		DatabaseManager* pDatabaseManager = iter.value();
		iter = g_listManagers.erase(iter);
		pDatabaseManager->dispose();
		delete pDatabaseManager;
	}
}

DatabaseController& DatabaseInstance::dbc(const QString& szDBName)
{
	DatabaseInstanceEntry* pDatabaseInstanceEntry = NULL;
	if(!m_listEntries.contains(szDBName)){
		pDatabaseInstanceEntry = new DatabaseInstanceEntry();

		// Create connection
		QString szConnectionName = buildConnectionName(szDBName, m_szId);
		pDatabaseInstanceEntry->m_pConnection = dbm(szDBName)->getNewConnection(szConnectionName);

		// Create controller
		if(g_pDCFactory) {
			pDatabaseInstanceEntry->m_pDBC = g_pDCFactory->create(szDBName, pDatabaseInstanceEntry->m_pConnection, g_errorHandler);
		}

		m_listEntries.insert(szDBName, pDatabaseInstanceEntry);
	}else{
		pDatabaseInstanceEntry = m_listEntries[szDBName];
	}
	return *pDatabaseInstanceEntry->m_pDBC;
}

void DatabaseInstance::disposeControllers()
{
	QHash<QString, DatabaseInstanceEntry*>::iterator iter;
	iter = m_listEntries.begin();
	while(iter != m_listEntries.end())
	{
		DatabaseInstanceEntry* pDatabaseInstanceEntry = iter.value();
		iter = m_listEntries.erase(iter);
		pDatabaseInstanceEntry->dispose();
		delete pDatabaseInstanceEntry;
	}
}

QString DatabaseInstance::buildConnectionName(const QString& szName, const QString& szId)
{
	if(szId.isEmpty()){
		return QString("qt_%0_connection").arg(szName);
	}
	return QString("qt_%0_connection_%1").arg(szName).arg(szId);
}

void DatabaseInstance::setErrorHandler(DatabaseErrorHandler errorHandler)
{
	g_errorHandler = errorHandler;
}

void DatabaseInstance::setDatabaseControllerFactory(DatabaseControllerFactory* pDCFactory)
{
	g_pDCFactory = pDCFactory;
}