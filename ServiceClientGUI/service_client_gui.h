#pragma once

#include <QtWidgets/QMainWindow>
#include <iostream>
#include <QTreeView>
#include <qtreewidget.h>
#include <QStandardItemModel>
#include <qmessagebox.h>
#include <qlist.h>

#include "qsizepolicy.h"

#include "service_settings_tab.h"
//#include "service_settings_wnd.h"
#include "ui_serviceclientgui.h"

//#include "../ServiceManager/server_type_info_i.c"
//#include "../ServiceManager/server_type_info_h.h"

class ServiceSettingsTab;
class DependentServicesTab;
class GeneralSettingsTab;
class RecoverySettingsTab;


class ServiceClientGUI : public QMainWindow
{
    Q_OBJECT

public:
    ServiceClientGUI(QWidget *parent = nullptr);
    ~ServiceClientGUI();
    QMap<QString, QList<QString>> dependentServices;

private slots:
    void ServiceClientGUI::settingsMenuRequested(const QModelIndex& index);

private:
    Ui::ServiceClientGUIClass ui;
    
    void initInterfaces();
    void updateServices();


    IClassFactory* pICF;
    ICreateManager* pICreateManager;
    IManager* pIManager;


    QTreeView* servicesTreeView;
    QStandardItemModel* servicesModel;

    ServiceSettingsTab* serviceSettingsTab;

    SAFEARRAY* servicesStatus;

};
