//
// Created by ebeuque on 05/12/22.
//

#include "DatabaseInstance.h"

#include "DatabaseControllerFactoryApplication.h"

DatabaseControllerFactoryApplication::DatabaseControllerFactoryApplication()
{

}

DatabaseControllerFactoryApplication::~DatabaseControllerFactoryApplication()
{

}

DatabaseController* DatabaseControllerFactoryApplication::create(const QString& szDBName, DatabaseConnection* dbc, DatabaseErrorHandler errorHandler)
{
	if(szDBName == DBNAME_MAIN){
		return new DatabaseControllerMain(dbc, errorHandler);
	}
	return NULL;
}