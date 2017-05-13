#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenu>
#include "urldatabase.h"
#include "QStandardItemModel"
#include "QStandardItem"

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
private:

    Ui::MainWindow *ui;
    UrlDatabase *dbMain;

    void createMenu();
    QString createStringTags(const RefRecord& currentRef);
    void addLineToRefs(QStandardItemModel* model, const unsigned int lineNum, const RefRecord& currentRef);
    void createTreeView();
};

#endif // MAINWINDOW_H
