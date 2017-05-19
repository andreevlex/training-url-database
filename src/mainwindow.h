#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenu>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtSql>

#include "urldatabase.h"
#include "factorymethod.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showRefs();
    void showTags();
    void showFavoriteRefs();
    void on_pbAddTag_clicked();
    void on_pbDelTag_clicked();
    void on_pbAddRefRecord_clicked();
    void on_pbDelRefRecord_clicked();
    void checkUrl();

private:

    Ui::MainWindow *ui;
    UrlDatabase* urlDB_m;
    RefRecords RefRecords_m;

    void createMenu();
    void showError(const QSqlError &err);
//    void addLineToRefs(QStandardItemModel* model, const unsigned int lineNum, const RefRecord& currentRef);
    void configureProgram();
};

#endif // MAINWINDOW_H
