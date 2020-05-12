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

    additem("00A1","TEST",5,1000,800);
    additem("00A2","TEST2",3,2000,1500);
}

Import::~Import()
{
    delete ui;
}

void Import::on_pushButton_5_clicked()
{
    this->close();
}

void Import::additem(QString Code,QString Name,int date,int value,int sale_value) // 삽입
{

    qDebug() << Code << Name << date << value << sale_value;

    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);


    treeItem->setText(0, Code);
    treeItem->setText(1, Name);
    treeItem->setText(2, QString::number(date));
    treeItem->setText(3, QString::number(value));
    treeItem->setText(4, QString::number(sale_value));

}
