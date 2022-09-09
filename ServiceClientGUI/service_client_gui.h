#pragma once

#include <QtWidgets/QMainWindow>
#include <iostream>
#include <QTreeView>
#include <QStandardItemModel>
#include <qmessagebox.h>


#include "service_settings_tab.h"
//#include "service_settings_wnd.h"
#include "ui_serviceclientgui.h"

//#include "../ServiceManager/server_type_info_i.c"
//#include "../ServiceManager/server_type_info_h.h"



class ServiceClientGUI : public QMainWindow
{
    Q_OBJECT

public:
    ServiceClientGUI(QWidget *parent = nullptr);
    ~ServiceClientGUI();

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
