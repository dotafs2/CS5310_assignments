/********************************************************************************
** Form generated from reading UI file 'mainwindowui.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWUI_H
#define UI_MAINWINDOWUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindowUI
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindowUI)
    {
        if (mainWindowUI->objectName().isEmpty())
            mainWindowUI->setObjectName("mainWindowUI");
        mainWindowUI->resize(400, 300);
        centralwidget = new QWidget(mainWindowUI);
        centralwidget->setObjectName("centralwidget");
        mainWindowUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindowUI);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 400, 17));
        mainWindowUI->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindowUI);
        statusbar->setObjectName("statusbar");
        mainWindowUI->setStatusBar(statusbar);

        retranslateUi(mainWindowUI);

        QMetaObject::connectSlotsByName(mainWindowUI);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindowUI)
    {
        mainWindowUI->setWindowTitle(QCoreApplication::translate("mainWindowUI", "mainWindowUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindowUI: public Ui_mainWindowUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWUI_H
