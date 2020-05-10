#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "QLineEdit"
#include "QPushButton"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QString dlgLineStr;

signals:
    // 메인 윈도우로 전송할 신호 함수 선언
    void sendData(QString text);

public slots:
   // 전송 버튼 클릭시 호출 함수 선언
    void dButtonClick();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
