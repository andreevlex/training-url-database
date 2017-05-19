#include <memory>
#include <QFileSystemModel>
#include <QDir>
#include <QFileInfo>
#include <QtSql>
#include <QMessageBox>

#include "makestring.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tagdialog.h"
#include "refdialog.h"
#include "aboutdialog.h"
#include "urldatabase.h"
#include "config.h"
#include "urllockchecker.h"
#include "initdb.h"
#include "workingdb.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    configureProgram();

    // initialize the database
    QSqlError err = initDB();
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }

    ui->setupUi(this);

    urlDB_m = new UrlDatabase(ui->RefRecordTV, ui->TagsTV, this);
    urlDB_m->showRefs();
    urlDB_m->showTags();

    RefRecords_m =  new RefRecords;
    Tags_m = new Tags;

    createMenu();
}

MainWindow::~MainWindow()
{
    delete RefRecords_m;
    delete Tags_m;
    delete urlDB_m;
    delete ui;
}

void MainWindow::configureProgram()
{
    QDir dconf;
    dconf.setPath(DATA_LOCATION);

    if( !dconf.exists() )
    {
        dconf.mkpath(DATA_LOCATION);
    }
}

void MainWindow::createMenu()
{

    QMenu * mnFile = new QMenu("Файл"); // создаём меню Файл

    QAction * exitAction = new QAction("Выход", mnFile);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    mnFile->addAction(exitAction);

    QMenu * mnEdit = new QMenu("Правка"); // создаём меню Edit
    QMenu * mnService = new QMenu("Сервис"); // Меню Сервис
    // ----------- здесь добавляем пункты меню и подключаем его к слоту----
    QAction * showRefsAction = new QAction("Показать ссылки", mnService);
    connect(showRefsAction, SIGNAL(triggered()), this, SLOT(showRefs()));
    mnService->addAction(showRefsAction);

    QAction * showFavoriteRefsAction = new QAction("Показать избранное", mnService);
    connect(showFavoriteRefsAction, SIGNAL(triggered()), this, SLOT(showFavoriteRefs()));
    mnService->addAction(showFavoriteRefsAction);

    QAction * showTagsAction = new QAction("Показать тэги", mnService);
    connect(showTagsAction, SIGNAL(triggered()), this, SLOT(showTags()));
    mnService->addAction(showTagsAction);

    QAction * checkUrlAction = new QAction("Проверить URL", mnService);
    connect(checkUrlAction, SIGNAL(triggered()), this, SLOT(checkUrl()));
    mnService->addAction(checkUrlAction);

    QMenu * mnHelp = new QMenu("Помощь");
    QAction * AboutAction = new QAction("О программе...", mnHelp);
    connect(AboutAction, SIGNAL(triggered()), this, SLOT(about()));
    mnHelp->addAction(AboutAction);

    ui->menuBar->addMenu(mnFile);
    ui->menuBar->addMenu(mnEdit);
    ui->menuBar->addMenu(mnService);
    ui->menuBar->addMenu(mnHelp);
}


void MainWindow::about()
{
    AboutDialog about_di(this);
    about_di.show();
    about_di.activateWindow();
    about_di.exec();
}

void MainWindow::showRefs()
{
    urlDB_m->showRefs();
}

void MainWindow::showFavoriteRefs()
{
    urlDB_m->showFavoriteRefs();
}

void MainWindow::showTags()
{
    urlDB_m->showTags();
}

void MainWindow::checkUrl()
{
    QAbstractItemModel * model = ui->RefRecordTV->model();
    QItemSelectionModel * selection = ui->RefRecordTV->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    if(indexes.isEmpty())
    {
       return;
    }

    QModelIndex current = indexes.first();
    QVariant data = model->data(current);
    bool okConv = false;
    long long id_ref = data.toLongLong(&okConv);

    if(!okConv)
    {
        return;
    }

    std::unique_ptr<RefRecord> refDB(dynamic_cast<RefRecord*>(RefRecords_m->findByCode(id_ref)));

    if(!refDB)
    {
        return;
    }

    QString selected_text = refDB->getUrl();

    UrlLockChecker newChecker(selected_text);

    if(newChecker.isValid())
    {
        if(newChecker.isLock())
        {
            QMessageBox::information(this, "",selected_text+ " в списке запрета!\n");
        }
        else
        {
            QMessageBox::information(this, "", selected_text + " сайт не входит в список\n");
        }
    }
}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Error Database",
                "Ошибка работы с базой данный: " + err.text());
}

void MainWindow::on_pbAddTag_clicked()
{
    TagDialog tag_di(this);
    tag_di.setAddMode(true);
    tag_di.show();
    tag_di.activateWindow();
    if(tag_di.exec())
    {
        urlDB_m->update();
    }
}

void MainWindow::on_pbDelTag_clicked()
{
    QAbstractItemModel * model = ui->TagsTV->model();
    QItemSelectionModel * selection = ui->TagsTV->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    if(indexes.isEmpty())
    {
       return;
    }

    QModelIndex current = indexes.first();
    QVariant data = model->data(current);
    bool okConv = false;
    long long id_tag = data.toLongLong(&okConv);

    if(!okConv)
    {
        return;
    }

    QSqlError err = Tags::remove(id_tag);
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }
    urlDB_m->update();
}

void MainWindow::on_pbAddRefRecord_clicked()
{
    RefDialog ref_di(this);
    ref_di.setAddMode(true);
    ref_di.show();
    ref_di.activateWindow();
    if(ref_di.exec())
    {
         urlDB_m->update();
    }
}

void MainWindow::on_pbDelRefRecord_clicked()
{
    QAbstractItemModel * model = ui->RefRecordTV->model();
    QItemSelectionModel * selection = ui->RefRecordTV->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    if(indexes.isEmpty())
    {
       return;
    }

    QModelIndex current = indexes.first();
    QVariant data = model->data(current);
    bool okConv = false;
    long long id_ref = data.toLongLong(&okConv);

    if(!okConv)
    {
        return;
    }

    QSqlError err = RefRecords::remove(id_ref);
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }
    urlDB_m->update();
}

void MainWindow::on_pbLinkTag_clicked()
{

}

void MainWindow::on_pbDelLinkTag_clicked()
{

}
