#ifndef INITDB_H
#define INITDB_H

#include <QtSql>
#include "config.h"

QSqlError initDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(TYPE_DB);

    db.setDatabaseName(BASE_NAME);
    if (!db.open())
        {return db.lastError();}

    QSqlQuery query_m;
     if ( false == db.tables().contains("refs") )
     {
         if(!query_m.exec( QLatin1String(
            "CREATE TABLE refs ("
            "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
            "`name` TEXT NOT NULL, "
            "`datecreate` TEXT, "
            "`url` TEXT, "
            "`favorite` INTEGER DEFAULT 0 "
            ");") ) )
         {
             return query_m.lastError();
         }
     }
     if ( false == db.tables().contains("tags") )
      {
         if(!query_m.exec( QLatin1String(
            "CREATE TABLE `tags` ("
            "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
            "`name` TEXT "
            ");") ) )
         {
             return query_m.lastError();
         }
     }
     if ( false == db.tables().contains("tags_refs") )
      {
        if(!query_m.exec( QLatin1String(
            "CREATE TABLE `tags_refs` ("
            "`ref_id`   INTEGER,"
            "`tag_id`   INTEGER,"
            "PRIMARY KEY(`ref_id`,`tag_id`)"
            ");") ) )
        {
            return query_m.lastError();
        }
     }

    return QSqlError();
}

#endif // INITDB_H
