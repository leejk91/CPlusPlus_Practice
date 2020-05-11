/********************************************************************************
** Form generated from reading UI file 'repairtestimageshowdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTIMAGESHOWDIALOG_H
#define UI_REPAIRTESTIMAGESHOWDIALOG_H

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

class Ui_RepairTestImageShowDialog
{
public:
    QLabel *label;
    QLabel *label_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_9;
    QFrame *frame;
    QLabel *label_10;

    void setupUi(QDialog *RepairTestImageShowDialog)
    {
        if (RepairTestImageShowDialog->objectName().isEmpty())
            RepairTestImageShowDialog->setObjectName(QStringLiteral("RepairTestImageShowDialog"));
        RepairTestImageShowDialog->resize(800, 1280);
        label = new QLabel(RepairTestImageShowDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 800, 1280));
        label->setStyleSheet(QLatin1String("QLabel{\n"
"\n"
"	background-color : #444444;\n"
"\n"
"}"));
        label_3 = new QLabel(RepairTestImageShowDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(455, 1450, 790, 50));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(0, 50));
        label_3->setStyleSheet(QLatin1String("QLabel{\n"
"	background-color:white;\n"
"}"));
        pushButton = new QPushButton(RepairTestImageShowDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(560, 1240, 89, 25));
        pushButton_2 = new QPushButton(RepairTestImageShowDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(680, 1240, 89, 25));
        label_6 = new QLabel(RepairTestImageShowDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(50, 40, 700, 300));
        label_6->setStyleSheet(QLatin1String("QLabel{\n"
"	border-radius:20px;\n"
"	background-color:white;\n"
"}"));
        label_2 = new QLabel(RepairTestImageShowDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(80, 130, 340, 130));
        label_4 = new QLabel(RepairTestImageShowDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(440, 90, 230, 74));
        label_4->setMinimumSize(QSize(230, 74));
        label_4->setStyleSheet(QLatin1String("QLabel{\n"
"	font: bold 50px;\n"
"color:#FF8000;\n"
"}"));
        label_5 = new QLabel(RepairTestImageShowDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(440, 160, 161, 74));
        label_5->setMinimumSize(QSize(0, 0));
        label_5->setStyleSheet(QLatin1String("QLabel{\n"
"	font: bold 50px;\n"
"color:#3366FF;\n"
"}"));
        label_7 = new QLabel(RepairTestImageShowDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(440, 230, 281, 74));
        label_7->setMinimumSize(QSize(230, 74));
        label_7->setStyleSheet(QLatin1String("QLabel{\n"
"	font: bold 50px;\n"
"color:#FF8000;\n"
"}"));
        label_9 = new QLabel(RepairTestImageShowDialog);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(600, 160, 141, 74));
        label_9->setMinimumSize(QSize(0, 0));
        label_9->setStyleSheet(QLatin1String("QLabel{\n"
"	font: bold 50px;\n"
"color:#FF8000;\n"
"}"));
        frame = new QFrame(RepairTestImageShowDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 385, 780, 741));
        frame->setStyleSheet(QLatin1String("QFrame{\n"
"	border-style:none;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_10 = new QLabel(frame);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(90, 130, 600, 500));

        retranslateUi(RepairTestImageShowDialog);

        QMetaObject::connectSlotsByName(RepairTestImageShowDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestImageShowDialog)
    {
        RepairTestImageShowDialog->setWindowTitle(QApplication::translate("RepairTestImageShowDialog", "Dialog", Q_NULLPTR));
        label->setText(QString());
        label_3->setText(QString());
        pushButton->setText(QApplication::translate("RepairTestImageShowDialog", "Start", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("RepairTestImageShowDialog", "Close", Q_NULLPTR));
        label_6->setText(QString());
        label_2->setText(QString());
        label_4->setText(QApplication::translate("RepairTestImageShowDialog", "\354\225\210\353\205\225\355\225\230\354\204\270\354\232\224", Q_NULLPTR));
        label_5->setText(QApplication::translate("RepairTestImageShowDialog", "UBCn", Q_NULLPTR));
        label_7->setText(QApplication::translate("RepairTestImageShowDialog", "\354\213\234\354\212\244\355\205\234\354\236\205\353\213\210\353\213\244", Q_NULLPTR));
        label_9->setText(QApplication::translate("RepairTestImageShowDialog", "\353\254\264\354\235\270", Q_NULLPTR));
        label_10->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RepairTestImageShowDialog: public Ui_RepairTestImageShowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTIMAGESHOWDIALOG_H
