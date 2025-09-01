//
// Created by ebeuque on 05/12/22.
//

#ifndef COMMON_DATABASE_DATABASECONTROLLERFACTORY_H
#define COMMON_DATABASE_DATABASECONTROLLERFACTORY_H

#include <QString>

#include "DatabaseErrorHandler.h"

class DatabaseController;
class DatabaseConnection;

class DatabaseControllerFactory
{
public:
	DatabaseControllerFactory();
	virtual ~DatabaseControllerFactory();

	virtual DatabaseController* create(const QString& szDBName, DatabaseConnection* dbc, DatabaseErrorHandler errorHandler) = 0;
};

#endif // COMMON_DATABASE_DATABASECONTROLLERFACTORY_H
