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
#include <QtWidgets/QTreeWidget>
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
    QLabel *detected_image;
    QLabel *detected_text;
    QWidget *list;
    QTreeWidget *treeWidget;
    QLabel *announce;
    QFrame *frame_2;
    QPushButton *config;
    QLabel *store_name;
    QLabel *logo;
    QPushButton *pushButton;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(800, 1280);
        frame = new QFrame(MainDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 350, 800, 1000));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        stackedWidget = new QStackedWidget(frame);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 800, 800));
        waiting = new QWidget();
        waiting->setObjectName(QStringLiteral("waiting"));
        ad = new QLabel(waiting);
        ad->setObjectName(QStringLiteral("ad"));
        ad->setGeometry(QRect(50, 50, 700, 600));
        ad->setStyleSheet(QLatin1String("#ad\n"
" { \n"
"	background-image : url(:/new/adimage/images/ad/ad01.png);\n"
" }"));
        stackedWidget->addWidget(waiting);
        detected = new QWidget();
        detected->setObjectName(QStringLiteral("detected"));
        detected_image = new QLabel(detected);
        detected_image->setObjectName(QStringLiteral("detected_image"));
        detected_image->setGeometry(QRect(250, 25, 300, 400));
        detected_text = new QLabel(detected);
        detected_text->setObjectName(QStringLiteral("detected_text"));
        detected_text->setGeometry(QRect(100, 450, 600, 25));
        detected_text->setAlignment(Qt::AlignCenter);
        stackedWidget->addWidget(detected);
        list = new QWidget();
        list->setObjectName(QStringLiteral("list"));
        treeWidget = new QTreeWidget(list);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(50, 50, 700, 700));
        stackedWidget->addWidget(list);
        announce = new QLabel(frame);
        announce->setObjectName(QStringLiteral("announce"));
        announce->setGeometry(QRect(0, 825, 800, 30));
        frame_2 = new QFrame(MainDialog);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(0, 0, 800, 250));
        frame_2->setStyleSheet(QStringLiteral(""));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        config = new QPushButton(frame_2);
        config->setObjectName(QStringLiteral("config"));
        config->setGeometry(QRect(10, 10, 40, 40));
        config->setMinimumSize(QSize(40, 40));
        store_name = new QLabel(frame_2);
        store_name->setObjectName(QStringLiteral("store_name"));
        store_name->setGeometry(QRect(60, 20, 381, 20));
        logo = new QLabel(frame_2);
        logo->setObjectName(QStringLiteral("logo"));
        logo->setGeometry(QRect(690, 10, 100, 50));
        pushButton = new QPushButton(MainDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(710, 260, 89, 25));

        retranslateUi(MainDialog);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "MainDialog", nullptr));
        ad->setText(QString());
        detected_image->setText(QString());
        detected_text->setText(QApplication::translate("MainDialog", "\354\271\264\353\223\234\353\245\274 \355\210\254\354\236\205\352\265\254\354\227\220 \353\204\243\354\226\264\354\243\274\354\204\270\354\232\224 ", nullptr));
        announce->setText(QApplication::translate("MainDialog", "Announce", nullptr));
        config->setText(QString());
        store_name->setText(QApplication::translate("MainDialog", "Unknown", nullptr));
        logo->setText(QString());
        pushButton->setText(QApplication::translate("MainDialog", "stacked test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
