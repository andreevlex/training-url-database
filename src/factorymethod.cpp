#include <iostream>

#include "factorymethod.h"
#include "workingdb.h"
#include "tagslist.h"

DataObject *RefRecords::create()
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
            newReference->setDateCreate(query.value("datecreate").toDateTime());
            newReference->setFavorite(query.value("favorite").toBool());
            newReference->setUrl(query.value("url").toString());

            createTags(newReference);
            return newReference;
        }
    }
    return newReference;
}

void RefRecords::createTags(RefRecord *parent)
{
    QSqlQuery query(currentDatabase());
    query.prepare("select t.id, t.name from tags as t join tags_refs as tr on (t.id = tr.tag_id) where tr.ref_id = :ID");
    query.bindValue(":ID", parent->getID());
    if( !query.exec() )
    {
         std::cerr << query.lastError().text().toStdString();
         return;
    }
    else
    {
        TagsList& parent_tags = parent->getTags();
        while( query.next() )
        {
            Tag* newTag = new Tag;
            newTag->setID(query.value("id").toLongLong());
            newTag->setName(query.value("name").toString());
            parent_tags.add(newTag);
        }
    }
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

QSqlError RefRecords::remove(long long id)
{
    QSqlQuery preQuery(currentDatabase());
    preQuery.prepare("DELETE FROM tags_refs WHERE ref_id = :RID1 ");
    preQuery.bindValue(":RID",  id);

    if( !preQuery.exec() )
    {
        return preQuery.lastError();
    }
    preQuery.prepare("DELETE FROM refs WHERE refs.id = :RID ");
    preQuery.bindValue(":RID",  id);

    if( !preQuery.exec() )
    {
        return preQuery.lastError();
    }
    return QSqlError();
}

QSqlError Tags::remove(long long id)
{
    QSqlQuery preQuery(currentDatabase());
    preQuery.prepare("DELETE FROM tags_refs WHERE tag_id = :TID ");
    preQuery.bindValue(":TID",  id);

    if( !preQuery.exec() )
    {
        return preQuery.lastError();
    }

    preQuery.prepare("DELETE FROM tags WHERE tags.id = :TID ");
    preQuery.bindValue(":TID",  id);

    if( !preQuery.exec() )
    {
        return preQuery.lastError();
    }
    return QSqlError();
}
