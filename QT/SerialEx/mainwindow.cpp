#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int buf[4] = {0x25,0x62,0x3F,0x52};

    qDebug <<

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::checksum(unsigned char *data, int len)
{

   int total=0, sum=0;
   for(int x=0;x<len;x++,data++){
       sum += *data;
   }
   total = sum;
   total = total&0xFF;
   total = ~total+1;
   total = total+sum;
   total = total&0xff;
   return total;

}
