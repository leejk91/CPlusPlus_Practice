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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QPushButton *pushButton;
    QLabel *label_3;
    QTreeWidget *treeWidget;
    QLabel *label_2;
    QLabel *label;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(800, 1280);
        MainDialog->setStyleSheet(QLatin1String("#MainDialog\n"
"{\n"
"\n"
"	background-color : #444444;\n"
"\n"
"}"));
        pushButton = new QPushButton(MainDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 800, 130));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(750, 120));
        label_3 = new QLabel(MainDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(200, 260, 400, 600));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(400, 200));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/Images/insert.png")));
        label_3->setAlignment(Qt::AlignCenter);
        treeWidget = new QTreeWidget(MainDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(25, 900, 750, 350));
        label_2 = new QLabel(MainDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 137, 622, 55));
        QFont font;
        font.setPointSize(28);
        label_2->setFont(font);
        label_2->setStyleSheet(QLatin1String("#label\n"
"{\n"
"	color: rgb(238, 238, 236);\n"
"	font: 57 13pt \"Ubuntu\";\n"
"}\n"
""));
        label_2->setAlignment(Qt::AlignCenter);
        label = new QLabel(MainDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 198, 614, 55));
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "MainDialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainDialog", "Admin", Q_NULLPTR));
        label_3->setText(QString());
        label_2->setText(QApplication::translate("MainDialog", "\354\203\201\355\222\210\354\235\204 \352\265\254\353\247\244 \355\225\230\354\213\234\353\240\244\353\251\264 \355\225\230\353\213\250\354\235\230 \352\267\270\353\246\274\352\263\274 \352\260\231\354\235\264", Q_NULLPTR));
        label->setText(QApplication::translate("MainDialog", "\354\213\240\354\232\251\354\271\264\353\223\234\353\245\274 \354\271\264\353\223\234\353\246\254\353\215\224\354\227\220 \353\260\200\354\226\264 \353\204\243\354\226\264\354\243\274\354\204\270\354\232\224.", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
