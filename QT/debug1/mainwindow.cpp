#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Hello" << "Hello";
    qDebug("Hello");

    QString str1="Hell";

    int n=10;
    qDebug() << n;

    double const PI = 3.141592;
    qDebug() << PI;

    qDebug() << (n*PI) +n;
    qDebug() << ((n<11) && (n>0));
}

MainWindow::~MainWindow()
{
    delete ui;
}
