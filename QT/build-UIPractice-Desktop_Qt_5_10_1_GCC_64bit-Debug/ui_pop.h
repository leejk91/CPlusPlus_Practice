/********************************************************************************
** Form generated from reading UI file 'pop.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POP_H
#define UI_POP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Pop
{
public:

    void setupUi(QDialog *Pop)
    {
        if (Pop->objectName().isEmpty())
            Pop->setObjectName(QStringLiteral("Pop"));
        Pop->resize(400, 300);

        retranslateUi(Pop);

        QMetaObject::connectSlotsByName(Pop);
    } // setupUi

    void retranslateUi(QDialog *Pop)
    {
        Pop->setWindowTitle(QApplication::translate("Pop", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Pop: public Ui_Pop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POP_H
