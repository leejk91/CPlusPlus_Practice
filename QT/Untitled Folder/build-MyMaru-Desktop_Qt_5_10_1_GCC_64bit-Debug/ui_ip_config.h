/********************************************************************************
** Form generated from reading UI file 'ip_config.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IP_CONFIG_H
#define UI_IP_CONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_IP_config
{
public:

    void setupUi(QDialog *IP_config)
    {
        if (IP_config->objectName().isEmpty())
            IP_config->setObjectName(QStringLiteral("IP_config"));
        IP_config->resize(400, 300);

        retranslateUi(IP_config);

        QMetaObject::connectSlotsByName(IP_config);
    } // setupUi

    void retranslateUi(QDialog *IP_config)
    {
        IP_config->setWindowTitle(QApplication::translate("IP_config", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IP_config: public Ui_IP_config {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IP_CONFIG_H
