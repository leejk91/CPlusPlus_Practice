#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    timerId = startTimer(1000);


    // Qtimer style

    timer =new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(deleteitem()));
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event) // work per 1 sec
{
    QTime a = QTime::currentTime();
    QString b = a.toString(Qt::TextDate);
    ui->label->setText(b);
}

void MainWindow::on_pushButton_clicked() // add item in list widget
{
    QString itemname = ui->lineEdit->text();
    ui->listWidget->addItem(itemname);
}

void MainWindow::deleteitem() //listWidget delete item per 5 sec
{
    QTime a = QTime::currentTime();
    QString b = a.toString(Qt::TextDate);
    ui->label_2->setText(b);
    ui->listWidget->model()->removeRow(0);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_Enter : MainWindow::on_pushButton_clicked(); break;
    }
}
