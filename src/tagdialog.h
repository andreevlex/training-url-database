#ifndef ADDTAGDIALOG_H
#define ADDTAGDIALOG_H

#include <QDialog>

namespace Ui {
class TagDialog;
}

class TagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagDialog(QWidget *parent = 0);
    ~TagDialog();

    void setAddMode(const bool addmode);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::TagDialog *ui;
    bool addmode_m{true};
};

#endif // ADDTAGDIALOG_H
