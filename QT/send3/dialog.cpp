#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    dlgLineStr = ui->lineEdit->text();

    connect(this,SIGNAL(sendData(QString)),parent(),SLOT(recData(QString)));

    emit sendData(dlgLineStr);

}
