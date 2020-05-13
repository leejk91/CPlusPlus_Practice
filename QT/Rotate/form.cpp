#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    mainscene2 = new QGraphicsScene(); //
    mainview2 = new QGraphicsView(); //
    mainview2->setScene(mainscene2);

    mainproxy2 = mainscene2->addWidget(this); // ???

}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    mainview2->rotate(90);

}
