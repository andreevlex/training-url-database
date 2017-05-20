#ifndef COMPOSITEPATTERN_H
#define COMPOSITEPATTERN_H

#include <vector>
#include <stddef.h>

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

#endif // COMPOSITEPATTERN_H
