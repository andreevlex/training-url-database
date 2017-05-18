#ifndef REFRECORD_H
#define REFRECORD_H

#include <QString>
#include <QDateTime>
#include <vector>
#include "tag.h"
#include "datawrite.h"
#include "tagslist.h"

class RefRecord: public DataWrite
{
public:
    RefRecord();
    RefRecord(const QString& name, const QString& url);
    RefRecord(const RefRecord& other);
    ~RefRecord();

    void setName(const QString& name);
    QString getName(void) const;

    void setUrl(const QString& url);
    QString getUrl(void) const;

    void setID(const long long id);
    long long getID() const;

    void setFavorite(const bool fav);
    bool getFavorite(void) const;

    void setDateCreate(const QDateTime& dateCreate);
    QDateTime getDateCreate(void) const;

    bool getModified(void) const;

    TagsList& getTags();
private:
    QString                 name_m;
    QString                 url_m;
    long long int           id_m{0};
    bool                    favorite_m{false};
    bool                    modified_m{false};
    QDateTime               dateCreate_m;
    TagsList                tags_m;

    bool saveData(QSqlQuery &query);
    bool checkIsNew();

};

bool operator ==(const RefRecord& loper, const RefRecord& roper);
bool operator !=(const RefRecord& loper, const RefRecord& roper);

#endif // REFRECORD_H
