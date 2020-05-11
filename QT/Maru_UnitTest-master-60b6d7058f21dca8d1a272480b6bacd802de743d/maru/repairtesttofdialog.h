#ifndef REPAIRTESTTOFDIALOG_H
#define REPAIRTESTTOFDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class RepairTestTOFDialog;
}

class RepairTestTOFDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestTOFDialog(QWidget *parent = nullptr);
    ~RepairTestTOFDialog();

private slots:
    void on_pushButton_3_clicked();
    void on_Timer();

    void on_pushButton_clicked();

private:
    Ui::RepairTestTOFDialog *ui;

    QTimer timer;

    int iDistance;
    int model, revision;

signals:
    void RepairTestTOFDialogClosed();
};

#endif // REPAIRTESTTOFDIALOG_H
