#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>

const QString PROGRAM_NAME("UrlDatabase");

#define TYPE_DB "QSQLITE"
const QString DATA_LOCATION( QString("%1/.%2").arg( QDir::homePath(), PROGRAM_NAME ));
const QString BASE_NAME( QString("%1/%2").arg(DATA_LOCATION, "base.db" ));


#endif // CONFIG_H

