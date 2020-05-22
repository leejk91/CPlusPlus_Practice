#ifndef POP_H
#define POP_H

#include <QDialog>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

namespace Ui {
class Pop;
}

class Pop : public QDialog
{
    Q_OBJECT

signals:
    void digclose();

public:
    explicit Pop(QWidget *parent = nullptr);
    ~Pop();
    //void paint(QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    //QStyledItemDelegate(parent);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Pop *ui;
};

#endif // POP_H
