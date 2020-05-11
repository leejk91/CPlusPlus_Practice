#ifndef REPAIRTESTDOORLOCKDIALOG_H
#define REPAIRTESTDOORLOCKDIALOG_H

#include <QDialog>
#include <QTimer>

#include "marudoorlock.h"

namespace Ui {
class RepairTestDoorlockDialog;
}

class RepairTestDoorlockDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestDoorlockDialog(QWidget *parent = nullptr);
    ~RepairTestDoorlockDialog();

private:
    Ui::RepairTestDoorlockDialog *ui;
    QTimer *status_timer;

signals:
    void RepairTestDoorlockDialogClosed();

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_timeout();
};

#endif // REPAIRTESTDOORLOCKDIALOG_H
