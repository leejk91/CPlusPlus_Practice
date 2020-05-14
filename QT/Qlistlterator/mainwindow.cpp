#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include <QDebug>
#include <QListIterator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> List;

    for(int i=0; i<10; i++) List.append(i);

    QListIterator<int> iter(List);

    qDebug() <<"Forward";

    while(iter.hasNext())
    {
        qDebug() << iter.next();
    }

    qDebug() <<"Backward";
    while(iter.hasPrevious())
    {
        qDebug() << iter.previous();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
