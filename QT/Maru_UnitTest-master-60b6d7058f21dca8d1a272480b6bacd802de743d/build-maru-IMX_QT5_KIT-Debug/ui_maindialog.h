/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QFrame *frame;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_7;
    QPushButton *pushButton_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_6;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_5;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(800, 1280);
        frame = new QFrame(MainDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(70, 60, 660, 1151));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(80, 20, 511, 1082));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
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

        verticalLayout_2->addWidget(label);

        verticalSpacer = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(0, 70));
        pushButton->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton);

        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(0, 70));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold  20px;\n"
"}"));

        verticalLayout->addWidget(pushButton_3);

        pushButton_7 = new QPushButton(layoutWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setMinimumSize(QSize(0, 70));
        pushButton_7->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton_7);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 70));
        pushButton_2->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton_2);

        pushButton_4 = new QPushButton(layoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(0, 70));
        pushButton_4->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton_4);

        pushButton_8 = new QPushButton(layoutWidget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setMinimumSize(QSize(0, 70));
        pushButton_8->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton_8);

        pushButton_9 = new QPushButton(layoutWidget);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setMinimumSize(QSize(0, 70));
        pushButton_9->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton_9);

        pushButton_6 = new QPushButton(layoutWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setMinimumSize(QSize(0, 70));
        pushButton_6->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}"));

        verticalLayout->addWidget(pushButton_6);

        pushButton_10 = new QPushButton(layoutWidget);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setMinimumSize(QSize(0, 70));
        pushButton_10->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_10);

        pushButton_11 = new QPushButton(layoutWidget);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setMinimumSize(QSize(0, 70));
        pushButton_11->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 20px;\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_11);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        pushButton_5 = new QPushButton(layoutWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setMinimumSize(QSize(0, 120));
        pushButton_5->setStyleSheet(QLatin1String("QPushButton{\n"
"	font: bold 30px;\n"
"}"));

        verticalLayout_2->addWidget(pushButton_5);


        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "MainDialog", Q_NULLPTR));
        label->setText(QApplication::translate("MainDialog", "BingoMaru Unit Test", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainDialog", "Key Button Test", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainDialog", "Backlight Test", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("MainDialog", "Network Test", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainDialog", "Sound Test", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainDialog", "Door Lock Test", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("MainDialog", "T.O.F Test", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("MainDialog", "Image Show Test", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("MainDialog", "Card Reader Test", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("MainDialog", "TEST", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("MainDialog", "PopUp Dialog", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainDialog", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
