#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenu>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtSql>

#include "urldatabase.h"

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
    void on_pushButton_clicked();
    void showRefs();
    void showTags();
    void showFavoriteRefs();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:

    Ui::MainWindow *ui;

    UrlDatabase* urlDB_m;

    void createMenu();
    void showError(const QSqlError &err);
//    void addLineToRefs(QStandardItemModel* model, const unsigned int lineNum, const RefRecord& currentRef);
    void configureProgram();
};

#endif // MAINWINDOW_H
