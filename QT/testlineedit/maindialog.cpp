#include "maindialog.h"
#include "ui_maindialog.h"


MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(3); // 항목 개수 설정

    ui->treeWidget->setHeaderLabels(QStringList() << "컴퓨터 이" << "IP" << "MAC"); // 항목 이름 설정

}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_pushButton_clicked() // 추
{
    Dialog *config = new Dialog();

    config->show();
}
