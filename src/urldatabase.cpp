#include "urldatabase.h"
#include "config.h"
#include <iostream>
#include <QDir>
#include <iterator>
#include <memory>
#include <QFileInfo>

UrlDatabase::UrlDatabase()
{
    db = QSqlDatabase::addDatabase(TYPE_DB);
    db.setDatabaseName(BASE_NAME);
}

UrlDatabase::UrlDatabase(const QString& dbname_)
{
    db = QSqlDatabase::addDatabase(TYPE_DB);

    if( dbname_.isEmpty() )
    {
        db.setDatabaseName(BASE_NAME);
        return;
    }

    QString pathToDB = dbname_;
    qDebug() << pathToDB;

    QFileInfo checkFile(pathToDB);

    if (checkFile.isFile()) {
        db.setDatabaseName(pathToDB);
    }
}

UrlDatabase::~UrlDatabase()
{
    db.close();
}

bool UrlDatabase::openDB()
{
    if( db.isOpen() )
    {
        return true;
    }

    bool result = db.open();
    if( result )
    {
        Update();
    }
    return result;
}

bool UrlDatabase::saveChangesDB()
{

    if( !db.isOpen() )
    {
        std::cerr << "Необходимо сначала открыть базу данных!" << std::endl;
        return false;
    }

    if( Refs.empty() )
    {
        return true;
    }

    for (auto it = Refs.begin(); it != Refs.end(); it++)
    {

        if( it->getModified() )
        {
            QSqlQuery query(db);

            query.prepare("UPDATE refs "
                          "SET "
                          "datecreate = :datecreate, "
                          "favorite = :favorite, "
                          "name = :name, "
                          "url = :url "
                          "WHERE id = :ID ");
            query.bindValue(":datecreate", it->getDateCreate().toString(Qt::ISODate));
            query.bindValue(":favorite", it->getFavorite());
            query.bindValue(":name", it->getName());
            query.bindValue(":url", it->getUrl());
            query.bindValue(":ID", it->getID());

            if( !query.exec() )
            {
                qDebug() << query.lastError().text();
                return false;
            }
            else
            {
                for(auto ittags = it->firstTag(); ittags != it->lastTag(); ittags++)
                {
                    QSqlQuery queryTag(db);

                    query.prepare("UPDATE tags "
                                  "SET "
                                  "TagName = :TAGNAME "
                                  "WHERE fk_id = :ID ");
                    query.bindValue(":TAGNAME", ittags->getName());
                    query.bindValue(":ID", it->getID());

                    if( !query.exec() )
                    {
                       qDebug() << query.lastError().text();
                       return false;
                    }
                }

                return true;
            }
        }
    }

    return true;

}

void UrlDatabase::closeDB()
{
    db.close();
    Refs.clear();
}

bool UrlDatabase::addRef(RefRecord& newref_)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO refs (datecreate, favorite, name, url ) "
                      "VALUES (:datecreate, :favorite, :name, :url)");
    query.bindValue(":datecreate", newref_.getDateCreate().toString(Qt::ISODate));
    query.bindValue(":favorite", static_cast<int>(newref_.getFavorite()));
    query.bindValue(":name", newref_.getName());
    query.bindValue(":url", newref_.getUrl());

    if( !query.exec() )
    {
        qDebug() << query.lastError().text();
        return false;
    }
    else
    {
        newref_.setID(query.lastInsertId().toLongLong());
        for(auto it = newref_.firstTag(); it != newref_.lastTag(); it++)
        {
            addTag(newref_, *it);
        }

        Update();
        return true;
    }
}

bool UrlDatabase::delRef(const RefRecord& delref_)
{

    QSqlQuery query(db);

    query.prepare("DELETE FROM tags WHERE tags.fk_id = :ID"
                  ";"
                  "DELETE FROM refs WHERE refs.id = :ID");
    query.bindValue(":ID", delref_.getID());

    if( !query.exec() )
    {
        qDebug() << query.lastError().text();
        return false;
    }
    else
    {
        Update();
        return true;
    }

}

size_t UrlDatabase::refsCount(void) const
{
    return Refs.size();
}

const RefRecord& UrlDatabase::getRef(const long long int id_, bool& refFind) const
{
    refFind = false;
    for( auto it = Refs.begin(); it != Refs.end(); it++)
    {
        if( it->getID() == id_ )
        {
            refFind = true;
            return *it;
        }
    }
    return *Refs.begin();
}

size_t UrlDatabase::favoriteRefsCount(void) const
{

    QSqlQuery query(db);

    if( !query.exec("SELECT "
                    "COUNT(*) as cn "
                    "FROM refs "
                    "WHERE refs.favorite = 1") )
    {
        qDebug() << query.lastError().text();
        return 0;
    }

    if( !query.next() )
    {
        return 0;
    }

    size_t countRecord = static_cast<size_t>(query.value(0).toInt());

    return countRecord;
}


