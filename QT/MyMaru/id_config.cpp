#include "id_config.h"
#include "ui_id_config.h"

ID_Config::ID_Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ID_Config)
{
    ui->setupUi(this);
}

ID_Config::~ID_Config()
{
    delete ui;
}
