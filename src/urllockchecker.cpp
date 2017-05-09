
#include "urllockchecker.h"
#include <QEventLoop>
#include <QVariant>

QString UrlLockChecker::baseUrl("https://api.antizapret.info");

UrlLockChecker::UrlLockChecker()
: mng(new QNetworkAccessManager(this))
, checkUrl_()
{

}

UrlLockChecker::UrlLockChecker(const QString& checkurl)
: mng(new QNetworkAccessManager(this))
{
    setCheckUrl(checkurl);
}

UrlLockChecker::~UrlLockChecker()
{
    delete mng;
}

void UrlLockChecker::setCheckUrl(const QString& checkurl)
{
    QUrl newheckurl(checkurl);

    checkUrl_ = newheckurl;

    if( checkUrl_.isValid() )
    {
        hostName_ = checkUrl_.host().replace("www.", "");
    }
    else
    {
        hostName_ = "";
    }
}

QUrl UrlLockChecker::getCheckUrl(void) const
{
    return checkUrl_;
}

QString UrlLockChecker::getHostName(void) const
{
    return hostName_;
}

bool UrlLockChecker::isLock()
{
    bool UrlLock = false;

    if( !checkUrl_.isValid() )
    {
        throw std::invalid_argument("Ссылка не корректна!"
                                    " Запрос невозможен.");
    }

    QString queryStr = buildQueryOneObject(typeFormat::small);
    QUrl urlRequest(queryStr);
    QNetworkRequest request(urlRequest);
    QNetworkReply* reply = mng->get(request);

    // реализуем ожидание конца загрузки
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    // выводим содержимое

    if (reply->error() == QNetworkReply::NoError)
    {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if ( statusCode.isValid() && statusCode.toInt() != 200)
        {
            qDebug() << "Код возврата: " << statusCode.toString();
            reply->deleteLater();
            return UrlLock;
        }

        QByteArray content = reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        QString encReply = codec->toUnicode(content.data());
        if( encReply.at(0) == '1' )
        {
            UrlLock = true;
        }
        else if( 0 == QString::compare("piff-paff", encReply, Qt::CaseInsensitive) )
        {
            qDebug() << "Запрос не корректный: "
                     << queryStr << "\n"
                     << checkUrl_.toString();
            UrlLock = false;
        }
        else
        {
            UrlLock = false;
        }
    }
    else
    {
        qDebug() << reply->errorString();
    }

    reply->deleteLater();

    return UrlLock;
}

bool UrlLockChecker::isValid(void) const
{
    return checkUrl_.isValid();
}

QString UrlLockChecker::buildQueryOneObject(const typeFormat format)
{
    return UrlLockChecker::baseUrl +
            QStringLiteral("/get.php?item=%1&type=%2")
            .arg(hostName_)
            .arg(getStringNameFormat(format));
}

QString UrlLockChecker::getStringNameFormat(const typeFormat format)
{
    QString formatStr;
    switch (format) {
    case typeFormat::small: formatStr = "small";    break;
    case typeFormat::csv:   formatStr = "csv";      break;
    case typeFormat::json:  formatStr = "json";     break;
    case typeFormat::xml:   formatStr = "xml";      break;
    default:                formatStr = "csv";      break;
    }

    return formatStr;
}
