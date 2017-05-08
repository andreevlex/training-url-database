#ifndef TAG_H
#define TAG_H

#include <QString>


class Tag
{
private:

    QString Name;

public:

    Tag();
    Tag(const Tag& tag_);
    ~Tag();

    void setName(const QString& name_);
    QString getName(void) const;

};

bool operator ==(const Tag& loper, const Tag& roper);

#endif // TAG_H
