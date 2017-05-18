#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QtSql/QSqlQuery>

class DataObject
{
public:
    DataObject();
    virtual ~DataObject();

    bool save();
    bool isNew();
private:
    virtual bool saveData(QSqlQuery &preQuery) = 0;
    virtual bool checkIsNew() = 0;
};

#endif // DATAOBJECT_H
