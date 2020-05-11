/********************************************************************************
** Form generated from reading UI file 'repairtestnetworkdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTNETWORKDIALOG_H
#define UI_REPAIRTESTNETWORKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RepairTestNetworkDialog
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QListWidget *listWidget;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_6;
    QLineEdit *lineEdit;

    void setupUi(QDialog *RepairTestNetworkDialog)
    {
        if (RepairTestNetworkDialog->objectName().isEmpty())
            RepairTestNetworkDialog->setObjectName(QStringLiteral("RepairTestNetworkDialog"));
        RepairTestNetworkDialog->resize(800, 1280);
        frame = new QFrame(RepairTestNetworkDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(40, 120, 660, 1050));
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
        pushButton_3->setGeometry(QRect(70, 590, 511, 51));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy1);
        pushButton_3->setMinimumSize(QSize(0, 0));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(500, 310, 80, 61));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMinimumSize(QSize(0, 0));
        pushButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        listWidget = new QListWidget(frame);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(70, 380, 511, 192));
        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(73, 180, 511, 108));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMinimumSize(QSize(200, 50));
        label_2->setStyleSheet(QLatin1String("\n"
"QLabel{\n"
"	font: bold 20px;\n"
"qproperty-alignment: AlignCenter;\n"
"background-color: #DDDDDD;\n"
"}"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(0, 50));
        label_5->setStyleSheet(QLatin1String("\n"
"QLabel{\n"
"	font: bold 20px;\n"
"}"));

        gridLayout->addWidget(label_5, 0, 1, 1, 1);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(0, 50));
        label_3->setStyleSheet(QLatin1String("\n"
"QLabel{\n"
"	font: bold 20px;\n"
"}"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setMinimumSize(QSize(200, 50));
        label_4->setStyleSheet(QLatin1String("\n"
"QLabel{\n"
"	font: bold 20px;\n"
"qproperty-alignment: AlignCenter;\n"
"background-color: #DDDDDD;\n"
"}"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setMinimumSize(QSize(0, 50));
        label_6->setStyleSheet(QLatin1String("\n"
"QLabel{\n"
"	font: bold 20px;\n"
"}"));

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 50));
        lineEdit->setStyleSheet(QLatin1String("QLineEdit{\n"
"	font: bold 20px;\n"
"}"));

        gridLayout->addWidget(lineEdit, 1, 2, 1, 1);


        retranslateUi(RepairTestNetworkDialog);

        QMetaObject::connectSlotsByName(RepairTestNetworkDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestNetworkDialog)
    {
        RepairTestNetworkDialog->setWindowTitle(QApplication::translate("RepairTestNetworkDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("RepairTestNetworkDialog", "Network Test", nullptr));
        pushButton_3->setText(QApplication::translate("RepairTestNetworkDialog", "Close", nullptr));
        pushButton->setText(QApplication::translate("RepairTestNetworkDialog", "Start", nullptr));
        label_2->setText(QApplication::translate("RepairTestNetworkDialog", "source ip ", nullptr));
        label_5->setText(QApplication::translate("RepairTestNetworkDialog", ":", nullptr));
        label_3->setText(QString());
        label_4->setText(QApplication::translate("RepairTestNetworkDialog", "destination ip", nullptr));
        label_6->setText(QApplication::translate("RepairTestNetworkDialog", ":", nullptr));
        lineEdit->setText(QApplication::translate("RepairTestNetworkDialog", "192.168.0.210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RepairTestNetworkDialog: public Ui_RepairTestNetworkDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTNETWORKDIALOG_H
