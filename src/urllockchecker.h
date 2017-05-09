#ifndef URLLOCKCHECKER_H
#define URLLOCKCHECKER_H

#include <QString>
#include <QDateTime>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QTextCodec>
//#include "lockitem.h"

class UrlLockChecker: public QObject
{

    Q_OBJECT

public:

    UrlLockChecker();
    UrlLockChecker(const QString& checkurl);
    ~UrlLockChecker();

    void setCheckUrl(const QString& checkurl);
    QUrl getCheckUrl(void) const;

    QString getHostName(void) const;

    bool isLock();
    bool isValid(void) const;

    static QString baseUrl;

private:
    enum class typeFormat
    {
        small,
        csv,
        json,
        xml
    };

    QNetworkAccessManager* mng;
    QUrl checkUrl_;
    QString hostName_{""};

    QString buildQueryOneObject(const typeFormat format);
    QString getStringNameFormat(const typeFormat format);
};


#endif // URLLOCKCHECKER_H
