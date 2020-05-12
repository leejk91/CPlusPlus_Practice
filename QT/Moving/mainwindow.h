#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include <QPropertyAnimation>

//#include <QGraphicsScene>
//#include <QGraphicsView>
//#include <QGraphicsItem>

#include <QThread>
#include <QProcess>
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
    void test_function();

    //QPropertyAnimation *anim;

private:
    Ui::MainWindow *ui;
    QProcess *process;

private slots:
    void move();

};

#endif // MAINWINDOW_H
