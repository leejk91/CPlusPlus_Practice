#include "admin.h"
#include "ui_admin.h"

QString password(10);

Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_pushButton_2_clicked()
{
    password.append("1");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_3_clicked()
{
    password.append("2");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_4_clicked()
{
    password.append("3");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_5_clicked()
{
    password.append("4");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_6_clicked()
{
    password.append("5");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_7_clicked()
{
    password.append("6");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_8_clicked()
{
    password.append("7");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_9_clicked()
{
    password.append("8");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_10_clicked()
{
    password.append("9");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_11_clicked()
{
    password.append("0");
    ui->lineEdit->setText(password);
}

void Admin::on_pushButton_12_clicked()
{
    int K= password.length();
    password.remove(K-1,1);
     ui->lineEdit->setText(password);
}

void Admin::on_pushButton_13_clicked()
{

}
