#ifndef NEW_H
#define NEW_H

#include <QDialog>
#include <QGraphicsView>

namespace Ui {
class New;
}

class New : public QDialog
{
    Q_OBJECT
signals:
    void DialogClosed();
    void DialogRotate();

public:
    explicit New(QWidget *parent = nullptr);
    ~New();
    QGraphicsView *thirdview;
    QGraphicsScene *thirdscene;
    QGraphicsProxyWidget *thirdproxy;
    void ShowThirdDialog(QDialog *widget);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::New *ui;
};

#endif // NEW_H
