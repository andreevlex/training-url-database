#include "refdialog.h"
#include "ui_refdialog.h"
#include "factorymethod.h"

RefDialog::RefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefDialog)
{
    ui->setupUi(this);
}



RefDialog::~RefDialog()
{
    delete ui;
}

void RefDialog::setAddMode(const bool addmode)
{
    addmode_m = addmode;
}

void RefDialog::on_buttonBox_rejected()
{
    this->close();
}

void RefDialog::on_buttonBox_accepted()
{
//    RefRecords RefRecords_m;
//    RefRecord* newRef = RefRecords.create();
//    newRef->setName(ui->editName->text());
//    newRef->setUrl(ui->plainUrl->toPlainText());

//    if( newRef->save() )
//    {
//        this->close();
//    }
}
