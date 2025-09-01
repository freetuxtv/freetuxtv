//
// Created by ebeuque on 01/12/22.
//

#ifndef COMMON_DATABASE_DATABASEERRORHANDLER_H
#define COMMON_DATABASE_DATABASEERRORHANDLER_H

#include <QSqlError>
class DatabaseConnection;

typedef void (*DatabaseErrorHandler)(DatabaseConnection* dbc, const QSqlError& error);

#endif // COMMON_DATABASE_DATABASEERRORHANDLER_H
