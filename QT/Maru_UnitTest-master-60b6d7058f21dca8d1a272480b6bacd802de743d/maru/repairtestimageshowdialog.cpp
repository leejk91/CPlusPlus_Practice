#include "repairtestimageshowdialog.h"
#include "ui_repairtestimageshowdialog.h"

#include <QDir>
#include <QDebug>

RepairTestImageShowDialog::RepairTestImageShowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepairTestImageShowDialog)
{
    ui->setupUi(this);
    ui->label_2->setStyleSheet("border-style: none; background-image: url(./img/logo.jpg);");

    QDir directory("./img");
    images = directory.entryList(QStringList() << "*.jpg" << "*.png" << "*.gif",QDir::Files);
    length = images.length() - 1;

    index = 0;
    QString str = "border-style: none; background-image: url(./img/" + images.value(index++) + ");";
    ui->label_10->setStyleSheet(str);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer->start(3000);

    stimer = new QTimer;
    connect(stimer, SIGNAL(timeout()), this, SLOT(on_stimeout()));

    label = new QLabel(this);
    label->setStyleSheet("QLabel{font:bold 30px; color:cyan; background-color:#444444}");
    label->setText("※ 안녕하세요 UBCn 무인시스템 입니다 ※");
    label->adjustSize();
    label->setGeometry(0, 1170, label->width(), label->height());

    qDebug() << label->width() << label->height();
    qDebug() << label->geometry().left() << label->geometry().right();

    label->show();
}

RepairTestImageShowDialog::~RepairTestImageShowDialog()
{
    delete ui;
}

void RepairTestImageShowDialog::on_pushButton_2_clicked()
{
    timer->stop();
    stimer->stop();
    close();
    emit RepairTestImageShowDialogClosed();
}

void RepairTestImageShowDialog::on_timeout()
{
    QString str = "border-style: none; background-image: url(./img/" + images.value(index++) + ");";
    ui->label_10->setStyleSheet(str);
    if(index > length)
        index = 0;
}

void RepairTestImageShowDialog::on_stimeout()
{
    if(label->geometry().right() >= 0){
        QRect rect = label->geometry();
        label->setGeometry(rect.x()-20 , 1170, rect.width(), rect.height());
    }
    else {
        label->setGeometry(800,1170,label->width(), label->height());
    }
}

void RepairTestImageShowDialog::on_pushButton_clicked()
{
    stimer->start(50);
}

