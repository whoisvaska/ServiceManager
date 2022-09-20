#pragma once

#include "qtabwidget.h"
#include "qpushbutton.h"
#include <qlayout.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include "qdialogbuttonbox.h"
#include "QSignalMapper"
#include "qstylefactory.h"

#include "value_to_str.h"
#include "general_settings_tab.h"
#include "recovery_settings_tab.h"
#include "dependent_services_tab.h"

#include "../ServiceManager/server_type_info_h.h"
#include "../ServiceManager/server_type_info_i.c"


class ServiceSettingsTab : public QDialog
{
    Q_OBJECT

public:
    ServiceSettingsTab(QString serviceName_, IManager* pIManager_, QMap<QString, QList<QString>> dependentServices, QWidget* parent = nullptr);
    ~ServiceSettingsTab();

    void updateServiceInfo();

    QString serviceName;
    SERVICE_STATUS_PROCESS_UDT serviceStatus;
    QUERY_SERVICE_CONFIG_UDT serviceConfig;
    SERVICE_FAILURE_ACTIONS_UDT serviceFailureActions;
    SERVICE_DESCRIPTION_UDT serviceDescription;

    QPushButton* applySettingsBtn;
    QPushButton* closeSettingsBtn;
    QPushButton* okSettingsBtn;

    QMap<QString, QList<QString>> dependentServices;

    void stopService();
    void startService();
    void setServiceConfig(int closeTab);


private slots:
    void closeSettings();
private:
    QTabWidget* tabWidget;
    //QPushButton* buttonBox;
    IManager* pIManager;

};