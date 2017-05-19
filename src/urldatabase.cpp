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
#include "workingdb.h"

UrlDatabase::UrlDatabase(QTableView *refRecords, QTableView *tags, QObject *parent)
: QObject(parent)
{
    refRecords_m = refRecords;
    tags_m = tags;
    RefRecordsModel_m = new QSqlTableModel (this, currentDatabase());
    TagsModel_m = new QSqlTableModel (this, currentDatabase());
    QueryModel_m = new QSqlQueryModel(this);
}

UrlDatabase::~UrlDatabase()
{
    refRecords_m = nullptr;
    tags_m = nullptr;
    delete QueryModel_m;
    delete RefRecordsModel_m;
    delete TagsModel_m;
}

void UrlDatabase::update()
{
    RefRecordsModel_m->select();
    TagsModel_m->select();
}

void UrlDatabase::showRefs()
{

    RefRecordsModel_m->setTable("refs");
    RefRecordsModel_m->select();
    RefRecordsModel_m->setEditStrategy(QSqlTableModel::OnRowChange);
    refRecords_m->setModel(RefRecordsModel_m);
}

void UrlDatabase::showTags()
{
    TagsModel_m->setTable("tags");
    TagsModel_m->select();
    TagsModel_m->setEditStrategy(QSqlTableModel::OnRowChange);
    tags_m->setModel(TagsModel_m);
}

void UrlDatabase::showFavoriteRefs()
{
    QueryModel_m->setQuery("select * from refs where refs.favorite = 1");
    refRecords_m->setModel(QueryModel_m);
}
