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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        MainWindow->setStyleSheet(QLatin1String("#push_button\n"
"{\n"
"	border-radius: 15px;\n"
"	padding: 2px;\n"
"	background-color: rgba(0, 222, 255, 110);\n"
"	font: bold italic 20px;\n"
"	color: white;\n"
"}\n"
"#push_button:pressed\n"
"{\n"
"	border-radius: 15px;\n"
"	padding: 2px;\n"
"	background-color: rgba(0, 222, 255, 110);\n"
"	font: bold italic 20px;\n"
"	color: black;\n"
"}"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(220, 10, 161, 221));
        groupBox->setStyleSheet(QLatin1String("#groubbox\n"
"{\n"
"	background-image\n"
"}"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 170, 151, 171));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(48, 48));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/ui_img/images/dog.png")));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 180, 141, 31));
        pushButton->setStyleSheet(QLatin1String("#pushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	padding: 2px; \n"
"	background-color: rgb(100,100,100);\n"
"	font: bold italic 10px;\n"
"	color: black;\n"
"	\n"
"}\n"
"#pushButton:pressed\n"
"{\n"
"	border-radius: 10px;\n"
"	padding: 2px;\n"
"	background-image: url(:images/dog.png);\n"
"	font: bold italic 10px;\n"
"	color: white;\n"
"}"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 20, 141, 41));
        pushButton_2->setStyleSheet(QLatin1String("#pushButton_2\n"
"{\n"
"	border-radius: 10px;\n"
"	padding: 2px;\n"
"	background-image: url(:/images/dog.png);\n"
"	font: bold italic 10px;\n"
"	color: black;\n"
"	\n"
"}\n"
"#pushButton_2:pressed\n"
"{\n"
"	border-radius: 10px;\n"
"	padding: 2px;\n"
"	background-image: url(:images/dog.png);\n"
"	font: bold italic 10px;\n"
"	color: white;\n"
"}"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "GroupBox", nullptr));
        label->setText(QString());
        pushButton->setText(QApplication::translate("MainWindow", "Press Button", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
