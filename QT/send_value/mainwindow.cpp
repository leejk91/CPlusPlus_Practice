#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int value = 100;

    dia_log = new Dialog(this);

    connect(this,SIGNAL(send_value(int)),dia_log,SLOT(recv_value(int)));

    emit send_value(value);

    dia_log ->exec();

    qDebug() << dia_log->result_value;

}
