#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>


SortDialog::SortDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SortDialog)
{
    ui->setupUi(this);

    ui->groupBox_2->hide();
    ui->groupBox_3->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setColumRange('A','Z');
}

SortDialog::~SortDialog()
{
    delete ui;
}

void SortDialog::setColumRange(QChar first, QChar last)
{
    ui->comboBox_2->clear();
    ui->comboBox_4->clear();
    ui->comboBox_6->clear();

    ui->comboBox_4->addItem(tr("None"));
    ui->comboBox_6->addItem(tr("None"));
    ui->comboBox_2->setMinimumSize(ui->comboBox_4->sizeHint());

    QChar ch = first;
    while(ch<= last){
        ui->comboBox_2->addItem(QString(ch));
        ui->comboBox_4->addItem(QString(ch));
        ui->comboBox_6->addItem(QString(ch));
        ch = ch.unicode() +1;
    }

  }

void SortDialog::on_cancelButton_clicked()
{
    close();
}
