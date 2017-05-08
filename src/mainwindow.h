#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenu>
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

private:

    Ui::MainWindow *ui;
    void createMenu();
    UrlDatabase *dbMain;
};

#endif // MAINWINDOW_H
