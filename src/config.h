#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>

const QString PROGRAM_NAME("UrlDataBase");

#define TYPE_DB "QSQLITE"
const QString DATA_LOCATION(QDir::homePath() + "/.local/share/" + PROGRAM_NAME);
const QString BASE_NAME(DATA_LOCATION + "/base.db");


#endif // CONFIG_H

