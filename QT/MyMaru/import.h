#ifndef IMPORT_H
#define IMPORT_H

#include <QDialog>

namespace Ui {
class Import;
}

class Import : public QDialog
{
    Q_OBJECT

public:
    explicit Import(QWidget *parent = nullptr);
    ~Import();

private slots:
    void on_pushButton_5_clicked();

private:
    Ui::Import *ui;
};

#endif // IMPORT_H
