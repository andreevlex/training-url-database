#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <QtSql>

class DataObject
{
public:
    DataObject();
    virtual ~DataObject();

    bool save();
    bool isNew() const;
    bool isModified() const;
private:
    virtual bool saveData(QSqlQuery &preQuery) = 0;
    virtual bool checkIsNew() const = 0;
    virtual bool getModified() const = 0;
};

#endif // DATAOBJECT_H
