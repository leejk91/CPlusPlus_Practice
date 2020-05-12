#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <unistd.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer;
    QTimer *stimer;
    QTimer *sstimer;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_timeout();
    void on_stimeout();
    void on_sstimeout();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();



    void on_actionNew_Window_triggered();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
