#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "new.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QGraphicsView *mainview, *subview;
    QGraphicsScene *mainscene, *subscene;
    QGraphicsProxyWidget *mainproxy, *subproxy;
    void ShowSubDialog(QDialog *widget);


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_subDialog_closed();

    void rotate();


private:
    Ui::Dialog *ui;
    New *newpage;
};

#endif // DIALOG_H
