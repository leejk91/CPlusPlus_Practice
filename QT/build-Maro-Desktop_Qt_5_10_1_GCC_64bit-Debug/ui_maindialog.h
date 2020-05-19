/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QFrame *frame;
    QStackedWidget *stackedWidget;
    QWidget *waiting;
    QLabel *ad;
    QWidget *detected;
    QLabel *ad_2;
    QWidget *list;
    QLabel *announce;
    QFrame *frame_2;
    QPushButton *config;
    QLabel *store_name;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(600, 800);
        frame = new QFrame(MainDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 200, 600, 600));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        stackedWidget = new QStackedWidget(frame);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(20, 20, 570, 525));
        waiting = new QWidget();
        waiting->setObjectName(QStringLiteral("waiting"));
        ad = new QLabel(waiting);
        ad->setObjectName(QStringLiteral("ad"));
        ad->setGeometry(QRect(10, 10, 550, 500));
        stackedWidget->addWidget(waiting);
        detected = new QWidget();
        detected->setObjectName(QStringLiteral("detected"));
        ad_2 = new QLabel(detected);
        ad_2->setObjectName(QStringLiteral("ad_2"));
        ad_2->setGeometry(QRect(10, 10, 550, 500));
        stackedWidget->addWidget(detected);
        list = new QWidget();
        list->setObjectName(QStringLiteral("list"));
        stackedWidget->addWidget(list);
        announce = new QLabel(frame);
        announce->setObjectName(QStringLiteral("announce"));
        announce->setGeometry(QRect(0, 560, 600, 30));
        frame_2 = new QFrame(MainDialog);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(0, 0, 600, 150));
        frame_2->setStyleSheet(QStringLiteral(""));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        config = new QPushButton(frame_2);
        config->setObjectName(QStringLiteral("config"));
        config->setGeometry(QRect(10, 10, 40, 40));
        config->setMinimumSize(QSize(40, 40));
        store_name = new QLabel(frame_2);
        store_name->setObjectName(QStringLiteral("store_name"));
        store_name->setGeometry(QRect(60, 20, 400, 20));

        retranslateUi(MainDialog);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "MainDialog", nullptr));
        ad->setText(QApplication::translate("MainDialog", "ad", nullptr));
        ad_2->setText(QApplication::translate("MainDialog", "ad", nullptr));
        announce->setText(QApplication::translate("MainDialog", "Announce", nullptr));
        config->setText(QString());
        store_name->setText(QApplication::translate("MainDialog", "Unknown", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
