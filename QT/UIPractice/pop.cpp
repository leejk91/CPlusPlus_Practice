#include "pop.h"
#include "ui_pop.h"
Pop::Pop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pop)
{
    ui->setupUi(this);

    //ui->treeWidget->setItemDelegate(new pop);


}

Pop::~Pop()
{
    delete ui;
}

void Pop::on_pushButton_clicked()
{
    emit digclose();
}
#if 0
void Pop::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    Pop::paint(painter,option,index);

           QPen pen;
           pen.setWidth(1);
           pen.setColor(QColor(200, 200, 200));
           pen.setStyle(Qt::DotLine);
           painter->setPen(pen);

           painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
           painter->drawLine(option.rect.topRight(), option.rect.bottomRight());

}
#endif
