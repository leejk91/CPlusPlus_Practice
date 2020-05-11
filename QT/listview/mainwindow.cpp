#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);

    QStringList List;
    List << "Clair de Lune "<< "Reverie";

    model->setStringList(List);

    ui->listView->setModel(model);
    ui->comboBox->setModel(model);

    //ui->listView->setEditTriggers(QAbstractItemView::AnyKeyPressed
                                 // | QAbstractItemView::DoubleClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int row = model ->rowCount();
    model->insertRows(row,1);
    QModelIndex index = model->index(row);

    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
}

void MainWindow::on_pushButton_2_clicked()
{
    int row = ui->listView->currentIndex().row();

    model->insertRows(row,1);

    QModelIndex index = model->index(row);

    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
}

void MainWindow::on_pushButton_3_clicked()
{
    model->removeRows(ui->listView->currentIndex().row(),1);
}
