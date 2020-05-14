#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int i =1000;
    QString test =  QString("%1").arg(i,8,10,QLatin1Char('0'));
    qDebug() << test;\

    QGraphicsProxyWidget *proxy = scene->addWidget(this);

    view->setScene(scene);
    view->show();
    view->rotate(90);
}

MainWindow::~MainWindow()
{
    delete ui;
}
