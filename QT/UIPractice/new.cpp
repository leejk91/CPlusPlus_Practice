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

    //connect(dlg,SIGNAL(DialogClosed()), this, SLOT(on_subDialog_closed()));
    emit DialogClosed();

    ShowThirdDialog(dlg);
}

void New::ShowThirdDialog(QDialog *widget)
{
    thirdscene = new QGraphicsScene();
    thirdview = new QGraphicsView();

    thirdproxy = thirdscene->addWidget(widget);

    thirdview->setScene(thirdscene);
    thirdview->setGeometry(geometry()); //  actual Display size
    thirdview->resize(600,800);


    thirdview->show();
}
