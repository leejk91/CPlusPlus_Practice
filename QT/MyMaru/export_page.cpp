#include "export_page.h"
#include "ui_export_page.h"

export_page::export_page(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::export_page)
{
    ui->setupUi(this);
}

export_page::~export_page()
{
    delete ui;
}

void export_page::on_pushButton_2_clicked()
{
    this -> close();
}
