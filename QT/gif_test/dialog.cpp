#include "dialog.h"
#include "ui_dialog.h"

static QString subview = nullptr;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    start_gif();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::start_gif()
{
    if(subview != nullptr) // false
    {
        return;
    }

    //ui->label->setText("AAAAAAAAAAA");
    QMovie *movie = new QMovie(":/new/prefix1/ad04.gif");

    ui->label->setMovie(movie);
    movie->start();

}
