#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QDebug>
#include "admin.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();
    Admin *admin;



private slots:
    void on_pushButton_clicked();

private:
    Ui::MainDialog *ui;
};

#endif // MAINDIALOG_H
