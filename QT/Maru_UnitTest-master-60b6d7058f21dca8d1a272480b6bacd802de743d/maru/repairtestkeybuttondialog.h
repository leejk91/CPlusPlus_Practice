#ifndef REPAIRTESTKEYBUTTONDIALOG_H
#define REPAIRTESTKEYBUTTONDIALOG_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class RepairTestKeyButtonDialog;
}

class RepairTestKeyButtonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestKeyButtonDialog(QWidget *parent = nullptr);
    ~RepairTestKeyButtonDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::RepairTestKeyButtonDialog *ui;

signals:
    void RepairTestKeyButtonDialogClosed();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // REPAIRTESTKEYBUTTONDIALOG_H
