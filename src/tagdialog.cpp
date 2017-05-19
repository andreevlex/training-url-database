#include <memory>

#include "tagdialog.h"
#include "ui_tagdialog.h"
#include "factorymethod.h"
#include "tag.h"

TagDialog::TagDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagDialog)
{
    ui->setupUi(this);
}

TagDialog::~TagDialog()
{
    delete ui;
}

void TagDialog::setAddMode(const bool addmode)
{
    addmode_m = addmode;
}

void TagDialog::on_buttonBox_accepted()
{
    Tags Tags_m;
    std::unique_ptr<Tag> newTag(dynamic_cast<Tag*>(Tags_m.create()));

    newTag->setName(ui->editTagName->text());

    if( newTag->save() )
    {
        this->close();
    }
}

void TagDialog::on_buttonBox_rejected()
{
    this->close();
}
