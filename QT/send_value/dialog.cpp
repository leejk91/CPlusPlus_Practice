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
    value2 = ui->lineEdit->text().toInt();
    result_value= value1+value2;

    qDebug() << value1 << " " << value2;

    this->close();
}
