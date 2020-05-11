/********************************************************************************
** Form generated from reading UI file 'repairtesttofdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTTOFDIALOG_H
#define UI_REPAIRTESTTOFDIALOG_H

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

class Ui_RepairTestTOFDialog
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *RepairTestTOFDialog)
    {
        if (RepairTestTOFDialog->objectName().isEmpty())
            RepairTestTOFDialog->setObjectName(QStringLiteral("RepairTestTOFDialog"));
        RepairTestTOFDialog->resize(800, 1280);
        frame = new QFrame(RepairTestTOFDialog);
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
        pushButton_3->setGeometry(QRect(70, 386, 501, 100));
        pushButton_3->setMinimumSize(QSize(0, 100));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(218, 200, 191, 61));
        pushButton->setStyleSheet(QLatin1String("QPushButton{\n"
"font: bold 20px;\n"
"}"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(240, 287, 91, 61));
        label_2->setStyleSheet(QLatin1String("QLabel{\n"
"padding: 10px;\n"
"/*background-color: #BBBBBB;*/\n"
"font: bold 25px;\n"
"qproperty-alignment: 'AlignVCenter | AlignRight';\n"
"}"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(330, 287, 61, 61));
        label_3->setStyleSheet(QLatin1String("QLabel{\n"
"/*background-color: #BBBBBB;*/\n"
"font: bold 25px;\n"
"qproperty-alignment: 'AlignVCenter  | AlignLeft';\n"
"}\n"
""));

        retranslateUi(RepairTestTOFDialog);

        QMetaObject::connectSlotsByName(RepairTestTOFDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestTOFDialog)
    {
        RepairTestTOFDialog->setWindowTitle(QApplication::translate("RepairTestTOFDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("RepairTestTOFDialog", "TOF Test", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("RepairTestTOFDialog", "Close", Q_NULLPTR));
        pushButton->setText(QApplication::translate("RepairTestTOFDialog", "start", Q_NULLPTR));
        label_2->setText(QApplication::translate("RepairTestTOFDialog", "0", Q_NULLPTR));
        label_3->setText(QApplication::translate("RepairTestTOFDialog", "mm", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RepairTestTOFDialog: public Ui_RepairTestTOFDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTTOFDIALOG_H
