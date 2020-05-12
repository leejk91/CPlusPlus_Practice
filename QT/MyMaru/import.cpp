#include "import.h"
#include "ui_import.h"

Import::Import(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Import)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(5); // 항목 개수 설정

    ui->treeWidget->setHeaderLabels(QStringList() << "상품코드" << "상품명" << "정상가 기간" <<"상품 가격"<<"할인 가격"); // 항목 이름 설정

    ui->label->setStyleSheet("QLabel{font: 12px; color:white}");
    ui->label_2->setStyleSheet("QLabel{font: 12px; color:white}");
    ui->label_3->setStyleSheet("QLabel{font: 12px; color:white}");
    ui->label_12->setStyleSheet("QLabel{font: 12px; color:white}");
    ui->label_10->setStyleSheet("QLabel{font: 12px; color:white}");
    ui->label_11->setStyleSheet("QLabel{font: 12px; color:white}");


}

Import::~Import()
{
    delete ui;
}

void Import::on_pushButton_5_clicked()
{
    this->close();
}
