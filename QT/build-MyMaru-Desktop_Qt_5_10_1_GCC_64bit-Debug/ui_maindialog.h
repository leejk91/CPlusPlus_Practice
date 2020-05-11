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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QPushButton *pushButton;
    QTreeWidget *treeWidget;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(800, 1280);
        MainDialog->setStyleSheet(QLatin1String("#MainDialog{\n"
"\n"
"	background-color : #444444;\n"
"\n"
"}"));
        pushButton = new QPushButton(MainDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 781, 141));
        treeWidget = new QTreeWidget(MainDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(10, 740, 781, 381));
        label = new QLabel(MainDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(200, 220, 281, 17));
        label_2 = new QLabel(MainDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(200, 200, 251, 17));
        label_2->setStyleSheet(QLatin1String("#label\n"
"{\n"
"	font: bold italic 10px;\n"
"	color: white;\n"
"}\n"
""));

        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "MainDialog", nullptr));
        pushButton->setText(QApplication::translate("MainDialog", "Admin", nullptr));
        label->setText(QApplication::translate("MainDialog", "\354\213\240\354\232\251\354\271\264\353\223\234\353\245\274 \354\271\264\353\223\234\353\246\254\353\215\224\354\227\220 \353\260\200\354\226\264 \353\204\243\354\226\264\354\243\274\354\204\270\354\232\224.", nullptr));
        label_2->setText(QApplication::translate("MainDialog", "\354\203\201\355\222\210\354\235\204 \352\265\254\353\247\244 \355\225\230\354\213\234\353\240\244\353\251\264 \355\225\230\353\213\250\354\235\230 \352\267\270\353\246\274\352\263\274 \352\260\231\354\235\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
