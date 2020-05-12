#include "ip_config.h"
#include "ui_ip_config.h"

IP_config::IP_config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IP_config)
{
    ui->setupUi(this);
}

IP_config::~IP_config()
{
    delete ui;
}
