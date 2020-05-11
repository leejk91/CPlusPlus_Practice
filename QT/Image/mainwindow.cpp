#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->label->setStyleSheet("QLabel{font:bold 30px; color:cyan; background-color:#444444}");
    ui->label->setText("※ 안녕하세요 UBCn 무인시스템 입니다 ※");
    ui->label->adjustSize();
    ui->label->setGeometry(0, 100, ui->label->width(), ui->label->height());

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer->start(3000);

    stimer = new QTimer;
    connect(stimer, SIGNAL(timeout()), this, SLOT(on_stimeout()));





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    stimer->start(50);
}

void MainWindow::on_pushButton_2_clicked()
{
    timer->stop();
    stimer->stop();
}

void MainWindow::on_timeout()
{

}
void MainWindow::on_stimeout()
{
    if(ui->label->geometry().right() >= 0){
        QRect rect = ui->label->geometry();
        ui->label->setGeometry(rect.x()-5 , 100, rect.width(), rect.height());
    }
    else {
        ui->label->setGeometry(800,100,ui->label->width(), ui->label->height());
    }

}