bool UrlDatabase::addTag(const RefRecord& parent_, const Tag& newtag_)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO tags (fk_id, TagName) "
                      "VALUES (:FK_ID, :TAGNAME)");
    query.bindValue(":FK_ID", parent_.getID());
    query.bindValue(":TAGNAME", newtag_.getName());

    if( !query.exec() )
    {
        qDebug() << query.lastError().text();
        return false;
    }
    else
    {
        return true;
    }
}

bool UrlDatabase::delTag(const RefRecord& parent_, const Tag deltag_)
{
    QSqlQuery query(db);

    query.prepare("DELETE FROM tags "
                  "WHERE tags.fk_id = :FK_ID AND tags.TagName = :TAGNAME");
    query.bindValue(":FK_ID", parent_.getID());
    query.bindValue(":TAGNAME", deltag_.getName());

    if( !query.exec() )
    {
        qDebug() << query.lastError().text();
        return false;
    }
    else
    {
        return true;
    }
}

size_t UrlDatabase::tagsCount(void) const
{
    QSqlQuery query(db);

    if( !query.exec("SELECT "
                    "Count(DISTINCT tags.TagName) as ctags "
                    "from tags") )
    {
        qDebug() << query.lastError().text();
        return 0;
    }

    if( !query.next() )
    {
        return 0;
    }

    size_t countRecord = static_cast<size_t>(query.value(0).toInt());

    return countRecord;
}

Udb::ListRefs* UrlDatabase::getRefs(void)
{

    Udb::ListRefs* allRefsDB = nullptr;

    QSqlQuery query(db);

    if( !query.exec("SELECT "
                    "refs.datecreate, refs.favorite, refs.id, refs.name, refs.url "
                    "FROM refs") )
    {
        qDebug() << query.lastError().text();
        return allRefsDB;
    }

    allRefsDB = new Udb::ListRefs;

    while( query.next() )
    {
        RefRecord newRef;
        newRef.setName(query.value("name").toString());
        newRef.setUrl(query.value("url").toString());
        newRef.setID(query.value("id").toLongLong());
        newRef.setFavorite(query.value("favorite").toBool());
        QDateTime newDate = QDateTime::fromString(
                    query.value("datecreate").toString(), Qt::ISODate);
        newRef.setDateCreate(newDate);

        QSqlQuery queryTags(db);
        queryTags.prepare("SELECT "
                          "tags.TagName "
                          "FROM tags "
                          "WHERE tags.fk_id= :FK_ID");
        queryTags.bindValue(":FK_ID", newRef.getID());
        if( !queryTags.exec() )
        {
            qDebug() << queryTags.lastError().text();
        }
        else
        {
            while( queryTags.next() )
            {
                Tag newTag;
                newTag.setName(queryTags.value(0).toString());
                newRef.addTag(newTag);
            }
        }

        allRefsDB->push_back(newRef);
    }

    return allRefsDB;
}

Udb::ListRefs* UrlDatabase::getFavoriteRefs(void)
{

    Udb::ListRefs* allFavoriteRefsDB = new Udb::ListRefs;

    QSqlQuery query(db);

    if( !query.exec("SELECT "
                    "refs.datecreate, refs.favorite, refs.id, refs.name, refs.url "
                    "FROM refs "
                    "WHERE refs.favorite = 1") )
    {
        qDebug() << query.lastError().text();
        return allFavoriteRefsDB;
    }

    allFavoriteRefsDB = new Udb::ListRefs;

    while( query.next() )
    {
        RefRecord newRef;
        newRef.setName(query.value("name").toString());
        newRef.setUrl(query.value("url").toString());
        newRef.setID(query.value("id").toLongLong());
        newRef.setFavorite(query.value("favorite").toBool());
        QDateTime newDate = QDateTime::fromString(
                    query.value("datecreate").toString(), Qt::ISODate);
        newRef.setDateCreate(newDate);

        allFavoriteRefsDB->push_back(newRef);
    }

    return allFavoriteRefsDB;
}

Udb::ListTags *UrlDatabase::getUniqTags(void)
{
    Udb::ListTags* allTagsDB = nullptr;

    QSqlQuery query(db);

    if( !query.exec("SELECT DISTINCT tags.TagName "
                    "FROM tags") )
    {
        qDebug() << query.lastError().text();
        return allTagsDB;
    }

    allTagsDB = new Udb::ListTags;

    while( query.next() )
    {
        Tag newTag;
        newTag.setName(query.value(0).toString());
        allTagsDB->push_back(newTag);
    }

    return allTagsDB;
}

void UrlDatabase::Update()
{
    Refs.clear();
    std::unique_ptr<Udb::ListRefs> allRefs(getRefs());
    //std::copy(allRefs->begin(), allRefs->end(), Refs.begin());
    for(auto it = allRefs->begin(); it != allRefs->end(); ++it)
    {
        Refs.push_back(*it);
    }
}
