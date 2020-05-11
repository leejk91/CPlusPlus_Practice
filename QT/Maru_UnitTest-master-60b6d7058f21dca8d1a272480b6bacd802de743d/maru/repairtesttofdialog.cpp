#include "repairtesttofdialog.h"
#include "ui_repairtesttofdialog.h"
#include "tof.h"

#include <QDebug>

RepairTestTOFDialog::RepairTestTOFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestTOFDialog)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(on_Timer()));

    //timer.start(2000);

    int i;

    i = tofInit(3, 0x29, 1); // set long range mode (up to 2m)

    if (i != 1)
    {
        //return -1; // problem - quit
        qDebug() << "VL53L0X device open fail.\n";

    }
    else{
        qDebug() << "VL53L0X device successfully opened.\n";

        i = tofGetModel(&model, &revision);

        if(i != 1){
            qDebug() << "tofGetModel Fail.\n";
        }
        else{
            qDebug() << "Model ID - " << model;
            qDebug() << "Revision ID - " << revision;

            //iDistance = tofReadDistance();
            //qDebug() << iDistance;

        }
    }




#if 0
    for (i=0; i<1200; i++) // read values 20 times a second for 1 minute
        {
            iDistance = tofReadDistance();
            if (iDistance < 4096) // valid range?
                printf("Distance = %dmm\n", iDistance);
            Sleep(50000); // 50ms
        }
#endif


}

RepairTestTOFDialog::~RepairTestTOFDialog()
{
    delete ui;
}

void RepairTestTOFDialog::on_pushButton_3_clicked()
{
    timer.stop();

    close();
    emit RepairTestTOFDialogClosed();
}

void RepairTestTOFDialog::on_Timer()
{

    //iDistance = tofReadDistance();
    //qDebug() << iDistance;

    iDistance = tofReadDistance();
    if(iDistance < 4096)
    {
        ui->label_2->setText(QString::number(iDistance));
    }
}

void RepairTestTOFDialog::on_pushButton_clicked()
{
    // start
    timer.start(200);
}
