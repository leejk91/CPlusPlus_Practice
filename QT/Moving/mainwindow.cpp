#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
    anim = new QPropertyAnimation(ui->label,"geometry");
    anim->setDuration(10000);
    anim->setStartValue(ui->label->geometry());
    anim->setEndValue(QRect(50,50,50,10));
    anim->start();
    */

    process = new QProcess(this);
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(process_out()));


    process->start();
    process->waitForStarted();
    QApplication::processEvents();


}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void move()
{

    QRect rect=this->label->geometry();
    ui->label->setGeometry(rect.x()-50 , 100, rect.width(), rect.height());

    return move();
}
*/

/*void MainWindow::process_out()
{
    QRect rect=ui->label->geometry();
    ui->label->setGeometry(rect.x()-50 , 100, rect.width(), rect.height());
}*/
