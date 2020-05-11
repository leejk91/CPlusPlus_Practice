#ifndef REPAIRTESTSOUNDDIALOG_H
#define REPAIRTESTSOUNDDIALOG_H

#include <QDialog>

#include "systemutil.h"

namespace Ui {
class RepairTestSoundDialog;
}

class RepairTestSoundDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTestSoundDialog(QWidget *parent = nullptr);
    ~RepairTestSoundDialog();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::RepairTestSoundDialog *ui;

    void volumeCheck(qint8);

    qint8 currentVolume, preVolume;

    SystemUtil sound;

signals:
    void RepairTestSoundDialogClosed();
};

#endif // REPAIRTESTSOUNDDIALOG_H
