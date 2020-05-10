#include "dialog.h"
#include "ui_dialog.h"

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

void Dialog::dButtonClick()
{
    // 대화상자의 라인에디트 상자에 입력된 문자열을 dlgLineStr에 대입
    dlgLineStr=ui->lineEdit->text();
    // 대화상자의 sendData 함수와 메인윈도우의 recData 함수와 연결시킨다.
    connect(this, SIGNAL(sendData(QString)), parent(), SLOT(recData(QString)));
    //  sendData 함수를 메인 윈도우의  recData 함수로  전송
    emit sendData(dlgLineStr);

    qDebug()<<"value:"<<dlgLineStr<<endl;
}
