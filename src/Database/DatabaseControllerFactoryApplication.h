//
// Created by ebeuque on 05/12/22.
//

#ifndef FREETUXTV_DATABASE_DATABASECONTROLLERFACTORYAPPLICATION_H
#define FREETUXTV_DATABASE_DATABASECONTROLLERFACTORYAPPLICATION_H

#include <Common/Database/DatabaseControllerFactory.h>

class DatabaseControllerFactoryApplication : public DatabaseControllerFactory
{
public:
	DatabaseControllerFactoryApplication();
	virtual ~DatabaseControllerFactoryApplication();

private:
	DatabaseController* create(const QString& szDBName, DatabaseConnection* dbc, DatabaseErrorHandler errorHandler);
};

#endif //FREETUXTV_DATABASE_DATABASECONTROLLERFACTORYAPPLICATION_H
