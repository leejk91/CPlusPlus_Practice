#include "repairtestlcddialog.h"
#include "ui_repairtestlcddialog.h"

#include "Settings.h"
#include "SoundUtil.h"

RepairTestLcdDialog::RepairTestLcdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestLcdDialog)
{
    ui->setupUi(this);

    //proc = new QProcess();

    backlight.gpio_init_backlight();

    backlight.sys_backlight_get(&value);

    if(value == 1){
        ui->label_3->setText(QString("%1%2").arg(value).arg(QString("(min)")));
    }
    else if(value == 7){
        ui->label_3->setText(QString("%1%2").arg(value).arg(QString("(max)")));
    }
    else{
        ui->label_3->setText(QString("%1").arg(value,3));
    }
}

RepairTestLcdDialog::~RepairTestLcdDialog()
{
    delete ui;
}

void RepairTestLcdDialog::on_pushButton_3_clicked()
{
    close();
    emit RepairTestLcdDialogClosed();
}

void RepairTestLcdDialog::on_pushButton_clicked()
{
    //UP
    SoundUtil::play("sound/ok.wav");

    if( value == -1 ) return;

    if( value < 7 ){
        value += 1;
        backlight.sys_backlight_set(value);
        if(value == 1){
            ui->label_3->setText(QString("%1%2").arg(value).arg(QString("(min)")));
        }
        else if(value == 7){
            ui->label_3->setText(QString("%1%2").arg(value).arg(QString("(max)")));
        }
        else{
            ui->label_3->setText(QString("%1").arg(value,3));
        }
    }
}

void RepairTestLcdDialog::on_pushButton_2_clicked()
{
    //DOWN
    SoundUtil::play("sound/ok.wav");

    if( value == -1 ) return;

    if( value > 1 ){
        value -= 1;
        backlight.sys_backlight_set(value);
        if(value == 1){
            ui->label_3->setText(QString("%1%2").arg(value).arg(QString("(min)")));
        }
        else if(value == 7){
            ui->label_3->setText(QString("%1%2").arg(value).arg(QString("(max)")));
        }
        else{
            ui->label_3->setText(QString("%1").arg(value,3));
        }
    }
}
