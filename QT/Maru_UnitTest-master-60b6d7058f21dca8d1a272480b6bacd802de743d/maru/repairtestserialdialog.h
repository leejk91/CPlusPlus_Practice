#ifndef REPAIRTESTSERIALDIALOG_H
#define REPAIRTESTSERIALDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class RepairTestSerialDialog;
}

class RepairTestSerialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestSerialDialog(QWidget *parent = nullptr);
    ~RepairTestSerialDialog();

private:
    Ui::RepairTestSerialDialog *ui;
    QProcess download;

signals:
    void RepairTestSerialDialogClosed();

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_6_clicked();
    void debugMessageReceived(QString str, int color);
    void on_Step1();
    void on_Step2();

private slots:
    void readerMessageReceived(const QByteArray &message);

    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_16_clicked();

public:
    unsigned char track2Length;
    unsigned char track2[40];
    unsigned char koces[512];
};

#endif // REPAIRTESTSERIALDIALOG_H
