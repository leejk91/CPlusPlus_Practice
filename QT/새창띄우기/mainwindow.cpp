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
    Dialog Dialog;
    Dialog.setModal(true);
    Dialog.exec();

    // Dia_log = new Dialog(this); 방법2
    // Dia_log->show(); 방법2
}
