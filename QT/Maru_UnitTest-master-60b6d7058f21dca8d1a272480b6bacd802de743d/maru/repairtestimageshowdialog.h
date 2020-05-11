#ifndef REPAIRTESTIMAGESHOWDIALOG_H
#define REPAIRTESTIMAGESHOWDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLabel>

namespace Ui {
class RepairTestImageShowDialog;
}

class RepairTestImageShowDialog : public QDialog
{
    Q_OBJECT

public:
    enum Direction{LeftToRight,RightToLeft};
    explicit RepairTestImageShowDialog(QWidget *parent = nullptr);
    ~RepairTestImageShowDialog();

private:
    Ui::RepairTestImageShowDialog *ui;

    QTimer *timer;
    QStringList images;
    int index, length;

    QLabel *label;
    QTimer *stimer;

signals:
    void RepairTestImageShowDialogClosed();

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_timeout();
    void on_stimeout();

};

#endif // REPAIRTESTIMAGESHOWDIALOG_H
