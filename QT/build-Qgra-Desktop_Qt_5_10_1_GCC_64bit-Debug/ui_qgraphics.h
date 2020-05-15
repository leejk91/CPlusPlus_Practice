/********************************************************************************
** Form generated from reading UI file 'qgraphics.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGRAPHICS_H
#define UI_QGRAPHICS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_QGraphics
{
public:

    void setupUi(QDialog *QGraphics)
    {
        if (QGraphics->objectName().isEmpty())
            QGraphics->setObjectName(QStringLiteral("QGraphics"));
        QGraphics->resize(400, 300);

        retranslateUi(QGraphics);

        QMetaObject::connectSlotsByName(QGraphics);
    } // setupUi

    void retranslateUi(QDialog *QGraphics)
    {
        QGraphics->setWindowTitle(QApplication::translate("QGraphics", "QGraphics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QGraphics: public Ui_QGraphics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGRAPHICS_H
