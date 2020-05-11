/********************************************************************************
** Form generated from reading UI file 'repairtestkeybuttondialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTKEYBUTTONDIALOG_H
#define UI_REPAIRTESTKEYBUTTONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RepairTestKeyButtonDialog
{
public:
    QFrame *frame;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *RepairTestKeyButtonDialog)
    {
        if (RepairTestKeyButtonDialog->objectName().isEmpty())
            RepairTestKeyButtonDialog->setObjectName(QStringLiteral("RepairTestKeyButtonDialog"));
        RepairTestKeyButtonDialog->resize(800, 1280);
        frame = new QFrame(RepairTestKeyButtonDialog);
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
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(440, 160, 67, 17));
        label_2->setStyleSheet(QLatin1String("QLabel{\n"
"	background-color: #00FF00;\n"
"}\n"
""));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(514, 160, 67, 17));
        label_3->setStyleSheet(QLatin1String("QLabel{\n"
"	background-color: #0066FF;\n"
"}"));
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(70, 180, 511, 468));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setMinimumSize(QSize(200, 100));
        label_4->setStyleSheet(QLatin1String("QLabel{\n"
"    background-color: #FFFF99;\n"
"    font:bold 30px;\n"
"	qproperty-alignment: AlignCenter;\n"
"}\n"
""));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setStyleSheet(QLatin1String("QLabel{\n"
"background-color: #CCCCCC;\n"
"}"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label_5);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setMinimumSize(QSize(200, 100));
        label_6->setStyleSheet(QLatin1String("QLabel{\n"
"background-color: #CCFF99;\n"
"    font:bold 30px;\n"
"	qproperty-alignment: AlignCenter;\n"
"}"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_6);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setStyleSheet(QLatin1String("QLabel{\n"
"background-color: #CCCCCC;\n"
"}"));

        formLayout->setWidget(1, QFormLayout::FieldRole, label_7);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);
        label_8->setMinimumSize(QSize(200, 100));
        label_8->setStyleSheet(QLatin1String("QLabel{\n"
"background-color: #99FFCC;\n"
"    font:bold 30px;\n"
"	qproperty-alignment: AlignCenter;\n"
"}"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_8);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setStyleSheet(QLatin1String("QLabel{\n"
"background-color: #CCCCCC;\n"
"}"));

        formLayout->setWidget(2, QFormLayout::FieldRole, label_9);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(0, 100));
        pushButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(RepairTestKeyButtonDialog);

        QMetaObject::connectSlotsByName(RepairTestKeyButtonDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestKeyButtonDialog)
    {
        RepairTestKeyButtonDialog->setWindowTitle(QApplication::translate("RepairTestKeyButtonDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("RepairTestKeyButtonDialog", "Key Button Test", Q_NULLPTR));
        label_2->setText(QApplication::translate("RepairTestKeyButtonDialog", "Pressed", Q_NULLPTR));
        label_3->setText(QApplication::translate("RepairTestKeyButtonDialog", "Released", Q_NULLPTR));
        label_4->setText(QApplication::translate("RepairTestKeyButtonDialog", "Escape", Q_NULLPTR));
        label_5->setText(QString());
        label_6->setText(QApplication::translate("RepairTestKeyButtonDialog", "Back Space", Q_NULLPTR));
        label_7->setText(QString());
        label_8->setText(QApplication::translate("RepairTestKeyButtonDialog", "Enter", Q_NULLPTR));
        label_9->setText(QString());
        pushButton->setText(QApplication::translate("RepairTestKeyButtonDialog", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RepairTestKeyButtonDialog: public Ui_RepairTestKeyButtonDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTKEYBUTTONDIALOG_H
