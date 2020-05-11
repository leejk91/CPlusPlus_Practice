/********************************************************************************
** Form generated from reading UI file 'repairtestlcddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAIRTESTLCDDIALOG_H
#define UI_REPAIRTESTLCDDIALOG_H

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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RepairTestLcdDialog
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_3;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *RepairTestLcdDialog)
    {
        if (RepairTestLcdDialog->objectName().isEmpty())
            RepairTestLcdDialog->setObjectName(QStringLiteral("RepairTestLcdDialog"));
        RepairTestLcdDialog->resize(800, 1280);
        frame = new QFrame(RepairTestLcdDialog);
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
        pushButton_3->setGeometry(QRect(70, 300, 501, 100));
        pushButton_3->setMinimumSize(QSize(0, 100));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(70, 170, 351, 101));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(200);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setStyleSheet(QLatin1String("QLabel{\n"
"	font: bold 30px;\n"
"}"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setStyleSheet(QLatin1String("QLabel{\n"
"	font: bold 30px;\n"
"}"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label_3);

        layoutWidget1 = new QWidget(frame);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(440, 170, 131, 101));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(layoutWidget1);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(layoutWidget1);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(pushButton_2);


        retranslateUi(RepairTestLcdDialog);

        QMetaObject::connectSlotsByName(RepairTestLcdDialog);
    } // setupUi

    void retranslateUi(QDialog *RepairTestLcdDialog)
    {
        RepairTestLcdDialog->setWindowTitle(QApplication::translate("RepairTestLcdDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("RepairTestLcdDialog", "Key Button Test", nullptr));
        pushButton_3->setText(QApplication::translate("RepairTestLcdDialog", "Close", nullptr));
        label_2->setText(QApplication::translate("RepairTestLcdDialog", "Brightness : ", nullptr));
        label_3->setText(QString());
        pushButton->setText(QApplication::translate("RepairTestLcdDialog", "UP", nullptr));
        pushButton_2->setText(QApplication::translate("RepairTestLcdDialog", "DOWN", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RepairTestLcdDialog: public Ui_RepairTestLcdDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAIRTESTLCDDIALOG_H
