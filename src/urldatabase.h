#ifndef URLDATABASE_H
#define URLDATABASE_H

#include "refrecord.h"
#include "config.h"
#include "tag.h"
#include <QString>
#include <QtSql>
#include <vector>

namespace Udb {
    typedef std::vector<RefRecord> ListRefs;
}

class UrlDatabase
{
public:

    UrlDatabase();
    UrlDatabase(const QString& dbname_);
    ~UrlDatabase();

    bool openDB();
    bool saveChangesDB();
    void Update();
    void closeDB();

    bool addRef(RefRecord& newref_);
    bool delRef(const RefRecord& delref_);
    size_t refsCount(void) const;
    const RefRecord& getRef(const long long int id_, bool& refFind) const;

    size_t favoriteRefsCount(void) const;

    bool addTag(const RefRecord& parent_, const Tag& newtag_);
    bool delTag(const RefRecord& parent_, const Tag deltag_);
    size_t tagsCount(void) const;

    Udb::ListRefs* getRefs(void);
    Udb::ListRefs* getFavoriteRefs(void);
    Udb::ListTags* getUniqTags(void);
private:

    QSqlDatabase db;
    Udb::ListRefs Refs;

};

#endif // URLDATABASE_H
