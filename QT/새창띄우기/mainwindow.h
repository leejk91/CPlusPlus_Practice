#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>

#include "dialog.h"
#include <QDebug>

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

    void recv_value(QString A,int B,int C);

private slots:
    void on_pushButton_clicked();


    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    Dialog *Dia_log; //새창
    void ShowSubDialog(QWidget *widget);



};

#endif // MAINWINDOW_H
