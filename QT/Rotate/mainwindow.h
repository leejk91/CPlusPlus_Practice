#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "dialog.h"
#include "form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsView *mainview;
    QGraphicsScene *mainscene;
    QGraphicsProxyWidget *mainproxy;

private slots:
    void A_Time_out();

    void on_pushButton_clicked();


    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    Dialog *dia_log;
    Form *form;

};

#endif // MAINWINDOW_H
