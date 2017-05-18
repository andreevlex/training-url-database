#include <QtSql/QtSql>
#include "datawrite.h"
#include "workingdb.h"

DataWrite::DataWrite()
{}

DataWrite::~DataWrite()
{

}

bool DataWrite::save()
{
    QSqlQuery preQuery = QSqlQuery(currentDatabase());
    return saveData(preQuery);
}

bool DataWrite::isNew()
{
    return checkIsNew();
}
