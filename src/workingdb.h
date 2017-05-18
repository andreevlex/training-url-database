#ifndef WORKINGDB_H
#define WORKINGDB_H
#include <QtSql>
#include "config.h"

inline QSqlDatabase currentDatabase()
{
    return QSqlDatabase::database(BASE_NAME);
}

#endif // WORKINGDB_H
