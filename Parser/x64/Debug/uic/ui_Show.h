/********************************************************************************
** Form generated from reading UI file 'Show.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOW_H
#define UI_SHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShowWindow
{
public:
    QWidget *centralwidget;

    void setupUi(QMainWindow *ShowWindow)
    {
        if (ShowWindow->objectName().isEmpty())
            ShowWindow->setObjectName(QString::fromUtf8("ShowWindow"));
        ShowWindow->resize(800, 600);
        centralwidget = new QWidget(ShowWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ShowWindow->setCentralWidget(centralwidget);

        retranslateUi(ShowWindow);

        QMetaObject::connectSlotsByName(ShowWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ShowWindow)
    {
        ShowWindow->setWindowTitle(QApplication::translate("ShowWindow", "Show", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowWindow: public Ui_ShowWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOW_H