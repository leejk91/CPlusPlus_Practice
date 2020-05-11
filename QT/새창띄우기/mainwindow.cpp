#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#define HEAD 3 // 표 항목 갯수

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(HEAD); // 항목 개수 설정

    ui->treeWidget->setHeaderLabels(QStringList() << "제품" << "가격" << "상태");

    recv_value("TEST",100,20);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //int value = 100;

    Dialog Dialog;
    Dialog.setModal(true);
    Dialog.exec();

    // Dia_log = new Dialog(this); 새창 띄우는 방법2
    // Dia_log->show(); 새창 띄우는 방법 방법2
    qDebug() << "입력창 오픈";

}

void MainWindow::recv_value(QString A,int B,int C)
{

    qDebug() << "A: "<< A << "B: " << B << "C: " << C;

    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    ui->label->setText(A);


    treeItem->setText(0, A);

    QString value;
    value.setNum(B);
    treeItem->setText(1, value);

    QString value2;
    value2.setNum(C);
    treeItem->setText(2, value2);

}
