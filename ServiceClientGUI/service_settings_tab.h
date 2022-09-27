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

class GeneralSettingsTab;
class RecoverySettingsTab;
class DependentServicesTab;


class ServiceSettingsTab : public QDialog
{
    Q_OBJECT

public:
    ServiceSettingsTab(QString serviceName_, IManager* pIManager_, QMap<QString, QList<QString>> dependentServices, QWidget* parent = nullptr);
    ~ServiceSettingsTab();

    void updateServiceInfo();

    /*сделать аксессоры*/

    const SERVICE_FAILURE_ACTIONS_UDT* getServiceFailureActions();
    const SERVICE_STATUS_PROCESS_UDT* getServiceStatus();
    const QUERY_SERVICE_CONFIG_UDT* getServiceConfig();
    const SERVICE_DESCRIPTION_UDT* getServiceDescription();
    void stopService();
    void startService();

    void setServiceConfig(int closeTab);
    
    /*general*/
    void changeStartType(int newType);

    /*recovery*/
    void changeFailureAction(int actionNumber, int newAction);
    void changeFailureActionDelay(int actionNumber, int newAction);
    void applySettingsBtnCheck(bool check);
    void changeResetPeriod(int period);
    void changeFailureActionOnNonCrashFlag(int newState);

private slots:

private:

    QString serviceName;

    QPushButton* applySettingsBtn;
    QPushButton* closeSettingsBtn;
    QPushButton* okSettingsBtn;

    QMap<QString, QList<QString>> dependentServices;

    GeneralSettingsTab* generalSettingsTab;
    RecoverySettingsTab* recoverySettingsTab;
    DependentServicesTab* dependentServicesTab;

    SERVICE_STATUS_PROCESS_UDT serviceStatus;
    QUERY_SERVICE_CONFIG_UDT serviceConfig;
    SERVICE_FAILURE_ACTIONS_UDT serviceFailureActions;
    SERVICE_DESCRIPTION_UDT serviceDescription;

    QTabWidget* tabWidget;
    IManager* pIManager;
};