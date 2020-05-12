#ifndef ID_CONFIG_H
#define ID_CONFIG_H

#include <QDialog>

namespace Ui {
class ID_Config;
}

class ID_Config : public QDialog
{
    Q_OBJECT

public:
    explicit ID_Config(QWidget *parent = nullptr);
    ~ID_Config();

private:
    Ui::ID_Config *ui;
};

#endif // ID_CONFIG_H
