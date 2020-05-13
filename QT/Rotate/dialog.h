#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include <QWheelEvent>
#include <QKeyEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QGraphicsView *mainview2;
    QGraphicsScene *mainscene2;
    QGraphicsProxyWidget *mainproxy2;

    //QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(Qpixmap(":/new/prefix1/Dog3.png"));
    //pixmapItem->setTransformationMode(Qt::smoothTransformation);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
