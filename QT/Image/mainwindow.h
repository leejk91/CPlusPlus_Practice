#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

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

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_timeout();
    void on_stimeout();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
