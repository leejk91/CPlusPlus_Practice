#ifndef REPAIRTESTLCDDIALOG_H
#define REPAIRTESTLCDDIALOG_H

#include <QDialog>

#include "systemutil.h"

//#include <QProcess>

namespace Ui {
class RepairTestLcdDialog;
}

class RepairTestLcdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestLcdDialog(QWidget *parent = nullptr);
    ~RepairTestLcdDialog();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::RepairTestLcdDialog *ui;
    SystemUtil backlight;
    int value;

    //QProcess *proc;


signals:
    void RepairTestLcdDialogClosed();
};

#endif // REPAIRTESTLCDDIALOG_H
