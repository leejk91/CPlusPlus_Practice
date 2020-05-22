#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMovie>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    void start_gif();
};

#endif // DIALOG_H
