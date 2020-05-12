/********************************************************************************
** Form generated from reading UI file 'import.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORT_H
#define UI_IMPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Import
{
public:
    QPushButton *pushButton;
    QTreeWidget *treeWidget;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QTreeWidget *treeWidget_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_12;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_10;
    QLineEdit *lineEdit_10;
    QLabel *label_11;
    QLineEdit *lineEdit_11;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *Import)
    {
        if (Import->objectName().isEmpty())
            Import->setObjectName(QStringLiteral("Import"));
        Import->resize(800, 1280);
        Import->setStyleSheet(QLatin1String("#Import\n"
"{\n"
"	background-color : #444444;\n"
"\n"
"}"));
        pushButton = new QPushButton(Import);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 800, 130));
        pushButton->setMinimumSize(QSize(750, 120));
        treeWidget = new QTreeWidget(Import);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(25, 220, 750, 500));
        pushButton_2 = new QPushButton(Import);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(510, 1210, 60, 60));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(30, 20));
        pushButton_3 = new QPushButton(Import);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(580, 1210, 60, 60));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setMinimumSize(QSize(30, 20));
        pushButton_4 = new QPushButton(Import);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(650, 1210, 60, 60));
        sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy);
        pushButton_4->setMinimumSize(QSize(30, 20));
        pushButton_5 = new QPushButton(Import);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(720, 1210, 60, 60));
        sizePolicy.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy);
        pushButton_5->setMinimumSize(QSize(30, 20));
        treeWidget_2 = new QTreeWidget(Import);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget_2->setHeaderItem(__qtreewidgetitem);
        treeWidget_2->setObjectName(QStringLiteral("treeWidget_2"));
        treeWidget_2->setGeometry(QRect(25, 730, 750, 300));
        widget = new QWidget(Import);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 130, 801, 81));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout->addWidget(lineEdit_2);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        lineEdit_3 = new QLineEdit(widget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout->addWidget(lineEdit_3);

        label_12 = new QLabel(widget);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout->addWidget(label_12);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_10 = new QLabel(widget);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_4->addWidget(label_10);

        lineEdit_10 = new QLineEdit(widget);
        lineEdit_10->setObjectName(QStringLiteral("lineEdit_10"));

        horizontalLayout_4->addWidget(lineEdit_10);

        label_11 = new QLabel(widget);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_4->addWidget(label_11);

        lineEdit_11 = new QLineEdit(widget);
        lineEdit_11->setObjectName(QStringLiteral("lineEdit_11"));

        horizontalLayout_4->addWidget(lineEdit_11);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(Import);

        QMetaObject::connectSlotsByName(Import);
    } // setupUi

    void retranslateUi(QDialog *Import)
    {
        Import->setWindowTitle(QApplication::translate("Import", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Import", "PushButton", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Import", "\354\266\224\352\260\200", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Import", "\354\210\230\354\240\225", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("Import", "\354\202\255\354\240\234", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("Import", "\353\213\253\352\270\260", Q_NULLPTR));
        label->setText(QApplication::translate("Import", "\354\203\201\355\222\210\354\275\224\353\223\234", Q_NULLPTR));
        label_2->setText(QApplication::translate("Import", "\354\203\201\355\222\210\353\252\205", Q_NULLPTR));
        label_3->setText(QApplication::translate("Import", "\354\240\225\354\203\201\352\260\200 \352\270\260\352\260\204", Q_NULLPTR));
        lineEdit_3->setText(QApplication::translate("Import", "0", Q_NULLPTR));
        label_12->setText(QApplication::translate("Import", "\354\235\274", Q_NULLPTR));
        label_10->setText(QApplication::translate("Import", "\354\203\201\355\222\210\352\260\200\352\262\251", Q_NULLPTR));
        lineEdit_10->setText(QApplication::translate("Import", "0", Q_NULLPTR));
        label_11->setText(QApplication::translate("Import", "\355\225\240\354\235\270 \352\260\200\352\262\251", Q_NULLPTR));
        lineEdit_11->setText(QApplication::translate("Import", "0", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Import: public Ui_Import {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORT_H
