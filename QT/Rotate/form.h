#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    QGraphicsView *mainview2;
    QGraphicsScene *mainscene2;
    QGraphicsProxyWidget *mainproxy2;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H
