/********************************************************************************
** Form generated from reading UI file 'repairtestktcdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTKTCDIALOG_H
#define UI_REPAIRTESTKTCDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_RepairTestKTCDialog
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_3;
    QTextEdit *textEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *RepairTestKTCDialog)
    {
        if (RepairTestKTCDialog->objectName().isEmpty())
            RepairTestKTCDialog->setObjectName(QStringLiteral("RepairTestKTCDialog"));
        RepairTestKTCDialog->resize(800, 1280);
        frame = new QFrame(RepairTestKTCDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(19, 120, 761, 1050));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 60, 509, 80));
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
        pushButton_3->setGeometry(QRect(120, 980, 501, 41));
        pushButton_3->setMinimumSize(QSize(0, 0));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        textEdit = new QTextEdit(frame);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(20, 210, 721, 711));
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(650, 930, 89, 25));

        retranslateUi(RepairTestKTCDialog);

        QMetaObject::connectSlotsByName(RepairTestKTCDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestKTCDialog)
    {
        RepairTestKTCDialog->setWindowTitle(QApplication::translate("RepairTestKTCDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("RepairTestKTCDialog", "KTC TEST", nullptr));
        pushButton_3->setText(QApplication::translate("RepairTestKTCDialog", "Close", nullptr));
        pushButton->setText(QApplication::translate("RepairTestKTCDialog", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RepairTestKTCDialog: public Ui_RepairTestKTCDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTKTCDIALOG_H
