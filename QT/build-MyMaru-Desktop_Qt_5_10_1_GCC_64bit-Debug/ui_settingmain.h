/********************************************************************************
** Form generated from reading UI file 'settingmain.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGMAIN_H
#define UI_SETTINGMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingMain
{
public:
    QLabel *label;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;

    void setupUi(QDialog *SettingMain)
    {
        if (SettingMain->objectName().isEmpty())
            SettingMain->setObjectName(QStringLiteral("SettingMain"));
        SettingMain->resize(750, 800);
        SettingMain->setStyleSheet(QLatin1String("#SettingMain\n"
"{\n"
"	background-color : #444444;\n"
"\n"
"}"));
        label = new QLabel(SettingMain);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(330, 10, 81, 16));
        widget = new QWidget(SettingMain);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 90, 721, 636));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(350, 120));

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(350, 120));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setMinimumSize(QSize(350, 120));

        horizontalLayout_2->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(350, 120));

        horizontalLayout_2->addWidget(pushButton_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton_7 = new QPushButton(widget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        sizePolicy.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy);
        pushButton_7->setMinimumSize(QSize(350, 120));

        horizontalLayout_4->addWidget(pushButton_7);

        pushButton_8 = new QPushButton(widget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        sizePolicy.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy);
        pushButton_8->setMinimumSize(QSize(350, 120));

        horizontalLayout_4->addWidget(pushButton_8);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton_9 = new QPushButton(widget);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        sizePolicy.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy);
        pushButton_9->setMinimumSize(QSize(350, 120));

        horizontalLayout_5->addWidget(pushButton_9);

        pushButton_10 = new QPushButton(widget);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        sizePolicy.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
        pushButton_10->setSizePolicy(sizePolicy);
        pushButton_10->setMinimumSize(QSize(350, 120));

        horizontalLayout_5->addWidget(pushButton_10);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        pushButton_11 = new QPushButton(widget);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        sizePolicy.setHeightForWidth(pushButton_11->sizePolicy().hasHeightForWidth());
        pushButton_11->setSizePolicy(sizePolicy);
        pushButton_11->setMinimumSize(QSize(350, 120));

        horizontalLayout_6->addWidget(pushButton_11);

        pushButton_12 = new QPushButton(widget);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        sizePolicy.setHeightForWidth(pushButton_12->sizePolicy().hasHeightForWidth());
        pushButton_12->setSizePolicy(sizePolicy);
        pushButton_12->setMinimumSize(QSize(350, 120));

        horizontalLayout_6->addWidget(pushButton_12);


        verticalLayout->addLayout(horizontalLayout_6);


        retranslateUi(SettingMain);

        QMetaObject::connectSlotsByName(SettingMain);
    } // setupUi

    void retranslateUi(QDialog *SettingMain)
    {
        SettingMain->setWindowTitle(QApplication::translate("SettingMain", "Dialog", nullptr));
        label->setText(QApplication::translate("SettingMain", "\352\264\200\353\246\254\354\236\220 \353\251\224\353\211\264", nullptr));
        pushButton_2->setText(QApplication::translate("SettingMain", "IP \354\204\244\354\240\225", nullptr));
        pushButton->setText(QApplication::translate("SettingMain", "ID \354\204\244\354\240\225", nullptr));
        pushButton_3->setText(QApplication::translate("SettingMain", "\354\236\205\352\263\240 \353\252\251\353\241\235", nullptr));
        pushButton_4->setText(QApplication::translate("SettingMain", "\354\266\234\352\263\240 \353\252\251\353\241\235", nullptr));
        pushButton_7->setText(QApplication::translate("SettingMain", "\354\236\254\352\263\240 \353\252\251\353\241\235", nullptr));
        pushButton_8->setText(QApplication::translate("SettingMain", "LCD \353\260\235\352\270\260", nullptr));
        pushButton_9->setText(QApplication::translate("SettingMain", "Volum", nullptr));
        pushButton_10->setText(QApplication::translate("SettingMain", "\352\270\260\353\212\245\355\205\214\354\212\244\355\212\270", nullptr));
        pushButton_11->setText(QApplication::translate("SettingMain", "\352\264\200\353\246\254 \353\271\204\353\260\200\353\262\210\355\230\270 \353\263\200\352\262\275", nullptr));
        pushButton_12->setText(QApplication::translate("SettingMain", "\355\224\204\353\241\234\352\267\270\353\236\250 \354\242\205\353\243\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingMain: public Ui_SettingMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGMAIN_H
