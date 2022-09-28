/********************************************************************************
** Form generated from reading UI file 'serviceclientgui.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICECLIENTGUI_H
#define UI_SERVICECLIENTGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServiceClientGUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ServiceClientGUIClass)
    {
        if (ServiceClientGUIClass->objectName().isEmpty())
            ServiceClientGUIClass->setObjectName(QString::fromUtf8("ServiceClientGUIClass"));
        ServiceClientGUIClass->resize(600, 400);
        menuBar = new QMenuBar(ServiceClientGUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        ServiceClientGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ServiceClientGUIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ServiceClientGUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ServiceClientGUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ServiceClientGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ServiceClientGUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ServiceClientGUIClass->setStatusBar(statusBar);

        retranslateUi(ServiceClientGUIClass);

        QMetaObject::connectSlotsByName(ServiceClientGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *ServiceClientGUIClass)
    {
        ServiceClientGUIClass->setWindowTitle(QCoreApplication::translate("ServiceClientGUIClass", "ServiceClientGUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServiceClientGUIClass: public Ui_ServiceClientGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICECLIENTGUI_H
