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
    //connect(this,SIGNAL(send_value(QString A,int B,int C)),this,recv_velue(QString A,int B,int C));
    //emit send_value(value);
    Dialog.exec();

    // Dia_log = new Dialog(this); 새창 띄우는 방법2
    // Dia_log->show(); 새창 띄우는 방법 방법2
}
