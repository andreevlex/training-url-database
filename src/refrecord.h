#ifndef REFRECORD_H
#define REFRECORD_H

#include <QString>
#include <QDateTime>
#include <vector>
#include "tag.h"

namespace Udb {
    typedef std::vector<Tag> ListTags;
}

class RefRecord
{
private:
    QString                 Name;
    QString                 Url;
    long long int           Id{0};
    bool                    Favorite{false};
    bool                    Modified{false};
    QDateTime               dateCreate;
    Udb::ListTags           Tags;

public:

    RefRecord();
    RefRecord(const QString& name_, const QString& url_);
    RefRecord(const RefRecord& rref_);
    ~RefRecord();

    void setName(const QString & name_);
    QString getName(void) const;

    void setUrl(const QString& url_);
    QString getUrl(void) const;

    void setID(const long long int id_);
    long long getID() const;

    void setFavorite(const bool fav_);
    bool getFavorite(void) const;

    void setDateCreate(const QDateTime& dateCreate_);
    QDateTime getDateCreate(void) const;

    void addTag(const Tag& newtag_);
    void delTag(const Udb::ListTags::const_iterator &deltag_);
    Tag getTag(const QString& nametag_) const;
    Udb::ListTags::const_iterator firstTag() const;
    Udb::ListTags::const_iterator lastTag() const;
    size_t tagCount() const;

    bool getModified(void) const;

};

bool operator ==(const RefRecord& loper, const RefRecord& roper);
bool operator !=(const RefRecord& loper, const RefRecord& roper);

#endif // REFRECORD_H
