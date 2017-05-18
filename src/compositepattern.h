#ifndef COMPOSITEPATTERN_H
#define COMPOSITEPATTERN_H

#include <vector>
#include <stddef.h>
//#include "tag.h"

class TagsList;

class AbstractComponent
{
  public:
    AbstractComponent() {}
    virtual ~AbstractComponent() {}

    virtual  TagsList* getComposite() {
        return nullptr;
    }
};

class TagsList: public AbstractComponent
{
public:
    typedef std::vector<AbstractComponent*>::iterator iterator;
    typedef std::vector<AbstractComponent*>::const_iterator const_iterator;

    TagsList() {}
    ~TagsList();

    void add(AbstractComponent* element);
    void remove(AbstractComponent* element);
    void clear();
    size_t count() const;
    TagsList* getComposite(){
        return this;
    }

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
    bool save();

    //Tag findByName(const QString& nametag) const;
  private:
   std::vector<AbstractComponent*> list_m;
};

#endif // COMPOSITEPATTERN_H
