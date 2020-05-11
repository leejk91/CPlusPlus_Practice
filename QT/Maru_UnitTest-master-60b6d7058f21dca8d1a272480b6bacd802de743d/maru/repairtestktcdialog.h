#ifndef REPAIRTESTKTCDIALOG_H
#define REPAIRTESTKTCDIALOG_H

#include <QDialog>

namespace Ui {
class RepairTestKTCDialog;
}

class RepairTestKTCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestKTCDialog(QWidget *parent = nullptr);
    ~RepairTestKTCDialog();

private:
    Ui::RepairTestKTCDialog *ui;
    void SendResponse(quint8 cmd, quint8 res, const char *data, quint16 len);
    unsigned char rcvBuff[2048];
    unsigned char sndBuff[2048];

signals:
    void RepairTestKTCDialogClosed();

private slots:
    void on_pushButton_3_clicked();
    void on_debugMessageReceived(QString str, int color);

private slots:
    void controllerMessageReceived(const QByteArray &message);
    void on_pushButton_clicked();
};

#endif // REPAIRTESTKTCDIALOG_H
