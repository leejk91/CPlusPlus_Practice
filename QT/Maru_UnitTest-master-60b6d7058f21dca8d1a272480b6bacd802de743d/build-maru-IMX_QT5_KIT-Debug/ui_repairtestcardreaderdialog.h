/********************************************************************************
** Form generated from reading UI file 'repairtestcardreaderdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTCARDREADERDIALOG_H
#define UI_REPAIRTESTCARDREADERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RepairTestCardReaderDialog
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *RepairTestCardReaderDialog)
    {
        if (RepairTestCardReaderDialog->objectName().isEmpty())
            RepairTestCardReaderDialog->setObjectName(QStringLiteral("RepairTestCardReaderDialog"));
        RepairTestCardReaderDialog->resize(800, 1280);
        pushButton = new QPushButton(RepairTestCardReaderDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(150, 390, 89, 25));

        retranslateUi(RepairTestCardReaderDialog);

        QMetaObject::connectSlotsByName(RepairTestCardReaderDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestCardReaderDialog)
    {
        RepairTestCardReaderDialog->setWindowTitle(QApplication::translate("RepairTestCardReaderDialog", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("RepairTestCardReaderDialog", "TEST", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RepairTestCardReaderDialog: public Ui_RepairTestCardReaderDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTCARDREADERDIALOG_H
