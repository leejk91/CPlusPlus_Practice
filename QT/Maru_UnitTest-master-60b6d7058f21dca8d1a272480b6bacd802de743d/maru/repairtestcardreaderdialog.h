#ifndef REPAIRTESTCARDREADERDIALOG_H
#define REPAIRTESTCARDREADERDIALOG_H

#include <QDialog>

namespace Ui {
class RepairTestCardReaderDialog;
}

class RepairTestCardReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestCardReaderDialog(QWidget *parent = nullptr);
    ~RepairTestCardReaderDialog();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::RepairTestCardReaderDialog *ui;

signals:
    void RepairTestCardReaderDialogClosed();
};

#endif // REPAIRTESTCARDREADERDIALOG_H
