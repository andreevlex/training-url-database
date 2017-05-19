#ifndef ADDREFDIALOG_H
#define ADDREFDIALOG_H

#include <QDialog>

namespace Ui {
class RefDialog;
}

class RefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RefDialog(QWidget *parent = 0);
    ~RefDialog();

    void setAddMode(const bool addmode);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::RefDialog *ui;
    bool addmode_m{true};
};

#endif // ADDREFDIALOG_H
