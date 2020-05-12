/********************************************************************************
** Form generated from reading UI file 'id_config.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ID_CONFIG_H
#define UI_ID_CONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ID_Config
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *ID_Config)
    {
        if (ID_Config->objectName().isEmpty())
            ID_Config->setObjectName(QStringLiteral("ID_Config"));
        ID_Config->resize(800, 1280);
        pushButton = new QPushButton(ID_Config);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 800, 130));

        retranslateUi(ID_Config);

        QMetaObject::connectSlotsByName(ID_Config);
    } // setupUi

    void retranslateUi(QDialog *ID_Config)
    {
        ID_Config->setWindowTitle(QApplication::translate("ID_Config", "Dialog", nullptr));
        pushButton->setText(QApplication::translate("ID_Config", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ID_Config: public Ui_ID_Config {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ID_CONFIG_H
