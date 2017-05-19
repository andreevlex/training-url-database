#include <iostream>
#include <iosfwd>
#include <QDir>
#include <iterator>
#include <memory>
#include <QFileInfo>
#include <stdexcept>
#include <QTableView>
#include <QTreeView>

#include "urldatabase.h"
#include "tag.h"
#include "workingdb.h"

UrlDatabase::UrlDatabase(QTableView * refRecords, QTableView *tags, QObject *parent)
: QObject(parent)
{

    RefRecordsModel_m = new QSqlTableModel (this, currentDatabase());
    RefRecordsModel_m->setTable("refs");
    RefRecordsModel_m->select();
    RefRecordsModel_m->setEditStrategy(QSqlTableModel::OnRowChange);
    refRecords->setModel(RefRecordsModel_m);

    TagsModel_m = new QSqlTableModel (this, currentDatabase());
    TagsModel_m->setTable("tags");
    TagsModel_m->select();
    TagsModel_m->setEditStrategy(QSqlTableModel::OnRowChange);
    tags->setModel(TagsModel_m);
}

UrlDatabase::~UrlDatabase()
{
    delete RefRecordsModel_m;
    delete TagsModel_m;
}

void UrlDatabase::update()
{
    RefRecordsModel_m->select();
    TagsModel_m->select();
}
