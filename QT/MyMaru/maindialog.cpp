#include "maindialog.h"
#include "ui_maindialog.h"

#define HEAD 4 // 표 항목 갯수

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{

    ui->setupUi(this);

    ui->label->setStyleSheet("QLabel{font:bold 28px; color:white; background-color:#444444}");
    ui->label_2->setStyleSheet("QLabel{font:bold 28px; color:white; background-color:#444444}");


}

MainDialog::~MainDialog()
{
    delete ui;
}


void MainDialog::on_pushButton_clicked()
{
    admin = new Admin(this);
    admin->show();

    qDebug() << "관리자 로그인 입장";
}
