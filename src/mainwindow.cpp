#include <memory>
#include <QFileSystemModel>
#include <QDir>
#include <QFileInfo>
#include <QtSql>
#include <QMessageBox>

#include "makestring.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
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

    urlDB_m = new UrlDatabase(ui->RefRecordTV, this);

    createMenu();
    //createTreeView();
}

MainWindow::~MainWindow()
{
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

void MainWindow::on_pushButton_clicked()
{
//    if( dbMain->openDB() )
//    {

//    size_t countt = dbMain->tagsCount();
//    QString sCount = QStringLiteral("Количество уникальных тэгов = %1\n").arg(countt);
//    ui->textEdit1->insertPlainText(sCount);

//    size_t countr = dbMain->refsCount();
//    QString sCount2 = QStringLiteral("Количество ссылок в базе = %1\n").arg(countr);
//    ui->textEdit1->insertPlainText(sCount2);

//    bool refFind = false;
//    long long int four = 4;
//    const RefRecord& refg = dbMain->getRef(four, refFind);
//    if(refFind)
//    {
//        QString sRefInfo = QStringLiteral("id=%1, "
//                                          "name=%2, "
//                                          "url=%3, "
//                                          "date=%, "
//                                          "fav=%4\n").arg(refg.getID())
//                                                     .arg(refg.getName())
//                                                     .arg(refg.getUrl())
//                                                     .arg(refg.getDateCreate().toString())
//                                                     .arg(refg.getFavorite());
//        ui->textEdit1->insertPlainText(sRefInfo);
//    }
//    // add
//    RefRecord newRec;
//    newRec.setName("Вторая ссылка добавлена");
//    newRec.setUrl("www.isa.ru/");
//    newRec.setDateCreate(QDateTime::currentDateTimeUtc());

//    Tag newTag;
//    newTag.setName("tag2");
//    newRec.addTag(newTag);

//    dbMain->addRef(newRec);

//    dbMain->closeDB();
//    }
//    else
//    {
//        ui->textEdit1->insertPlainText("что-то пошло не так");
//    }

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

    QMenu * mnHelp = new QMenu("Помощь"); // Меню помощь
    ui->menuBar->addMenu(mnFile);
    ui->menuBar->addMenu(mnEdit);
    ui->menuBar->addMenu(mnService);
    ui->menuBar->addMenu(mnHelp);
}

//QString MainWindow::createStringTags(const RefRecord& currentRef)
//{
//    QString allTags("");

//    for(auto ittags = currentRef.firstTag(); ittags != currentRef.lastTag(); ++ittags)
//    {
//        allTags += ittags->getName() + " ";
//    }
//    return allTags;
//}

void MainWindow::addLineToRefs(QStandardItemModel* model, const unsigned int lineNum, const RefRecord& currentRef)
{
    QStandardItem *item;

    item = new QStandardItem(QStringLiteral("%1").arg(currentRef.getID()));
    model->setItem(lineNum, 0, item);

    item = new QStandardItem(currentRef.getName());
    model->setItem(lineNum, 1, item);

    item = new QStandardItem(currentRef.getUrl());
    model->setItem(lineNum, 2, item);

    item = new QStandardItem("");
    model->setItem(lineNum, 3, item);

    /* реализация не функционального требования */
    UrlLockChecker newChecker(currentRef.getUrl());
    QString lockedStr("");

    if( newChecker.isValid() )
    {
      lockedStr = newChecker.isLock() ? "ДА" : "НЕТ";
    }
    else
    {
        lockedStr = "";
    }

    item = new QStandardItem(lockedStr);
    model->setItem(lineNum, 4, item);
    /* реализация не функционального требования */
}

void MainWindow::showRefs()
{

    QStandardItemModel *model = new QStandardItemModel;

    // Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("id");
    horizontalHeader.append("name");
    horizontalHeader.append("url");
    horizontalHeader.append("tags");
    horizontalHeader.append("locked");


    model->setHorizontalHeaderLabels(horizontalHeader);

    std::unique_ptr<Udb::ListRefs> allRefs(urlDB_m->getRefs());

    if(allRefs.get() != nullptr)
    {
        int i = 0;
        for(auto it = allRefs->begin(); it != allRefs->end(); ++it)
        {
            addLineToRefs(model, i, *it);
            i++;
        }
    }

    ui->RefRecordTV->setModel(model);
    ui->RefRecordTV->resizeRowsToContents();
    ui->RefRecordTV->resizeColumnsToContents();

}

void MainWindow::showFavoriteRefs()
{
    QStandardItemModel *model = new QStandardItemModel;

    // Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("id");
    horizontalHeader.append("name");
    horizontalHeader.append("url");
    horizontalHeader.append("tags");
    horizontalHeader.append("locked");

    model->setHorizontalHeaderLabels(horizontalHeader);

    std::unique_ptr<Udb::ListRefs> allFavoriteRefs(urlDB_m->getFavoriteRefs());

    if(allFavoriteRefs.get() != nullptr)
    {
        int i = 0;
        for(auto it = allFavoriteRefs->begin(); it != allFavoriteRefs->end(); ++it)
        {
            addLineToRefs(model, i, *it);

            i++;
        }
    }

    ui->RefRecordTV->setModel(model);
    ui->RefRecordTV->resizeRowsToContents();
    ui->RefRecordTV->resizeColumnsToContents();

}

void MainWindow::showTags()
{
//        QStandardItemModel *model = new QStandardItemModel;
//    QStandardItem *item;

//    // Заголовки столбцов
//    QStringList horizontalHeader;
//    horizontalHeader.append("name");

//    model->setHorizontalHeaderLabels(horizontalHeader);

//    std::unique_ptr<Udb::ListTags> allUniqTags(urlDB_m->getUniqTags());

//    if(allUniqTags.get() != nullptr)
//    {
//        int i = 0;
//        for(auto it = allUniqTags->begin(); it != allUniqTags->end(); ++it)
//        {
//            item = new QStandardItem(it->getName());
//            model->setItem(i, 0, item);

//            i++;
//        }
//    }

//    ui->TRV->setModel(model);
}

void MainWindow::on_pushButton_2_clicked()
{
    UrlLockChecker newChecker("http://www.rutracker.org/");

    if(newChecker.isValid())
    {
        if(newChecker.isLock())
        {
            QMessageBox::information(this, "","rutracker.org в списке запрета!\n");
        }
        else
        {
            QMessageBox::information(this, "", "rutracker.org сайт не входит в список\n");
        }
    }
}


void MainWindow::createTreeView()
{
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());

    ui->TRV->setModel(model);
}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

