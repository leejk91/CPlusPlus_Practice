/********************************************************************************
** Form generated from reading UI file 'repairtestserialdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTSERIALDIALOG_H
#define UI_REPAIRTESTSERIALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_RepairTestSerialDialog
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_3;
    QFrame *frame_2;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_5;
    QFrame *frame_3;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QTextEdit *textEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QFrame *frame_4;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QPushButton *pushButton_16;

    void setupUi(QDialog *RepairTestSerialDialog)
    {
        if (RepairTestSerialDialog->objectName().isEmpty())
            RepairTestSerialDialog->setObjectName(QStringLiteral("RepairTestSerialDialog"));
        RepairTestSerialDialog->resize(800, 1280);
        frame = new QFrame(RepairTestSerialDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(70, 120, 660, 1141));
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
        pushButton_3->setGeometry(QRect(30, 1070, 601, 51));
        pushButton_3->setMinimumSize(QSize(0, 0));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(30, 300, 601, 81));
        frame_2->setStyleSheet(QLatin1String("#frame_2{\n"
"/*\n"
"	border-style:outset;\n"
"*/\n"
"	border: 1px solid black;\n"
"}"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        pushButton_4 = new QPushButton(frame_2);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(500, 10, 89, 25));
        lineEdit_2 = new QLineEdit(frame_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(10, 40, 471, 25));
        pushButton_5 = new QPushButton(frame_2);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(500, 40, 89, 25));
        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(30, 410, 601, 191));
        frame_3->setStyleSheet(QLatin1String("QFrame{\n"
"	border: 1px solid black\n"
"}"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        pushButton_6 = new QPushButton(frame_3);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(500, 10, 89, 171));
        pushButton_7 = new QPushButton(frame_3);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(10, 10, 150, 50));
        pushButton_8 = new QPushButton(frame_3);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(170, 10, 150, 50));
        pushButton_9 = new QPushButton(frame_3);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(330, 10, 150, 50));
        pushButton_10 = new QPushButton(frame_3);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(10, 70, 150, 50));
        pushButton_11 = new QPushButton(frame_3);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(170, 70, 150, 50));
        pushButton_12 = new QPushButton(frame_3);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        pushButton_12->setGeometry(QRect(330, 70, 150, 50));
        pushButton_13 = new QPushButton(frame_3);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        pushButton_13->setGeometry(QRect(10, 130, 150, 50));
        pushButton_14 = new QPushButton(frame_3);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        pushButton_14->setGeometry(QRect(170, 130, 150, 50));
        pushButton_15 = new QPushButton(frame_3);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));
        pushButton_15->setGeometry(QRect(330, 130, 150, 50));
        textEdit = new QTextEdit(frame);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(30, 630, 601, 381));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 280, 81, 17));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 390, 67, 17));
        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 170, 81, 17));
        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(30, 190, 601, 81));
        frame_4->setStyleSheet(QLatin1String("#frame_4{\n"
"/*\n"
"	border-style:outset;\n"
"*/\n"
"	border: 1px solid black;\n"
"}"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        pushButton = new QPushButton(frame_4);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(500, 10, 89, 25));
        lineEdit = new QLineEdit(frame_4);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 40, 471, 25));
        pushButton_2 = new QPushButton(frame_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(500, 40, 89, 25));
        pushButton_16 = new QPushButton(frame);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        pushButton_16->setGeometry(QRect(540, 1024, 89, 31));

        retranslateUi(RepairTestSerialDialog);

        QMetaObject::connectSlotsByName(RepairTestSerialDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestSerialDialog)
    {
        RepairTestSerialDialog->setWindowTitle(QApplication::translate("RepairTestSerialDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("RepairTestSerialDialog", "Serial Test", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("RepairTestSerialDialog", "Close", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("RepairTestSerialDialog", "Open", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("RepairTestSerialDialog", "02010B010E01D700007B4A03", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("RepairTestSerialDialog", "Send", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("RepairTestSerialDialog", "Open", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("RepairTestSerialDialog", "POLLING", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("RepairTestSerialDialog", "\353\252\250\353\223\210 \354\236\254\353\266\200\355\214\205", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("RepairTestSerialDialog", "\354\213\240\354\232\251\354\271\264\353\223\234\352\261\260\353\236\230\354\232\224\354\262\255", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("RepairTestSerialDialog", "\354\213\240\354\232\251\354\271\264\353\223\234\352\261\260\353\236\230\352\262\260\352\263\274\354\240\204\354\206\241", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("RepairTestSerialDialog", "\354\213\234\352\260\204\354\204\270\355\214\205", Q_NULLPTR));
        pushButton_12->setText(QApplication::translate("RepairTestSerialDialog", "\354\271\264\353\223\234\353\260\260\354\266\234", Q_NULLPTR));
        pushButton_13->setText(QApplication::translate("RepairTestSerialDialog", "\353\252\250\353\223\210\354\227\205\353\215\260\354\235\264\355\212\270\354\264\210\352\270\260\355\231\224", Q_NULLPTR));
        pushButton_14->setText(QApplication::translate("RepairTestSerialDialog", "\353\252\250\353\223\210\354\227\205\353\215\260\354\235\264\355\212\270", Q_NULLPTR));
        pushButton_15->setText(QApplication::translate("RepairTestSerialDialog", "\354\271\264\353\223\234\354\203\201\355\203\234\354\240\225\353\263\264", Q_NULLPTR));
        label_2->setText(QApplication::translate("RepairTestSerialDialog", "Controller", Q_NULLPTR));
        label_3->setText(QApplication::translate("RepairTestSerialDialog", "Reader", Q_NULLPTR));
        label_4->setText(QApplication::translate("RepairTestSerialDialog", "Debugger", Q_NULLPTR));
        pushButton->setText(QApplication::translate("RepairTestSerialDialog", "Open", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("RepairTestSerialDialog", "Test Message", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("RepairTestSerialDialog", "Send", Q_NULLPTR));
        pushButton_16->setText(QApplication::translate("RepairTestSerialDialog", "Clear", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RepairTestSerialDialog: public Ui_RepairTestSerialDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTSERIALDIALOG_H
