#ifndef IMPORT_H
#define IMPORT_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QDebug>

namespace Ui {
class Import;
}

class Import : public QDialog
{
    Q_OBJECT

public:
    explicit Import(QWidget *parent = nullptr);
    ~Import();
    void additem(QString Code,QString Name,int date,int value,int sale_value);

private slots:
    void on_pushButton_5_clicked();


private:
    Ui::Import *ui;
};

#endif // IMPORT_H
