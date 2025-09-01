/*
 * DatabaseInstance.h
 *
 *  Created on: 24 mars 2014
 *      Author: ebeuque
 */

#ifndef FREETUXTV_DATABASE_DATABASEINSTANCE_H_
#define FREETUXTV_DATABASE_DATABASEINSTANCE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Common/Database/DatabaseInstance.h>

#include "Database/DatabaseController.h"

class DatabaseManager;

DatabaseManager* databaseManagerLocal();

#define DBNAME_MAIN "main"
#define dbcMain() dbcCast<DatabaseControllerMain>(DBNAME_MAIN)
#define db_main(dbInstance) (dbInstance.dbcMain())

#endif /* FREETUXTV_DATABASE_DATABASEINSTANCE_H_ */
