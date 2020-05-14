#include "dialog.h"
#include "ui_dialog.h"
#include <QRadioButton>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
{
    int ret;

    if(ui->radioButton->isChecked())
    {
        QMessageBox Box;
        Box.setText("테스트 테스트");
        ret = Box.exec();
    }

    else if(ui->radioButton_2->isChecked())
    {
        QMessageBox Box;
        Box.setText("테스트2");
        Box.setInformativeText("테스트3");
        Box.setStandardButtons(QMessageBox::Save |QMessageBox::Discard | QMessageBox::Cancel );
        Box.setDefaultButton(QMessageBox::Save);
        ret = Box.exec();

    }

    else {
        ret = QMessageBox::warning(this, tr("My Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save |QMessageBox::Discard | QMessageBox::Cancel,
                                   QMessageBox::Save);
    }

    switch(ret){
        case QMessageBox::Save:
        break;
        case QMessageBox::Discard:
        break;
        case QMessageBox::Cancel:
        break;
        default:
        break;
    }

}
