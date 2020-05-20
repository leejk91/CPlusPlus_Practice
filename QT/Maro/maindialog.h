#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QDebug>

#include <QGraphicsView>
#include <QGraphicsScene>

#include <QDir>

#include <QTreeWidgetItem>
#include <QFont>

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();


    QGraphicsView *mainview, *subview;
    QGraphicsScene *mainscene, *subscene;
    QGraphicsProxyWidget *mainproxy, *subproxy;

private slots:

    void announce_move_timeout();
    void adimageslide_timeout();

    void on_subDialog_closed();

    void on_config_clicked();
    void on_pushButton_clicked(); // Stacked Test

    void list_insert(QString A,int B,int C);

private:
    Ui::MainDialog *ui;

    QStringList images;
    int index, length;

    QTimer *announce_move, *adimageslide;

    void ShowSubDialog(QWidget *widget);

};

#endif // MAINDIALOG_H
