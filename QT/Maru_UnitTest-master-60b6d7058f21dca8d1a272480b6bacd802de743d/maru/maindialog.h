#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QProcess>

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

public:
    QGraphicsView *mainview, *subview;
    QGraphicsScene *mainscene, *subscene;
    QGraphicsProxyWidget *mainproxy, *subproxy;

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

    void on_subDialog_closed();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_test(QString msg);

private:
    Ui::MainDialog *ui;

    QProcess process;

    void ShowSubDialog(QWidget *widget);

};

#endif // MAINDIALOG_H
