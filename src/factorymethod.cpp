#include <QtSql>
#include <iostream>

#include "factorymethod.h"
#include "workingdb.h"

DataObject* RefRecords::create()
{
  return new RefRecord;
}

DataObject* RefRecords::findByCode(const long long id)
{
    RefRecord* newReference = nullptr;

    QSqlQuery query(currentDatabase());
    query.prepare("select refs.id, refs.name, refs.url, refs.datecreate, refs.favorite from refs where refs.id = :ID");
    query.bindValue(":ID", id);
    if( !query.exec() )
    {
         std::cerr << query.lastError().text().toStdString();
         return newReference;
    }
    else
    {
        if( query.first() )
        {
            newReference = new RefRecord;
            newReference->setID(query.value("id").toLongLong());
            newReference->setName(query.value("name").toString());
            return newReference;
        }
    }
    return newReference;
}

DataObject* Tags::create()
{
  return new Tag;
}

DataObject* Tags::findByCode(const long long id)
{
    Tag* newTag = nullptr;

    QSqlQuery query(currentDatabase());
    query.prepare("select tags.id, tags.name from tags where tags.id = :ID");
    query.bindValue(":ID", id);
    if( !query.exec() )
    {
         std::cerr << query.lastError().text().toStdString();
         return newTag;
    }
    else
    {
        if( query.first() )
        {
            newTag = new Tag;
            newTag->setID(query.value("id").toLongLong());
            newTag->setName(query.value("name").toString());
            return newTag;
        }
    }
    return newTag;
}
