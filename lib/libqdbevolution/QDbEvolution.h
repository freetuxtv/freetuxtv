/*
 * QDbEvolution.h
 *
 *  Created on: 30 juil. 2012
 *      Author: ebeuque
 */

#ifndef QDBEVOLUTION_H_
#define QDBEVOLUTION_H_

#define DBEVOLUTION_LASTDBVERSION_TAGS  "-- @libdbevolution-lastdbversion="
#define DBEVOLUTION_DBVERSION_TAGS      "-- @libdbevolution-dbversion="
#define DBEVOLUTION_DBUPDATE_TAGS      "-- @libdbevolution-dbupdate="
#define DBEVOLUTION_SEPARATOR_TAGS      "-- @libdbevolution-separator"

#include <QString>


class QDbEvolution {
public:
	QDbEvolution(const QString& szScriptFilename);
	virtual ~QDbEvolution();

protected:
	// Function to be implemented
	virtual bool notifyDbEvolution(const QString& szEvolutionVersion) = 0;
	virtual bool handleDbUpdate(const QString& szUpdateValue) = 0;
	virtual bool getCurrentDbVersion(QString& szVersion) = 0;
	virtual bool setCurrentDbVersion(const QString& szVersion) = 0;
	virtual int compareDbVersion(const QString& szVersion1, const QString& szVersion2) = 0;
	virtual bool execQuery(const QString& szQuery) = 0;

public:
	bool hasVersion();
	bool doCreation();
	bool doEvolution();

private:
	QString m_szScriptFilename;
};

#endif /* QDBEVOLUTION_H_ */
