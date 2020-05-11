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
    //connect(ui->pushButton,SIGNAL(clicked),this,SLOT(ButtonClick()));

    dlg = new Dialog(this);
    dlg->setModal(true); // 모델형 대화상자로 설정
    dlg->show();


}

void MainWindow::recData(QString str)
{
      ui->lineEdit->setText(str);
}
