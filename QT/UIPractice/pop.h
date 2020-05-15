#ifndef POP_H
#define POP_H

#include <QDialog>

namespace Ui {
class Pop;
}

class Pop : public QDialog
{
    Q_OBJECT

public:
    explicit Pop(QWidget *parent = nullptr);
    ~Pop();

private:
    Ui::Pop *ui;
};

#endif // POP_H
