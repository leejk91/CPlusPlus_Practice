#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

#if 1
    ui->lineEdit_2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    ui->lineEdit_3->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
    ui->lineEdit_4->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ui->lineEdit_2->setFixedHeight(25);
    ui->lineEdit_3->setFixedHeight(25);
    ui->lineEdit_4->setFixedHeight(25);
    //ui->lineEdit_2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //ui->lineEdit_2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //ui->lineEdit_2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ui->label_2->setFixedWidth(100);
    ui->label_3->setFixedWidth(100);
    ui->label_4->setFixedWidth(100);
    ui->lineEdit_2->setFixedWidth(200);
    ui->lineEdit_3->setFixedWidth(200);
    ui->lineEdit_4->setFixedWidth(200);
#endif


}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked() // 이전
{

}

void Dialog::on_pushButton_2_clicked() // 다음
{

}

void Dialog::on_pushButton_3_clicked() // 확인
{

}

void Dialog::on_pushButton_4_clicked() // 취소
{
    close();
}
