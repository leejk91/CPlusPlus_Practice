#include "repairtestkeybuttondialog.h"
#include "ui_repairtestkeybuttondialog.h"
#include "lib_util.h"

#include <QDebug>
#include <QDateTime>

RepairTestKeyButtonDialog::RepairTestKeyButtonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestKeyButtonDialog)
{
    ui->setupUi(this);

#if 0
    unsigned char TimeStamp[20];
    GetTimeString(NULL, (char *)TimeStamp);
    qDebug() << QByteArray::fromRawData((char *)TimeStamp, sizeof (TimeStamp));
#endif

#if 0
    QByteArray ba = QDateTime::currentDateTime().toString("yyMMddhhmmss").toUtf8();
    qDebug() << ba;

    unsigned char temp[12], temp2[6];
    memcpy((char *)temp, ba.constData(), 12);
    for (int i = 0 ; i < 12; i++) {
        qDebug() << QString::number(temp[i],16);
    }

    qDebug() << "test";

    Char2BCD(temp, temp2, 12);
    for (int i = 0 ; i < 6; i++) {
        qDebug() << QString::number(temp2[i],16);
    }
#endif
}

RepairTestKeyButtonDialog::~RepairTestKeyButtonDialog()
{
    delete ui;
}

void RepairTestKeyButtonDialog::on_pushButton_clicked()
{
    close();
    emit RepairTestKeyButtonDialogClosed();
}

void RepairTestKeyButtonDialog::keyPressEvent(QKeyEvent *event)
{
    //if(event->isAutoRepeat() == true)
    //        return;
        switch(event->key()){
            case Qt::Key_Escape:
                ui->label_5->setText("Pressed");
                ui->label_5->setStyleSheet("QLabel { background-color : #00FF00; font: bold 30px;qproperty-alignment: AlignCenter;}");
                //qDebug() << "Key Press Escape";
                break;
            case Qt::Key_Backspace:
                ui->label_7->setText("Pressed");
                ui->label_7->setStyleSheet("QLabel { background-color : #00FF00; font: bold 30px;qproperty-alignment: AlignCenter;}");
               //qDebug() << "Key Press Backspace";
                break;
            case Qt::Key_Enter:
                ui->label_9->setText("Pressed");
                ui->label_9->setStyleSheet("QLabel { background-color : #00FF00; font: bold 30px;qproperty-alignment: AlignCenter;}");
                //qDebug() << "Key Press Enter";
                break;
        }
}

void RepairTestKeyButtonDialog::keyReleaseEvent(QKeyEvent *event)
{
    //if(event->isAutoRepeat() == true)
    //        return;
        switch(event->key()){
            case Qt::Key_Escape:
                ui->label_5->setText("Released");
                ui->label_5->setStyleSheet("QLabel { background-color : #0066FF; font: bold 30px;qproperty-alignment: AlignCenter;}");
                //qDebug() << "Key Release Escape";
                break;
            case Qt::Key_Backspace:
                ui->label_7->setText("Released");
                ui->label_7->setStyleSheet("QLabel { background-color : #0066FF; font: bold 30px;qproperty-alignment: AlignCenter;}");
                //qDebug() << "Key Release Backspace";
                break;
            case Qt::Key_Enter:
                ui->label_9->setText("Released");
                ui->label_9->setStyleSheet("QLabel { background-color : #0066FF; font: bold 30px;qproperty-alignment: AlignCenter;}");
                //qDebug() << "Key Release Enter";
                break;
        }
}

