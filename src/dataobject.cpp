#include <QtSql/QtSql>
#include "dataobject.h"
#include "workingdb.h"

DataObject::DataObject()
{}

DataObject::~DataObject()
{

}

bool DataObject::save()
{
    QSqlQuery preQuery = QSqlQuery(currentDatabase());
    return saveData(preQuery);
}

bool DataObject::isNew() const
{
    return checkIsNew();
}

bool DataObject::isModified() const
{
    return getModified();
}
