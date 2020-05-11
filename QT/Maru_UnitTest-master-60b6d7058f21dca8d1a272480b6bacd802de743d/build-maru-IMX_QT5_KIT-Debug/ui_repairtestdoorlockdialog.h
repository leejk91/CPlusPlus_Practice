/********************************************************************************
** Form generated from reading UI file 'repairtestdoorlockdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTDOORLOCKDIALOG_H
#define UI_REPAIRTESTDOORLOCKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RepairTestDoorlockDialog
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QLabel *label_2;

    void setupUi(QDialog *RepairTestDoorlockDialog)
    {
        if (RepairTestDoorlockDialog->objectName().isEmpty())
            RepairTestDoorlockDialog->setObjectName(QStringLiteral("RepairTestDoorlockDialog"));
        RepairTestDoorlockDialog->resize(800, 1280);
        frame = new QFrame(RepairTestDoorlockDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(70, 120, 660, 1050));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 60, 509, 80));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 80));
        label->setStyleSheet(QLatin1String("QLabel{\n"
"border-radius:5px;\n"
"padding: 2px;\n"
"background-color: #6E6E6E;\n"
"font: bold 20px;\n"
"color:white;\n"
"qproperty-alignment: AlignCenter;\n"
"}\n"
""));
        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(70, 580, 501, 100));
        pushButton_3->setMinimumSize(QSize(0, 100));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(70, 190, 511, 100));
        pushButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(70, 330, 511, 131));
        label_2->setStyleSheet(QLatin1String("QLabel{\n"
"/*background-color: #BBBBBB;*/\n"
"font: bold 30px;\n"
"qproperty-alignment: 'AlignCenter ';\n"
"}\n"
""));

        retranslateUi(RepairTestDoorlockDialog);

        QMetaObject::connectSlotsByName(RepairTestDoorlockDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestDoorlockDialog)
    {
        RepairTestDoorlockDialog->setWindowTitle(QApplication::translate("RepairTestDoorlockDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("RepairTestDoorlockDialog", "Doorlock Test", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("RepairTestDoorlockDialog", "Exit", Q_NULLPTR));
        pushButton->setText(QApplication::translate("RepairTestDoorlockDialog", "Door Open", Q_NULLPTR));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RepairTestDoorlockDialog: public Ui_RepairTestDoorlockDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTDOORLOCKDIALOG_H
