#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = nullptr);
    ~TestDialog();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::TestDialog *ui;

signals:
    void TestDialogClosed(QString msg);
};

#endif // TESTDIALOG_H
