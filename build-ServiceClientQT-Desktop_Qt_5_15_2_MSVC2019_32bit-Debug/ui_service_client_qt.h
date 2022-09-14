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
#include <QtWidgets/QColumnView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServiceClientQT
{
public:
    QWidget *centralwidget;
    QTreeView *servicesTreeView;
    QColumnView *columnView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ServiceClientQT)
    {
        if (ServiceClientQT->objectName().isEmpty())
            ServiceClientQT->setObjectName(QString::fromUtf8("ServiceClientQT"));
        ServiceClientQT->resize(800, 600);
        centralwidget = new QWidget(ServiceClientQT);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        servicesTreeView = new QTreeView(centralwidget);
        servicesTreeView->setObjectName(QString::fromUtf8("servicesTreeView"));
        servicesTreeView->setGeometry(QRect(80, 120, 256, 192));
        columnView = new QColumnView(centralwidget);
        columnView->setObjectName(QString::fromUtf8("columnView"));
        columnView->setGeometry(QRect(70, 360, 256, 192));
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
