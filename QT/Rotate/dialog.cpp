#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    mainscene2 = new QGraphicsScene(); //
    mainview2 = new QGraphicsView(); //
    mainview2->setScene(mainscene2);

    mainproxy2 = mainscene2->addWidget(this); // ???

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    mainview2->rotate(90);
}
