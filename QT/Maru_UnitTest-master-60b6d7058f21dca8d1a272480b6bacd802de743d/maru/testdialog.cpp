#include "testdialog.h"
#include "ui_testdialog.h"

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);

}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::on_pushButton_2_clicked()
{
    close();
    emit TestDialogClosed("TEST");
}
