#ifndef EXPORT_PAGE_H
#define EXPORT_PAGE_H

#include <QDialog>

namespace Ui {
class export_page;
}

class export_page : public QDialog
{
    Q_OBJECT

public:
    explicit export_page(QWidget *parent = nullptr);
    ~export_page();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::export_page *ui;
};

#endif // EXPORT_PAGE_H
