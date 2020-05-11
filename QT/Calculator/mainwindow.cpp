#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QList>
#include <QtGui>

QString vect(100);
QString vect2(100);
int A=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vect = "";
    vect2 = "";

    ui->treeWidget->setColumnCount(4);

    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);


    timer =new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(deleteitem()));
    timer->start(5000);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTreeRoot(QString A, QString oper, QString B, QString C)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    treeItem->setText(0,A);
    treeItem->setText(1,oper);
    treeItem->setText(2,B);
    treeItem->setText(3,C);

}

void MainWindow::on_push_1_clicked()
{
    if(A==0)
    {
   vect.append("1");
    ui->label->setText(vect);
    }
    else {
        vect2.append("1");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_2_clicked()
{
    if(A==0)
    {
    vect.append("2");
     ui->label->setText(vect);
    }
    else {
        vect2.append("2");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_3_clicked()
{
    if(A==0)
    {
    vect.append("3");
     ui->label->setText(vect);
    }
    else {
        vect2.append("3");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_4_clicked()
{
    if(A==0)
    {
    vect.append("4");
     ui->label->setText(vect);
    }
    else {
        vect2.append("4");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_5_clicked()
{
    if(A==0)
    {
    vect.append("5");
     ui->label->setText(vect);
    }
    else {
        vect2.append("5");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_6_clicked()
{
    if(A==0)
    {
    vect.append("6");
     ui->label->setText(vect);
    }
    else {
        vect2.append("6");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_7_clicked()
{
    if(A==0)
    {
    vect.append("7");
     ui->label->setText(vect);
    }
    else {
    vect2.append("7");
    ui->label->setText(vect2);
    }
}
void MainWindow::on_push_8_clicked()
{
    if(A==0)
    {
    vect.append("8");
    ui->label->setText(vect);
    }
    else {
        vect2.append("8");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_9_clicked()
{
    if(A==0)
    {
    vect.append("9");
     ui->label->setText(vect);
    }
    else {
        vect2.append("9");
        ui->label->setText(vect2);
    }
}
void MainWindow::on_push_0_clicked()
{
    if(A==0)
    {
    vect.append("0");
     ui->label->setText(vect);
    }
    else {
        vect2.append("0");
        ui->label->setText(vect2);
    }
}

void MainWindow::on_push_C_clicked()
{
    vect = "";
    vect2 = "";
    A=0;
    ui->label->setText(vect);
}

void MainWindow::on_push_oper1_clicked()
{
    ui->label_vect1->setText(vect);
    ui->label_oper->setText("+");

    if(A<=1)
    {
        A=2;
    }
    else {
     on_push_finish_clicked();
    }
}

void MainWindow::on_push_oper2_clicked()
{

    ui->label_vect1->setText(vect);
    ui->label_oper->setText("-");

    if(A<=1)
    {
        A=3;
    }
    else {
     on_push_finish_clicked();
    }
}

void MainWindow::on_push_oper3_clicked()
{
    ui->label_vect1->setText(vect);
    ui->label_oper->setText("*");

    if(A<=1)
    {
        ui->label_vect1->setText(vect);
        A=4;
    }
    else {
     on_push_finish_clicked();
    }
}
void MainWindow::on_push_oper4_clicked()
{
    ui->label_vect1->setText(vect);
    ui->label_oper->setText("/");

    if(A<=1)
    {
        ui->label_vect1->setText(vect);
        A=5;
    }
    else {
     on_push_finish_clicked();
    }
}

void MainWindow::on_push_finish_clicked()
{
    ui->label_vect2->setText(vect2);

    if(A==2){
        int A = vect.toInt();
        int B = vect2.toInt();
        int C = A+B;



        QString s = QString::number(C);
        addTreeRoot(vect,"+", vect2,s);
        ui->label->setText(s);

        vect=s;
        vect2="";

        ui->label->setText(s);
            }

    if(A==3){
        int A = vect.toInt();
        int B = vect2.toInt();
        int C = A-B;

        QString s = QString::number(C);
        addTreeRoot(vect,"-", vect2,s);
        ui->label->setText(s);

        vect=s;
        vect2="";

        ui->label->setText(s);
            }

    if(A==4){
        int A = vect.toInt();
        int B = vect2.toInt();
        int C = A*B;

        QString s = QString::number(C);
        addTreeRoot(vect,"*", vect2,s);

        vect=s;
        vect2="";

        ui->label->setText(s);
            }

    if(A==5){
        int A = vect.toInt();
        int B = vect2.toInt();
        int C = A/B;

        QString s = QString::number(C);
        addTreeRoot(vect,"/", vect2,s);

        vect=s;
        vect2="";

        ui->label->setText(s);

            }

      A=1;


}

void MainWindow::on_push_back_clicked()
{
    if(A==0)
    {
    int K= vect.length();
    vect.remove(K-1,1);
     ui->label->setText(vect);
    }
    else {
        int K= vect2.length();
        vect2.remove(K-1,1);
        ui->label->setText(vect2);
        }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_0 : on_push_0_clicked(); break;
    case Qt::Key_1 : on_push_1_clicked(); break;
    case Qt::Key_2 : on_push_2_clicked(); break;
    case Qt::Key_3 : on_push_3_clicked(); break;
    case Qt::Key_4 : on_push_4_clicked(); break;
    case Qt::Key_5 : on_push_5_clicked(); break;
    case Qt::Key_6 : on_push_6_clicked(); break;
    case Qt::Key_7 : on_push_7_clicked(); break;
    case Qt::Key_8 : on_push_8_clicked(); break;
    case Qt::Key_9 : on_push_9_clicked(); break;
    case Qt::Key_Plus : on_push_oper1_clicked(); break;
    case Qt::Key_Minus : on_push_oper2_clicked(); break;
    case Qt::Key_Asterisk : on_push_oper3_clicked(); break;
    case Qt::Key_Slash : on_push_oper4_clicked(); break;
    case Qt::Key_Enter : on_push_finish_clicked(); break;
    case Qt::Key_Backspace : on_push_back_clicked(); break;



    }
}

void MainWindow::deleteitem() //listWidget delete item per 5 sec
{
    ui->treeWidget->takeTopLevelItem(0);
}


