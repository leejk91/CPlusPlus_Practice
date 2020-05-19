#include "maindialog.h"
#include "ui_maindialog.h"


MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    this->setStyleSheet("QFrame { background-color : white; border: 1px; border-radius: 30px;}");
    // 프레임 스타일 시트 설정

    QString store_name;
    store_name = "UBCN 1호점";
    ui->store_name->setText(store_name); // 점포 이름 설정
    ui->store_name->setStyleSheet("#store_name{color:blue}");


}

MainDialog::~MainDialog()
{
    delete ui;
}
