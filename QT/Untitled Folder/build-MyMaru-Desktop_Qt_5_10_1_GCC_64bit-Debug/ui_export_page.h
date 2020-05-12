/********************************************************************************
** Form generated from reading UI file 'export_page.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORT_PAGE_H
#define UI_EXPORT_PAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_export_page
{
public:
    QPushButton *pushButton;
    QTreeWidget *treeWidget;
    QCalendarWidget *calendarWidget;
    QPushButton *pushButton_2;

    void setupUi(QDialog *export_page)
    {
        if (export_page->objectName().isEmpty())
            export_page->setObjectName(QStringLiteral("export_page"));
        export_page->resize(800, 1280);
        export_page->setStyleSheet(QLatin1String("#export_page\n"
"\n"
"{\n"
"	background-color : #444444;\n"
"\n"
"}"));
        pushButton = new QPushButton(export_page);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 800, 130));
        treeWidget = new QTreeWidget(export_page);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(25, 160, 750, 800));
        calendarWidget = new QCalendarWidget(export_page);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setGeometry(QRect(310, 970, 456, 171));
        pushButton_2 = new QPushButton(export_page);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(710, 1200, 60, 60));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(30, 20));

        retranslateUi(export_page);

        QMetaObject::connectSlotsByName(export_page);
    } // setupUi

    void retranslateUi(QDialog *export_page)
    {
        export_page->setWindowTitle(QApplication::translate("export_page", "Dialog", nullptr));
        pushButton->setText(QApplication::translate("export_page", "PushButton", nullptr));
        pushButton_2->setText(QApplication::translate("export_page", "\353\213\253\352\270\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class export_page: public Ui_export_page {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORT_PAGE_H
