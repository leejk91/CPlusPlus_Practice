#include "maindialog.h"
#include "ui_maindialog.h"

#define HEAD 4 // 표 항목 갯수

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{

    ui->setupUi(this);

}

MainDialog::~MainDialog()
{
    delete ui;
}

