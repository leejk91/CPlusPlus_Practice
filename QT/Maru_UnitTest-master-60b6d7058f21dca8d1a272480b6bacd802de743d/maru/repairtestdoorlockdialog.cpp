#include "repairtestdoorlockdialog.h"
#include "ui_repairtestdoorlockdialog.h"
#include "lib_koces.h"

RepairTestDoorlockDialog::RepairTestDoorlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestDoorlockDialog)
{
    ui->setupUi(this);

    status_timer = new QTimer;
    connect(status_timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    status_timer->start(500);
    //Debug_Open();
}

RepairTestDoorlockDialog::~RepairTestDoorlockDialog()
{
    //Debug_Close();
    delete ui;
}

void RepairTestDoorlockDialog::on_pushButton_3_clicked()
{
    close();
    emit RepairTestDoorlockDialogClosed();
}

void RepairTestDoorlockDialog::on_pushButton_clicked()
{
    //Dbg_Printf("\n\rButton Pressed\n\r");
    MaruDoorlock::open();
}

void RepairTestDoorlockDialog::on_timeout()
{
    if(MaruDoorlock::status()){
        //ui->label_2->setStyleSheet("QLabel{font:bold 50px; color:darkMagenta;}");
        ui->label_2->setStyleSheet("QLabel{font:bold 50px; color:magenta;}");
        ui->label_2->setText("Closed");
    }
    else{
        //ui->label_2->setStyleSheet("QLabel{font:bold 50px; color:darkGreen;}");
        ui->label_2->setStyleSheet("QLabel{font:bold 50px; color:green;}");
        ui->label_2->setText("Opened");
    }
}
