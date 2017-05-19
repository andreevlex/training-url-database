#ifndef URLDATABASE_H
#define URLDATABASE_H

#include "refrecord.h"
#include "config.h"
#include "tag.h"
#include <QString>
#include <QtSql/QtSql>
#include <vector>

class QTableView;
class QSqlTableModel;
class QSqlQueryModel;

class UrlDatabase: public QObject
{
    Q_OBJECT
public:

    UrlDatabase(QTableView * refRecords, QTableView * tags, QObject *parent = 0);
    virtual ~UrlDatabase();

protected:

    QSqlTableModel *RefRecordsModel_m;
    QSqlTableModel *TagsModel_m;
private:
};

#endif // URLDATABASE_H
