#ifndef DATAWRITE_H
#define DATAWRITE_H

#include <QtSql/QSqlQuery>

class DataWrite
{
public:
    DataWrite();
    virtual ~DataWrite();

    bool save();
    bool isNew();
private:
    virtual bool saveData(QSqlQuery &preQuery) = 0;
    virtual bool checkIsNew() = 0;
};

#endif // DATAWRITE_H
