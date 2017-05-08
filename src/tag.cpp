#include "tag.h"

#include <QString>
#include <iostream>
#include <regex>

Tag::Tag()
: Name("empty")
{

}

Tag::Tag(const Tag &tag_)
{
    Name = tag_.Name;
}

Tag::~Tag()
{

}

void Tag::setName(const QString & name_)
{

    std::regex re("\\s");

    if( name_.isEmpty() )
    {
        throw std::invalid_argument ("Имя тэга не должно быть пустым.");
    }

    if( regex_match( name_.toStdString(), re) )
    {
        throw std::invalid_argument ("Имя тэга не должно содержать пробелов.");
    }
    else
    {
        Name = name_;
    }

}

QString Tag::getName(void) const
{
    return Name;
}

bool operator ==(const Tag& loper, const Tag& roper)
{
    return (loper.getName() == roper.getName());
}


