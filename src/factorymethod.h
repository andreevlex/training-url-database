#ifndef FACTORYMETHOD_H
#define FACTORYMETHOD_H

#include <QString>

#include "dataobject.h"
#include "refrecord.h"

class DataObject;

class DataObjectFactory
{
  public:
    virtual DataObject* create() = 0;
    virtual DataObject* findByCode(const long long id) = 0;
    virtual ~DataObjectFactory() {}
};

class RefRecords: public DataObjectFactory
{
  public:
    DataObject* create();
    DataObject* findByCode(const long long id);
  private:
    void createTags(RefRecord* parent);
};

class Tags: public DataObjectFactory
{
  public:
    DataObject* create();
    DataObject* findByCode(const long long id);
};

#endif // FACTORYMETHOD_H
