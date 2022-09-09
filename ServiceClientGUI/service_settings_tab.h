#pragma once

#include "qtabwidget.h"
#include "qpushbutton.h"
#include <qlayout.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include "qdialogbuttonbox.h"
#include "QSignalMapper"

#include "general_settings_tab.h"
#include "recovery_settings_tab.h"

#include "../ServiceManager/server_type_info_h.h"
#include "../ServiceManager/server_type_info_i.c"



class ServiceSettingsTab : public QDialog
{
    Q_OBJECT

public:
    ServiceSettingsTab(QString serviceName_, IManager* pIManager_, QWidget* parent = nullptr);
    ~ServiceSettingsTab();

    void updateServiceInfo();

    SERVICE_STATUS_PROCESS_UDT serviceStatus;
    QUERY_SERVICE_CONFIG_UDT serviceConfig;
    SERVICE_FAILURE_ACTIONS_UDT serviceFailureActions;
    //SERVICE_FAILURE_ACTIONSW serviceFailureActions_;

    QPushButton* applySettingsBtn;

    QPushButton* closeSettingsBtn;
    QPushButton* okSettingsBtn;

    void stopService();
    void startService();
    void setServiceConfig(int closeTab);


private slots:
    void closeSettings();
    //void changeStartType(int);
    //void setServiceConfig();



private:
    QString serviceName;

    QTabWidget* tabWidget;
    //QPushButton* buttonBox;
    IManager* pIManager;

};