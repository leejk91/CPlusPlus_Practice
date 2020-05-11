#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "findclick.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->findbutton->setDefault(true);
    ui->findbutton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->label_2->setText("Hello World");
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void MainWindow::on_findbutton_clicked() // find find button
{
    QString text = ui->lineEdit->text();
    Qt::CaseSensitivity cs= ui->checkBox->isChecked() ? Qt::CaseSensitive
                                                      :Qt::CaseInsensitive;
    if (ui->checkBox_2->isChecked())
    {
        emit findPrevious(text, cs);
    }
        else
    {
            emit findNext(text,cs);
        }
    }

void MainWindow::on_Closebutton_clicked() // find close button
{
    close();
}

void MainWindow::on_lineEdit_textChanged()
{
    ui->findbutton->setEnabled(true);
}
