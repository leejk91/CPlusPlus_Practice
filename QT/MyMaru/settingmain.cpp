#include "settingmain.h"
#include "ui_settingmain.h"

SettingMain::SettingMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingMain)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("QLabel{font:bold 24px; color:white; background-color:#444444}");

}

SettingMain::~SettingMain()
{
    delete ui;
}

void SettingMain::on_pushButton_clicked() // ID setting
{

}

void SettingMain::on_pushButton_2_clicked() // IP Setting
{

}


void SettingMain::on_pushButton_3_clicked() // 입고 목록
{
    import = new Import(this);
    import->show();
}

void SettingMain::on_pushButton_4_clicked() // 출고 목록
{
    exportpage = new export_page(this);
    exportpage -> show();
}

void SettingMain::on_pushButton_7_clicked() // 재고 목록
{

}


void SettingMain::on_pushButton_8_clicked() // LCD
{

}

void SettingMain::on_pushButton_9_clicked() // Vloum
{

}

void SettingMain::on_pushButton_10_clicked() // 기능테스트
{

}

void SettingMain::on_pushButton_11_clicked() // 관리 비밀번호 변경
{

}

void SettingMain::on_pushButton_12_clicked() //  프로그램 종료
{
    this->close();
}
