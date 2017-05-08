#include "refrecord.h"
#include <algorithm>
#include <iostream>

RefRecord::RefRecord()
{}

RefRecord::RefRecord(const QString & name_, const QString & url_)
    :Name(name_), Url(url_)
{}

RefRecord::RefRecord(const RefRecord &rref_)
{
    Id          = rref_.Id;
    Name        = rref_.Name;
    Url         = rref_.Url;
    Favorite    = rref_.Favorite;
    Modified    = rref_.Modified;
    Tags        = rref_.Tags;
}

RefRecord::~RefRecord()
{}

void RefRecord::setName(const QString & name_)
{
    if( !name_.isEmpty() )
    {
        Name        = name_;
        Modified    = true;
    }
}

QString RefRecord::getName(void) const
{
    return Name;
}

void RefRecord::setUrl(const QString & url_)
{
    if( !url_.isEmpty() )
    {
        Url         = url_;
        Modified    = true;
    }
}

QString RefRecord::getUrl(void) const
{
    return Url;
}

void RefRecord::setID(const long long int id_)
{

    if( id_ < 0 ){
        std::cerr << "Не достутимое значение ID" << std::endl;
        return;
    }

    Id          = id_;
    Modified    = true;
}

long long RefRecord::getID() const
{
    return Id;
}

void RefRecord::setFavorite(const bool fav_)
{
    Favorite = fav_;
    Modified = true;
}

bool RefRecord::getFavorite(void) const
{
    return Favorite;
}

void RefRecord::setDateCreate(const QDateTime & dateCreate_)
{
    QDate dateMin(2017, 01, 01);
    QDateTime date_t_min(dateMin);

    if( dateCreate_ < date_t_min )
    {
        std::cout << "Новая дата не может быть меньше 01.01.2017" << std::endl;
        return;
    }
    dateCreate = dateCreate_;
    Modified = true;
}

QDateTime RefRecord::getDateCreate(void) const
{
    return dateCreate;
}

void RefRecord::addTag(const Tag & newtag_)
{
    if( !Tags.empty() )
    {
        auto finder = std::find(Tags.begin(), Tags.end(), newtag_);
        if( finder != Tags.end() )
        {
            return;
        }
    }

    Tags.push_back(newtag_);
    Modified = true;
}

void RefRecord::delTag(const Udb::ListTags::const_iterator& deltag_)
{
   Tags.erase(deltag_);
   Modified = true;
}

Tag RefRecord::getTag(const QString & nametag_) const
{
    if( !Tags.empty() )
    {
        for( auto it = Tags.begin(); it != Tags.end(); ++it)
        {
            if( it->getName() == nametag_ )
            {
                return *it;
            }
        }
        return Tag();
    }

    return Tag();
}

Udb::ListTags::const_iterator RefRecord::firstTag() const
{
    return Tags.begin();
}

Udb::ListTags::const_iterator RefRecord::lastTag() const
{
    return Tags.end();
}

size_t RefRecord::tagCount() const
{
    return Tags.size();
}

bool RefRecord::getModified() const
{
    return Modified;
}

bool operator ==(const RefRecord& loper, const RefRecord& roper)
{
    return loper.getID() == roper.getID();
}

bool operator !=(const RefRecord& loper, const RefRecord& roper)
{
    return !(loper == roper);
}
