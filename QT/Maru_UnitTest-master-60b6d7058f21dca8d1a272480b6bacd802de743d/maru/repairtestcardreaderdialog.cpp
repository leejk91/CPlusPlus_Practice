#include "repairtestcardreaderdialog.h"
#include "ui_repairtestcardreaderdialog.h"

//#include "maruserial.h"

RepairTestCardReaderDialog::RepairTestCardReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestCardReaderDialog)
{
    //MaruSerial &maru = MaruSerial::instance();
}

RepairTestCardReaderDialog::~RepairTestCardReaderDialog()
{
    delete ui;
}

void RepairTestCardReaderDialog::on_pushButton_2_clicked()
{
    close();
    emit RepairTestCardReaderDialogClosed();
}


void RepairTestCardReaderDialog::on_pushButton_clicked()
{
    //MaruSerial::sendMessage(0x00, nullptr, 0x00);
}
