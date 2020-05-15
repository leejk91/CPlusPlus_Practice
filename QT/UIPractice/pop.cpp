#include "pop.h"
#include "ui_pop.h"

Pop::Pop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pop)
{
    ui->setupUi(this);
}

Pop::~Pop()
{
    delete ui;
}
