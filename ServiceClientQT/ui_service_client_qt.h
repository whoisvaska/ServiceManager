/********************************************************************************
** Form generated from reading UI file 'service_client_qt.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICE_CLIENT_QT_H
#define UI_SERVICE_CLIENT_QT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServiceClientQT
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ServiceClientQT)
    {
        if (ServiceClientQT->objectName().isEmpty())
            ServiceClientQT->setObjectName(QString::fromUtf8("ServiceClientQT"));
        ServiceClientQT->resize(800, 600);
        menubar = new QMenuBar(ServiceClientQT);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        ServiceClientQT->setMenuBar(menubar);
        centralwidget = new QWidget(ServiceClientQT);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ServiceClientQT->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(ServiceClientQT);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ServiceClientQT->setStatusBar(statusbar);

        retranslateUi(ServiceClientQT);

        QMetaObject::connectSlotsByName(ServiceClientQT);
    } // setupUi

    void retranslateUi(QMainWindow *ServiceClientQT)
    {
        ServiceClientQT->setWindowTitle(QCoreApplication::translate("ServiceClientQT", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServiceClientQT: public Ui_ServiceClientQT {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICE_CLIENT_QT_H
