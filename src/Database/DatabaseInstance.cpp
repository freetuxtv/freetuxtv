/*
 * DatabaseInstance.cpp
 *
 *  Created on: 24 mars 2014
 *      Author: ebeuque
 */

#include "./DatabaseInstance.h"

DatabaseManager* databaseManagerLocal()
{
	return DatabaseInstance::dbm(DBNAME_MAIN);
}