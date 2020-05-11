#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void recData(QString str);


private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Dialog *dlg;
};

#endif // MAINWINDOW_H
