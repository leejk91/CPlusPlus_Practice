#ifndef IP_CONFIG_H
#define IP_CONFIG_H

#include <QDialog>

namespace Ui {
class IP_config;
}

class IP_config : public QDialog
{
    Q_OBJECT

public:
    explicit IP_config(QWidget *parent = nullptr);
    ~IP_config();

private:
    Ui::IP_config *ui;
};

#endif // IP_CONFIG_H
