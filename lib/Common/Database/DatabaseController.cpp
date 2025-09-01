//
// Created by ebeuque on 10/08/2022.
//

#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>

#include "DatabaseManager.h"

#include "DatabaseController.h"

DatabaseController::DatabaseController(DatabaseConnection* dbc, DatabaseErrorHandler errorHandler)
{
	m_dbc = dbc;
	m_errorHandler = errorHandler;
}

DatabaseController::~DatabaseController()
{

}

QSqlDatabase& DatabaseController::db() const
{
	return *m_dbc->db();
}

bool DatabaseController::open(const char* szLog)
{
	return m_dbc->open(szLog);
}

void DatabaseController::close(const char* szLog)
{
	m_dbc->close(szLog);
}

void DatabaseController::raiseError(const QSqlError& error)
{
	DatabaseManager::raiseError(m_dbc, error);
	if(m_errorHandler){
		m_errorHandler(m_dbc, error);
	}
}

bool DatabaseController::beginTransaction()
{
	return m_dbc->beginTransaction();
}

bool DatabaseController::endTransaction(bool bCommit)
{
	return m_dbc->endTransaction(bCommit);
}

bool DatabaseController::updateJournalModeWAL()
{
	bool bRes ;
	QString szTmp;

	// Be sure no query is active to switch journal_mode
	QSqlQuery query(db());
	bRes = query.exec("PRAGMA journal_mode=WAL");
	if(bRes){
		bRes = query.next();
		if (bRes){
			szTmp = query.value(0).toString();
			bRes = (szTmp.toLower() == "wal");
			if(!bRes){
				qCritical("[Database] Unable to set WAL mode: %s", qPrintable(szTmp));
			}
		}else{
			qCritical("[Database] No row after journal_mode set");
		}
	}else{
		raiseError(query.lastError());
	}

	return bRes;
}

bool DatabaseController::compactDatabase()
{
	bool bRes = false;

	QSqlQuery query(db());

	bRes = query.exec("VACUUM");
	if(!bRes){
		raiseError(query.lastError());
	}

	return bRes;
}



int DatabaseController::getUpdateVersion(const QString& szTable, const QString& szField)
{
	int iRes = -1;

	QString szQuery = QString("SELECT %0 FROM %1").arg(szField).arg(szTable);

	QSqlQuery query(db());
	if(query.exec(szQuery)){
		QSqlRecord rec = query.record();
		if (query.next()){
			// Get the current update version
			iRes = query.value(0).toUInt();
		}else{
			qCritical("[Database] Table %s is empty", qPrintable(szTable));
		}
	}else{
		raiseError(query.lastError());
	}
	return iRes;
}

bool DatabaseController::updatePaths(const QString& szTable, const QString& szField, const QDir& dirOld, const QDir& dirNew)
{
	bool bRes;

	QSqlQuery query(db());

	// Fix recording_export.download_filepath
	QString szQuery = "UPDATE %0 SET %1=REPLACE(%1, :old_path, :new_path)";
	szQuery = szQuery.arg(szTable).arg(szField);
	bRes = query.prepare(szQuery);
	if(bRes){
		// With relative path
		query.bindValue(":old_path", dirOld.path());
		query.bindValue(":new_path", dirNew.path());
		bRes = query.exec();
		if(!bRes){
			raiseError(query.lastError());
		}

		// With absolute path
		if(bRes){
			query.bindValue(":old_path", dirOld.absolutePath());
			query.bindValue(":new_path", dirNew.absolutePath());
			bRes = query.exec();
			if(!bRes){
				raiseError(query.lastError());
			}
		}
	}else{
		raiseError(query.lastError());
	}

	return bRes;
}

bool DatabaseController::isTableExists(const QString& szTable, const QString& szDb)
{
	QString szWhere;
	szWhere = QString("type='table' AND name='%0'").arg(szTable);

	QString szTableMaster;
	if(szDb.isEmpty()){
		szTableMaster = QString("sqlite_master").arg(szDb);
	}else{
		szTableMaster = QString("%0.sqlite_master").arg(szDb);
	}
	return rowCount(szTableMaster, szWhere);
}

int DatabaseController::rowCount(const QString& szTable, const QString& szWhere)
{
	int iRowCount = -1;

	bool bRes = false;

	QSqlQuery query(db());
	QVariant variant;

	QString szTmp = "SELECT count(*) FROM " + szTable + " ";
	if(!szWhere.isEmpty()){
		szTmp += "WHERE " + szWhere;
	}
	bRes = query.exec(szTmp);
	if(bRes){
		QSqlRecord rec = query.record();
		while (query.next()){
			iRowCount = query.value(0).toUInt();
		}
	}else{
		raiseError(query.lastError());
	}

	return iRowCount;
}

bool DatabaseController::deleteIdList(const QString& szTable, const QList<unsigned int>& listId, const QString& szLabel)
{
	return deleteIdList(szTable, "id", listId, QString(), szLabel);
}

bool DatabaseController::deleteIdList(const QString& szTable, const QString& szField, const QList<unsigned int>& listId, const QString& szCondition, const QString& szLabel)
{
	bool bRes = false;

	int iNb = 0;

	qDebug("[Database] Number of %s to delete: %d", qPrintable(szLabel), (int)listId.count());

	unsigned int iMin, iMax;

	iMin = 0;
	iMax = 0;

	QString szQuery = QString("DELETE FROM %0 WHERE %1 BETWEEN :min AND :max").arg(szTable).arg(szField);
	if(!szCondition.isEmpty()){
		szQuery += " AND (" + szCondition + ")";
	}

	QSqlQuery query(db());
	bRes = query.prepare(szQuery);
	if(bRes){
		// We assume the list is sorted
		QList<unsigned int>::const_iterator iter;
		for(iter = listId.constBegin(); iter != listId.constEnd(); ++iter){
			iNb++;
			if(iMin == 0){
				iMin = *iter;
				iMax = iMin;
			}else if((*iter == iMax+1) && (iMax - iMin + 1) < 250){
				iMax = *iter;
			}else{
				query.bindValue(":min", iMin);
				query.bindValue(":max", iMax);
				bRes = query.exec();
				if(bRes){
					qDebug("[Database] Count of deleted %s: %d/%d", qPrintable(szLabel), (iNb-1), (int)listId.count());
				}else{
					raiseError(query.lastError());
				}
				iMin = *iter;
				iMax = iMin;
			}
		}
		if(iMin != 0 && iMax != 0){
			query.bindValue(":min", iMin);
			query.bindValue(":max", iMax);

			bRes = query.exec();
			if(bRes){
				qDebug("[Database] Deleted %s: %d/%d", qPrintable(szLabel), iNb, (int)listId.count());
			}else{
				raiseError(query.lastError());
			}
		}
	}else{
		raiseError(query.lastError());
	}

	return bRes;
}

QString DatabaseController::joinListId(const QList<int>& listId)
{
	QString szTmp;
	for(int i=0; i < listId.count(); ++i) {
		if(szTmp.isEmpty()) {
			szTmp = QString::number(listId[i]);
		} else {
			szTmp += ",";
			szTmp += QString::number(listId[i]);
		}
	}
	return szTmp;
}