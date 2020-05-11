#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTabWidget>
#include <QTimer>
#include <QTreeWidget>
#include <QLayout>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_push_1_clicked();
    void on_push_2_clicked();
    void on_push_3_clicked();
    void on_push_4_clicked();
    void on_push_5_clicked();
    void on_push_6_clicked();
    void on_push_7_clicked();
    void on_push_8_clicked();
    void on_push_9_clicked();
    void on_push_0_clicked();

    void on_push_C_clicked();

    void on_push_oper1_clicked();
    void on_push_oper2_clicked();
    void on_push_oper3_clicked();
    void on_push_oper4_clicked();

    void on_push_finish_clicked();

    void on_push_back_clicked();

    void deleteitem();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    void addTreeRoot(QString A, QString oper, QString B, QString C);
    void addTreeChild(QTreeWidgetItem *parent,QString name, QString B);

};

#endif // MAINWINDOW_H
