/*
 * DatabaseEvolution.cpp
 *
 *  Created on: 30 juil. 2012
 *      Author: ebeuque
 */

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>

#include "DatabaseEvolution.h"

#include <Toolkit/QtCompat.h>
#include <Toolkit/p_string.h>

#define DB_T_CONFIG_DB "config"

DatabaseEvolutionHandler::DatabaseEvolutionHandler()
{

}

DatabaseEvolutionHandler::~DatabaseEvolutionHandler()
{

}

bool DatabaseEvolutionHandler::notifyDbEvolution(const QSqlDatabase& db, const QString& szEvolutionVersion, void* pUserData)
{
	return true;
}

bool DatabaseEvolutionHandler::handleDbUpdate(const QSqlDatabase& db, int iDbUpdate)
{
	return true;
}

DatabaseEvolution::DatabaseEvolution(const QString& szScriptFilename, QSqlDatabase& db, void* pUserData)
	: QDbEvolution(szScriptFilename), m_db(db)
{
	m_pHandler = NULL;
	m_pUserData = pUserData;
}

DatabaseEvolution::~DatabaseEvolution()
{

}

void DatabaseEvolution::setHandler(DatabaseEvolutionHandler* pHandler)
{
	m_pHandler = pHandler;
}

bool DatabaseEvolution::notifyDbEvolution(const QString& szEvolutionVersion)
{
	bool bRes = true;
	if(m_pHandler != NULL) {
		bRes = m_pHandler->notifyDbEvolution(m_db, szEvolutionVersion, m_pUserData);
	}
	return bRes;
}

bool DatabaseEvolution::handleDbUpdate(const QString& szUpdateValue)
{
	bool bRes = true;
	if(m_pHandler != NULL) {
		int iDbUpdate = szUpdateValue.toInt(&bRes);
		if(bRes) {
			bRes = m_pHandler->handleDbUpdate(m_db, iDbUpdate);
		}
	}
	return bRes;
}

bool DatabaseEvolution::getCurrentDbVersion(QString& szVersion)
{
	bool bRes = false;

	QSqlQuery query(m_db);

	bRes = query.prepare("SELECT dbversion FROM " DB_T_CONFIG_DB);
	if(bRes) {
		if (query.exec()) {
			QSqlRecord rec = query.record();
			int dbversionCol = rec.indexOf("dbversion");
			while (query.next()) {
				szVersion = query.value(dbversionCol).toString();
				break;
			}
			return true;
		} else {
			qDebug("[Database] Cannot get current database version: %s", qPrintable(query.lastError().text()));
		}
	}else{
		qDebug("[Database] Cannot prepare query to get current database version: %s", qPrintable(query.lastError().text()));
	}

	return false;
}

bool DatabaseEvolution::setCurrentDbVersion(const QString& szVersion)
{
	bool bRes = false;
	QSqlQuery query(m_db);

	QString szTmpVersion;
	if(getCurrentDbVersion(szTmpVersion)){
		if(szTmpVersion.isEmpty()){
			query.prepare("INSERT INTO " DB_T_CONFIG_DB " (dbversion) VALUES (:dbversion)");
		}else{
			query.prepare("UPDATE " DB_T_CONFIG_DB " SET dbversion=:dbversion");
		}
		query.bindValue(":dbversion", szVersion);
		bRes = query.exec();
	}

	return bRes;
}

int DatabaseEvolution::compareDbVersion(const QString& szVersion1, const QString& szVersion2)
{
	int M1, m1, r1, b1;
	int M2, m2, r2, b2;

	p_sscanf(qPrintable(szVersion1), "%d.%d.%d.%d", &M1, &m1, &r1, &b1);
	p_sscanf(qPrintable(szVersion2), "%d.%d.%d.%d", &M2, &m2, &r2, &b2);

	if(M1 > M2){
		return 1;
	}
	if(M1 < M2){
		return -1;
	}
	if(m1 > m2){
		return 1;
	}
	if(m1 < m2){
		return -1;
	}
	if(r1 > r2){
		return 1;
	}
	if(r1 < r2){
		return -1;
	}
	if(b1 > b2){
		return 1;
	}
	if(b1 < b2){
		return -1;
	}
	return 0;
}

bool DatabaseEvolution::execQuery(const QString& szQuery)
{
	QSqlQuery query(m_db);

	if(query.exec(szQuery)){
		return true;
	}
	raiseError(query.lastError());
	return false;
}

void DatabaseEvolution::raiseError(const QSqlError& error)
{
#ifdef USE_QSENATIVEERRORCODE
	qCritical("[Database] (Error %s) %s", qPrintable(error.nativeErrorCode()), qPrintable(error.text()));
#else
	qCritical("[Database] (Error %d) %s", error.number(), qPrintable(error.text()));
#endif
}
