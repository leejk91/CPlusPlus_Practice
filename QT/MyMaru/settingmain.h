#ifndef SETTINGMAIN_H
#define SETTINGMAIN_H

#include <QDialog>
#include "import.h"
#include "export_page.h"

namespace Ui {
class SettingMain;
}

class SettingMain : public QDialog
{
    Q_OBJECT

public:
    explicit SettingMain(QWidget *parent = nullptr);
    ~SettingMain();
    Import *import;
    export_page *exportpage;



private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::SettingMain *ui;
};

#endif // SETTINGMAIN_H
