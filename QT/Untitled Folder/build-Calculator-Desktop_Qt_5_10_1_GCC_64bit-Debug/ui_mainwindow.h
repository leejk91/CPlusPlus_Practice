/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QPushButton *push_1;
    QPushButton *push_2;
    QPushButton *push_3;
    QPushButton *push_4;
    QPushButton *push_5;
    QPushButton *push_6;
    QPushButton *push_7;
    QPushButton *push_8;
    QPushButton *push_9;
    QPushButton *push_0;
    QPushButton *push_C;
    QPushButton *push_finish;
    QPushButton *push_oper1;
    QPushButton *push_oper2;
    QPushButton *push_oper3;
    QPushButton *push_oper4;
    QLabel *label;
    QPushButton *push_back;
    QLabel *label_vect1;
    QLabel *label_vect2;
    QLabel *label_oper;
    QPushButton *pushButton;
    QTreeWidget *treeWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(706, 497);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(11);
        MainWindow->setFont(font);
        MainWindow->setIconSize(QSize(20, 24));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 110, 211, 271));
        widget->setStyleSheet(QLatin1String("QPushButton{\n"
"	border-radius: 10px;\n"
"	padding: 2px; \n"
"	background-color: rgb(100,100,100);\n"
"	font: bold italic 10px;\n"
"	color: white;\n"
"}\n"
""));
        push_1 = new QPushButton(widget);
        push_1->setObjectName(QStringLiteral("push_1"));
        push_1->setGeometry(QRect(20, 30, 51, 51));
        push_1->setStyleSheet(QStringLiteral(""));
        push_2 = new QPushButton(widget);
        push_2->setObjectName(QStringLiteral("push_2"));
        push_2->setGeometry(QRect(80, 30, 51, 51));
        push_3 = new QPushButton(widget);
        push_3->setObjectName(QStringLiteral("push_3"));
        push_3->setGeometry(QRect(140, 30, 51, 51));
        push_4 = new QPushButton(widget);
        push_4->setObjectName(QStringLiteral("push_4"));
        push_4->setGeometry(QRect(20, 90, 51, 51));
        push_5 = new QPushButton(widget);
        push_5->setObjectName(QStringLiteral("push_5"));
        push_5->setGeometry(QRect(80, 90, 51, 51));
        push_6 = new QPushButton(widget);
        push_6->setObjectName(QStringLiteral("push_6"));
        push_6->setGeometry(QRect(140, 90, 51, 51));
        push_7 = new QPushButton(widget);
        push_7->setObjectName(QStringLiteral("push_7"));
        push_7->setGeometry(QRect(20, 150, 51, 51));
        push_8 = new QPushButton(widget);
        push_8->setObjectName(QStringLiteral("push_8"));
        push_8->setGeometry(QRect(80, 150, 51, 51));
        push_9 = new QPushButton(widget);
        push_9->setObjectName(QStringLiteral("push_9"));
        push_9->setGeometry(QRect(140, 150, 51, 51));
        push_0 = new QPushButton(widget);
        push_0->setObjectName(QStringLiteral("push_0"));
        push_0->setGeometry(QRect(80, 210, 51, 51));
        push_C = new QPushButton(widget);
        push_C->setObjectName(QStringLiteral("push_C"));
        push_C->setGeometry(QRect(20, 210, 51, 51));
        push_finish = new QPushButton(widget);
        push_finish->setObjectName(QStringLiteral("push_finish"));
        push_finish->setGeometry(QRect(140, 210, 51, 51));
        push_oper1 = new QPushButton(centralWidget);
        push_oper1->setObjectName(QStringLiteral("push_oper1"));
        push_oper1->setGeometry(QRect(240, 130, 51, 51));
        push_oper1->setStyleSheet(QLatin1String("#push_oper1\n"
"{\n"
"	border-image: url(:/new/prefix1/image2/dog2.png);\n"
"	\n"
"}\n"
"\n"
"#push_oper1:press\n"
"{\n"
"	color: white;\n"
"}"));
        push_oper2 = new QPushButton(centralWidget);
        push_oper2->setObjectName(QStringLiteral("push_oper2"));
        push_oper2->setGeometry(QRect(240, 190, 51, 51));
        push_oper3 = new QPushButton(centralWidget);
        push_oper3->setObjectName(QStringLiteral("push_oper3"));
        push_oper3->setGeometry(QRect(240, 250, 51, 51));
        push_oper4 = new QPushButton(centralWidget);
        push_oper4->setObjectName(QStringLiteral("push_oper4"));
        push_oper4->setGeometry(QRect(240, 310, 51, 51));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 70, 241, 41));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        push_back = new QPushButton(centralWidget);
        push_back->setObjectName(QStringLiteral("push_back"));
        push_back->setGeometry(QRect(270, 60, 21, 51));
        label_vect1 = new QLabel(centralWidget);
        label_vect1->setObjectName(QStringLiteral("label_vect1"));
        label_vect1->setGeometry(QRect(30, 20, 67, 17));
        label_vect2 = new QLabel(centralWidget);
        label_vect2->setObjectName(QStringLiteral("label_vect2"));
        label_vect2->setGeometry(QRect(140, 20, 67, 17));
        label_oper = new QLabel(centralWidget);
        label_oper->setObjectName(QStringLiteral("label_oper"));
        label_oper->setGeometry(QRect(110, 20, 21, 17));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(240, 10, 51, 41));
        treeWidget = new QTreeWidget(centralWidget);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(320, 10, 351, 241));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy1);
        treeWidget->setMaximumSize(QSize(5000, 16777215));
        QFont font1;
        font1.setFamily(QStringLiteral("Ubuntu Mono"));
        treeWidget->setFont(font1);
        treeWidget->setToolTipDuration(-6);
        treeWidget->setFrameShape(QFrame::Panel);
        treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        treeWidget->setAutoScrollMargin(16);
        treeWidget->setIndentation(20);
        treeWidget->header()->setDefaultSectionSize(80);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 706, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(clicked(bool)), treeWidget, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        push_1->setText(QApplication::translate("MainWindow", "1", nullptr));
        push_2->setText(QApplication::translate("MainWindow", "2", nullptr));
        push_3->setText(QApplication::translate("MainWindow", "3", nullptr));
        push_4->setText(QApplication::translate("MainWindow", "4", nullptr));
        push_5->setText(QApplication::translate("MainWindow", "5", nullptr));
        push_6->setText(QApplication::translate("MainWindow", "6", nullptr));
        push_7->setText(QApplication::translate("MainWindow", "7", nullptr));
        push_8->setText(QApplication::translate("MainWindow", "8", nullptr));
        push_9->setText(QApplication::translate("MainWindow", "9", nullptr));
        push_0->setText(QApplication::translate("MainWindow", "0", nullptr));
        push_C->setText(QApplication::translate("MainWindow", "C", nullptr));
        push_finish->setText(QApplication::translate("MainWindow", "=", nullptr));
        push_oper1->setText(QApplication::translate("MainWindow", "+", nullptr));
        push_oper2->setText(QApplication::translate("MainWindow", "-", nullptr));
        push_oper3->setText(QApplication::translate("MainWindow", "X", nullptr));
        push_oper4->setText(QApplication::translate("MainWindow", "/", nullptr));
        label->setText(QString());
        push_back->setText(QApplication::translate("MainWindow", "<", nullptr));
        label_vect1->setText(QString());
        label_vect2->setText(QString());
        label_oper->setText(QString());
        pushButton->setText(QApplication::translate("MainWindow", "+++", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
