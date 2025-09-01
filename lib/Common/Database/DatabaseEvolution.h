/*
 * DatabaseEvolution.h
 *
 *  Created on: 30 juil. 2012
 *      Author: ebeuque
 */

#ifndef DATABASEEVOLUTION_H_
#define DATABASEEVOLUTION_H_

#include <QSqlDatabase>

#include <libqdbevolution/QDbEvolution.h>


class DatabaseEvolutionHandler {
public:
	DatabaseEvolutionHandler();
	virtual ~DatabaseEvolutionHandler();

	virtual bool notifyDbEvolution(const QSqlDatabase& db, const QString& szEvolutionVersion, void* pUserData);
	virtual bool handleDbUpdate(const QSqlDatabase& db, int iDbUpdate);
};

class DatabaseEvolution : public QDbEvolution {
public:
	DatabaseEvolution(const QString& szScriptFilename, QSqlDatabase& db, void* pUserData = NULL);
	virtual ~DatabaseEvolution();

	void setHandler(DatabaseEvolutionHandler* pHandler);

protected:
	virtual bool notifyDbEvolution(const QString& szEvolutionVersion);
	virtual bool handleDbUpdate(const QString& szUpdateValue);
	virtual bool getCurrentDbVersion(QString& szVersion);
	virtual bool setCurrentDbVersion(const QString& szVersion);
	virtual int compareDbVersion(const QString& szVersion1, const QString& szVersion2);
	virtual bool execQuery(const QString& szQuery);

	void raiseError(const QSqlError& error);

private:
	QSqlDatabase& m_db;
	DatabaseEvolutionHandler* m_pHandler;
	void* m_pUserData;
};

#endif /* DATABASEEVOLUTION_H_ */
