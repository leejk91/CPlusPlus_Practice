#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
signals:
    void sendData(QString text);

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QString dlgLineStr;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
