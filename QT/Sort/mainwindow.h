#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SortDialog;
}

class SortDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit SortDialog(QWidget *parent = nullptr);
    ~SortDialog();

    void setColumRange(QChar first, QChar last);

private slots:
    void on_cancelButton_clicked();

private:
    Ui::SortDialog *ui;
};

#endif // MAINWINDOW_H
