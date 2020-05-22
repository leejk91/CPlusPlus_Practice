#include "new.h"
#include "ui_new.h"
#include "pop.h"

New::New(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New)
{
    ui->setupUi(this);
}

New::~New()
{
    delete ui;
}

void New::on_pushButton_clicked()
{
    close();
    emit DialogClosed();
}

void New::on_pushButton_2_clicked()
{
    emit DialogRotate();
}


void New::on_pushButton_3_clicked()
{
    Pop *dlg = new Pop();

    connect(dlg,SIGNAL(digclose()), this, SLOT(on_Dialog_closed()));

    emit DialogClosed();

    ShowThirdDialog(dlg);
}

void New::ShowThirdDialog(QDialog *widget)
{
    thirdscene = new QGraphicsScene();
    thirdview = new QGraphicsView();

    //thirdproxy = thirdscene->addWidget(widget);
    thirdscene->addWidget(widget);

    thirdview->setScene(thirdscene);
    thirdview->setGeometry(geometry()); //  actual Display size
    thirdview->resize(600,800);


    thirdview->show();
}

void New::on_Dialog_closed()
{
    thirdview->close();
}
