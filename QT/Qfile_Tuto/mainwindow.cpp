#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::write(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << " could not open file for writing";
        return;
    }

    QTextStream out(&file);
    out << "QFile Tutorial";
    file.flush();
    file.close();


}

void MainWindow::read(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " could not open the file for reading";
        return;
    }

    QTextStream in(&file);
    QString mytext = in.readAll();
    qDebug() << mytext;

    file.close();
}
