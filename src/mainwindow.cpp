#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "urldatabase.h"
#include "config.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include <memory>
#include "urllockchecker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dbMain(new UrlDatabase())
{
    ui->setupUi(this);
    createMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dbMain;
}

void MainWindow::on_pushButton_clicked()
{
    if( dbMain->openDB() )
    {

    size_t countt = dbMain->tagsCount();
    QString sCount = QStringLiteral("Количество уникальных тэгов = %1\n").arg(countt);
    ui->textEdit1->insertPlainText(sCount);

    size_t countr = dbMain->refsCount();
    QString sCount2 = QStringLiteral("Количество ссылок в базе = %1\n").arg(countr);
    ui->textEdit1->insertPlainText(sCount2);

    bool refFind = false;
    long long int four = 4;
    const RefRecord& refg = dbMain->getRef(four, refFind);
    if(refFind)
    {
        QString sRefInfo = QStringLiteral("id=%1, "
                                          "name=%2, "
                                          "url=%3, "
                                          "date=%, "
                                          "fav=%4\n").arg(refg.getID())
                                                     .arg(refg.getName())
                                                     .arg(refg.getUrl())
                                                     .arg(refg.getDateCreate().toString())
                                                     .arg(refg.getFavorite());
        ui->textEdit1->insertPlainText(sRefInfo);
    }
    // add
    RefRecord newRec;
    newRec.setName("Вторая ссылка добавлена");
    newRec.setUrl("www.isa.ru/");
    newRec.setDateCreate(QDateTime::currentDateTimeUtc());

    Tag newTag;
    newTag.setName("tag2");
    newRec.addTag(newTag);

    dbMain->addRef(newRec);

    dbMain->closeDB();
    }
    else
    {
        ui->textEdit1->insertPlainText("что-то пошло не так");
    }

}

void MainWindow::createMenu()
{
QMenu * mnFile = new QMenu("Файл"); // создаём меню Файл

    QMenu * mnEdit = new QMenu("Правка"); // создаём меню Edit
    QMenu * mnService = new QMenu("Сервис"); // Меню Сервис
    // ----------- здесь добавляем пункты меню и подключаем его к слоту----
    QAction *showRefsAction = new QAction("Показать ссылки", mnService);
    connect(showRefsAction, SIGNAL(triggered()), this, SLOT(showRefs()));
    mnService->addAction(showRefsAction);

    QAction *showFavoriteRefsAction = new QAction("Показать избранное", mnService);
    connect(showFavoriteRefsAction, SIGNAL(triggered()), this, SLOT(showFavoriteRefs()));
    mnService->addAction(showFavoriteRefsAction);

    QAction *showTagsAction = new QAction("Показать тэги", mnService);
    connect(showTagsAction, SIGNAL(triggered()), this, SLOT(showTags()));
    mnService->addAction(showTagsAction);

    QMenu * mnHelp = new QMenu("Помощь"); // Меню помощь
    ui->menuBar->addMenu(mnFile);
    ui->menuBar->addMenu(mnEdit);
    ui->menuBar->addMenu(mnService);
    ui->menuBar->addMenu(mnHelp);
}

QString MainWindow::createStringTags(const RefRecord& currentRef)
{
    QString allTags("");

    for(auto ittags = currentRef.firstTag(); ittags != currentRef.lastTag(); ++ittags)
    {
        allTags += ittags->getName() + " ";
    }
    return allTags;
}

void MainWindow::addLineToRefs(QStandardItemModel* model, const unsigned int lineNum, const RefRecord& currentRef)
{
    QStandardItem *item;

    item = new QStandardItem(QStringLiteral("%1").arg(currentRef.getID()));
    model->setItem(lineNum, 0, item);

    item = new QStandardItem(currentRef.getName());
    model->setItem(lineNum, 1, item);

    item = new QStandardItem(currentRef.getUrl());
    model->setItem(lineNum, 2, item);

    item = new QStandardItem(createStringTags(currentRef));
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

    if ( !dbMain->openDB() )
    {
        return;
    }

    QStandardItemModel *model = new QStandardItemModel;

    // Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("id");
    horizontalHeader.append("name");
    horizontalHeader.append("url");
    horizontalHeader.append("tags");
    horizontalHeader.append("locked");


    model->setHorizontalHeaderLabels(horizontalHeader);

    std::unique_ptr<Udb::ListRefs> allRefs(dbMain->getRefs());

    if(allRefs.get() != nullptr)
    {
        int i = 0;
        for(auto it = allRefs->begin(); it != allRefs->end(); ++it)
        {
            addLineToRefs(model, i, *it);
            i++;
        }
    }

    ui->tableView1->setModel(model);
    ui->tableView1->resizeRowsToContents();
    ui->tableView1->resizeColumnsToContents();

    dbMain->closeDB();
}

void MainWindow::showFavoriteRefs()
{

    if ( !dbMain->openDB() )
    {
        return;
    }

    QStandardItemModel *model = new QStandardItemModel;

    // Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("id");
    horizontalHeader.append("name");
    horizontalHeader.append("url");
    horizontalHeader.append("tags");
    horizontalHeader.append("locked");

    model->setHorizontalHeaderLabels(horizontalHeader);

    std::unique_ptr<Udb::ListRefs> allFavoriteRefs(dbMain->getFavoriteRefs());

    if(allFavoriteRefs.get() != nullptr)
    {
        int i = 0;
        for(auto it = allFavoriteRefs->begin(); it != allFavoriteRefs->end(); ++it)
        {
            addLineToRefs(model, i, *it);

            i++;
        }
    }

    ui->tableView1->setModel(model);
    ui->tableView1->resizeRowsToContents();
    ui->tableView1->resizeColumnsToContents();

    dbMain->closeDB();
}

void MainWindow::showTags()
{
    if ( !dbMain->openDB() )
    {
        return;
    }

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

    // Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("name");

    model->setHorizontalHeaderLabels(horizontalHeader);

    std::unique_ptr<Udb::ListTags> allUniqTags(dbMain->getUniqTags());

    if(allUniqTags.get() != nullptr)
    {
        int i = 0;
        for(auto it = allUniqTags->begin(); it != allUniqTags->end(); ++it)
        {
            item = new QStandardItem(it->getName());
            model->setItem(i, 0, item);

            i++;
        }
    }

    ui->tableView1_2->setModel(model);
    ui->tableView1_2->resizeRowsToContents();
    ui->tableView1_2->resizeColumnsToContents();

    dbMain->closeDB();
}

void MainWindow::on_pushButton_2_clicked()
{
    UrlLockChecker newChecker("http://www.rutracker.org/");

    if(newChecker.isValid())
    {
        if(newChecker.isLock())
        {
            ui->textEdit_2->insertPlainText("rutracker.org в списке запрета!\n");
        }
        else
        {
            ui->textEdit_2->insertPlainText("rutracker.org сайт не входит в список\n");
        }
    }
}

