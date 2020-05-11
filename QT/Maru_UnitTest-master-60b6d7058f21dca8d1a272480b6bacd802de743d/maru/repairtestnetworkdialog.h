#ifndef REPAIRTESTNETWORKDIALOG_H
#define REPAIRTESTNETWORKDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class RepairTestNetworkDialog;
}

class RepairTestNetworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestNetworkDialog(QWidget *parent = nullptr);
    ~RepairTestNetworkDialog();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void onPingEnded();

private:
    Ui::RepairTestNetworkDialog *ui;
    void onPing();

public:
    QString sourceIP;
    QString destinationIP;
    QProcess *proc;

signals:
    void RepairTestNetworkDialogClosed();
};

#endif // REPAIRTESTNETWORKDIALOG_H
