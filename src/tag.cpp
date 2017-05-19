#include "tag.h"

#include <QString>
#include <iostream>
#include <regex>
#include <QtSql/QtSql>

Tag::Tag(const QString& name)
{
    setName(name);
}

Tag::Tag(const Tag& other)
{
    id_m = other.id_m;
    name_m = other.name_m;
}

Tag::~Tag(){}

void Tag::setID(const long long id)
{
    if( id < 0 ){
        std::cerr << "Не достутимое значение ID" << std::endl;
        return;
    }

    id_m = id;
}

long long Tag::getID() const
{
    return id_m;
}

void Tag::setName(const QString& name)
{
    name_m = name;
}

QString Tag::getName(void) const
{
    return name_m;
}

bool Tag::isValid() const
{
    std::regex re("\\s");

    if( name_m.isEmpty() )
    {
        std::cerr << "Имя тэга не должно быть пустым.";
        return false;
    }

    if( regex_match( name_m.toStdString(), re) )
    {
        std::cerr << "Имя тэга не должно содержать пробелов.";
        return false;
    }

    return true;

}

bool Tag::saveData(QSqlQuery &query)
{

    if( !isValid() )
    {
        return false;
    }

    if( checkIsNew() )
    {
       query.prepare("INSERT INTO tags (name) VALUES (:TAGNAME)");
    }
    else
    {
        query.prepare("UPDATE tags SET name = :TAGNAME WHERE id = :ID ");
        query.bindValue(":ID", id_m);
    }
    query.bindValue(":TAGNAME", name_m);

    if( !query.exec() )
    {

        std::cerr << query.lastError().text().toStdString();
        return false;
    }

    if( checkIsNew() )
    {
       setID(query.lastInsertId().toLongLong());
    }

    return true;
}

bool Tag::checkIsNew()
{
    return ( 0 == id_m );
}

bool operator ==(const Tag& loper, const Tag& roper)
{
    return (loper.getName() == roper.getName());
}
