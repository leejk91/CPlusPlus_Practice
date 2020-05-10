#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    int value1;
    int value2;
    int result_value;



public slots:
    void recv_value(int value)
    {
        value1=value;
    }


private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
