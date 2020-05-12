/********************************************************************************
** Form generated from reading UI file 'export.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORT_H
#define UI_EXPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_export
{
public:

    void setupUi(QDialog *export)
    {
        if (export->objectName().isEmpty())
            export->setObjectName(QStringLiteral("export"));
        export->resize(400, 300);

        retranslateUi(export);

        QMetaObject::connectSlotsByName(export);
    } // setupUi

    void retranslateUi(QDialog *export)
    {
        export->setWindowTitle(QApplication::translate("export", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class export: public Ui_export {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORT_H
