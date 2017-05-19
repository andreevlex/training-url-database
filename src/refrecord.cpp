#include <algorithm>
#include <iostream>
#include <QVariant>
#include <QtSql>
#include <typeinfo>

#include "refrecord.h"
#include "workingdb.h"

RefRecord::RefRecord()
{
    setDateCreate(QDateTime::currentDateTimeUtc());    
}

RefRecord::RefRecord(const QString& name, const QString& url):
    name_m(name),
    url_m(url)
{
    setDateCreate(QDateTime::currentDateTimeUtc());
}

RefRecord::RefRecord(const RefRecord& other)
{
    id_m          = other.id_m;
    name_m        = other.name_m;
    url_m         = other.url_m;
    favorite_m    = other.favorite_m;
    modified_m    = other.modified_m;

    tags_m.clear();
    for(TagsList::const_iterator it_tag = other.tags_m.begin(); it_tag != other.tags_m.end(); ++it_tag)
    {
       if( const Tag *other_tag = dynamic_cast<const Tag *>(*it_tag) )
       {
           Tag* newTag = new Tag(*other_tag);
           tags_m.add(newTag);
       }
    }
    modified_m = true;
}

RefRecord::~RefRecord()
{}

void RefRecord::setName(const QString& name)
{
    if( !name.isEmpty() )
    {
        name_m      = name;
        modified_m  = true;
    }
}

QString RefRecord::getName(void) const
{
    return name_m;
}

void RefRecord::setUrl(const QString & url)
{
    if( !url.isEmpty() )
    {
        url_m         = url;
        modified_m    = true;
    }
}

QString RefRecord::getUrl(void) const
{
    return url_m;
}

void RefRecord::setID(const long long int id)
{

    if( id < 0 )
    {
        std::cerr << "Не достутимое значение ID" << std::endl;
        return;
    }

    id_m          = id;
    modified_m    = true;
}

long long RefRecord::getID() const
{
    return id_m;
}

void RefRecord::setFavorite(const bool fav)
{
    favorite_m = fav;
    modified_m = true;
}

bool RefRecord::getFavorite(void) const
{
    return favorite_m;
}

void RefRecord::setDateCreate(const QDateTime& dateCreate)
{
    QDateTime date_t_min = QDateTime::currentDateTime().addDays(-1);

    if( dateCreate < date_t_min )
    {
        std::cout << "Новая дата не может быть меньше 01.01.2017" << std::endl;
        return;
    }
    dateCreate_m = dateCreate;
    modified_m = true;
}

QDateTime RefRecord::getDateCreate(void) const
{
    return dateCreate_m;
}

bool RefRecord::getModified() const
{
    return modified_m;
}

bool RefRecord::saveData(QSqlQuery &query)
{

    if( checkIsNew() )
    {
       query.prepare("INSERT INTO refs (datecreate, favorite, name, url ) VALUES (:datecreate, :favorite, :name, :url)");
    }
    else
    {
        query.prepare("UPDATE refs SET datecreate = :datecreate, favorite = :favorite, name = :name, url = :url WHERE id = :ID");
        query.bindValue(":ID", id_m);
    }

    query.bindValue(":datecreate",  dateCreate_m.toString(Qt::ISODate));
    query.bindValue(":favorite",    favorite_m);
    query.bindValue(":name",        name_m);
    query.bindValue(":url",         url_m);

    if( !query.exec() )
    {
        std::cerr << query.lastError().text().toStdString();
        return false;
    }

    if( checkIsNew() )
    {
       setID(query.lastInsertId().toLongLong());
    }

    if( !tags_m.save() )
    {
        return false;
    }

    return true;
}

bool RefRecord::checkIsNew() const
{
    return ( 0 == id_m );
}

bool RefRecord::addTag(Tag *value)
{
    if( checkIsNew() )
    {
        return false;
    }

    if( value->isNew() || value->isModified() )
    {
        if( !value->save() )
        {
            return false;
        }
    }
    QSqlQuery query(currentDatabase());
    query.prepare("INSERT INTO tags_refs (ref_id, tag_id) VALUES (:RID, :TID)");
    query.bindValue(":RID",  id_m);
    query.bindValue(":TID",  value->getID());

    if( !query.exec() )
    {
        std::cerr << query.lastError().text().toStdString();
        return false;
    }

    tags_m.add(value);
    return true;
}

bool RefRecord::removeTag(const Tag &value)
{
    QSqlQuery query(currentDatabase());
    query.prepare("DELETE FROM tags_refs WHERE ref_id = :RID and tag_id = :TID");
    query.bindValue(":RID",  id_m);
    query.bindValue(":TID",  value.getID());

    if( !query.exec() )
    {
        std::cerr << query.lastError().text().toStdString();
        return false;
    }
    return true;
}

TagsList& RefRecord::getTags()
{
    return tags_m;
}

bool operator ==(const RefRecord& loper, const RefRecord& roper)
{
    return loper.getID() == roper.getID();
}

bool operator !=(const RefRecord& loper, const RefRecord& roper)
{
    return !(loper == roper);
}
