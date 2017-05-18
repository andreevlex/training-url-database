
#include <algorithm>
#include "tagslist.h"
#include "tag.h"

TagsList::~TagsList()
{
    for(size_t i=0; i < list_m.size(); ++i)
    {
        delete list_m[i];
    }
}

bool TagsList::save()
{
    bool savecomplete = false;
    for( TagsList::iterator it_tag = list_m.begin(); it_tag != list_m.end(); ++it_tag)
    {
        if( !(*it_tag)->getComposite() )
        {
            if( Tag *other_tag = dynamic_cast<Tag *>(*it_tag) )
            {
                savecomplete = savecomplete && other_tag->save();
            }
        }
    }
    return savecomplete;
}

void TagsList::clear()
{
    for(size_t i=0; i<list_m.size(); ++i)
    {
        delete list_m[i];
    }

    list_m.clear();
}

//Tag TagsList::findByName(const QString& nametag) const
//{
//    for( TagsList::iterator it = list_m.begin(); it != list_m.end(); ++it)
//    {
//        if( it->getElement()->getName() == nametag )
//        {
//            return *(*it->getElement());
//        }
//    }
//    return Tag();
//}

void TagsList::add(AbstractComponent *element){
    list_m.push_back(element);
}

void TagsList::remove(AbstractComponent* element){
    auto finder = std::find(list_m.begin(), list_m.end(), element);
    if( finder != list_m.end() )
    {
       delete *finder;
        list_m.erase(finder);
    }
}

size_t TagsList::count() const
{
    return list_m.size();
}

TagsList::iterator TagsList::begin()
{
    return list_m.begin();
}

TagsList::iterator TagsList::end()
{
    return list_m.end();
}

TagsList::const_iterator TagsList::begin() const
{
    return list_m.begin();
}

TagsList::const_iterator TagsList::end() const
{
    return list_m.end();
}
