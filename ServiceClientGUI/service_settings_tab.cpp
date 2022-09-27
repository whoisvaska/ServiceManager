#include "service_settings_tab.h"

ServiceSettingsTab::ServiceSettingsTab(QString serviceName_, IManager* pIManager_, QMap<QString, QList<QString>> dependentServices_, QWidget* parent)
    : QDialog(parent)
{
    //QMessageBox::information(this, "", "ServiceSettingsTab");

    this->dependentServices = dependentServices_;

    this->pIManager = pIManager_;
    this->serviceName = serviceName_;

    this->updateServiceInfo();

    tabWidget = new QTabWidget;
    
    this->okSettingsBtn = new QPushButton(this);
    this->okSettingsBtn->setText("Ok");
    connect(okSettingsBtn, &QPushButton::clicked, [this]() {
        this->setServiceConfig(true);
        });
    
    this->applySettingsBtn = new QPushButton();
    this->applySettingsBtn->setText("Apply");
    this->applySettingsBtn->setDisabled(true);
    connect(applySettingsBtn, &QPushButton::clicked, [this]() {
        this->setServiceConfig(false);
        });
   

    generalSettingsTab = new GeneralSettingsTab(this, serviceName);
    tabWidget->addTab(generalSettingsTab, tr("General"));
    
    recoverySettingsTab = new RecoverySettingsTab(this);
    tabWidget->addTab(recoverySettingsTab, tr("Recovery"));

    dependentServicesTab = new DependentServicesTab(this, this->dependentServices);
    tabWidget->addTab(dependentServicesTab, tr("Dependent"));

    this->closeSettingsBtn = new QPushButton();
    this->closeSettingsBtn->setText("Close");
    connect(this->closeSettingsBtn, SIGNAL(clicked()), this, SLOT(this->close()));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);


    mainLayout->addWidget(okSettingsBtn);
    mainLayout->addWidget(closeSettingsBtn);

    mainLayout->addWidget(applySettingsBtn);
    setLayout(mainLayout);
    this->setWindowTitle(serviceName);

    this->applySettingsBtn->setDisabled(true);

    //this->setFixedSize(640, 480);
}


ServiceSettingsTab::~ServiceSettingsTab() 
{
    //QMessageBox::information(this, "ServSet", "");

    if (this->applySettingsBtn != NULL) delete (this->applySettingsBtn);
    if (this->closeSettingsBtn != NULL) delete (this->closeSettingsBtn);
    if (this->okSettingsBtn != NULL)    delete (this->okSettingsBtn);

    if (this->generalSettingsTab != NULL)    delete (this->generalSettingsTab);
    if (this->recoverySettingsTab != NULL)    delete (this->recoverySettingsTab);
    if (this->dependentServicesTab != NULL)    delete (this->dependentServicesTab);
    if (this->tabWidget != NULL)    delete (this->tabWidget);    
}


void ServiceSettingsTab::startService() 
{
    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());
    this->pIManager->startService(szServiceName);

    return;
}

void ServiceSettingsTab::stopService() 
{
    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());
    this->pIManager->stopService(szServiceName);

    return;
}

void ServiceSettingsTab::updateServiceInfo() 
{
    BSTR szServiceName = SysAllocString(this->serviceName.toStdWString().c_str());

    this->pIManager->queryServiceInfo(szServiceName, &this->serviceStatus);

    this->pIManager->queryServiceConfigSM(szServiceName, &this->serviceConfig);

    this->pIManager->queryServiceFailureActionsSM(szServiceName, &this->serviceFailureActions);

    /*QMessageBox::information(
       this,
       "setServiceConfig",
       "setServiceConfig",
       QMessageBox::Ok);*/

    this->pIManager->queryServiceDescriptionSM(szServiceName, &this->serviceDescription);

    return;
}


void ServiceSettingsTab::changeStartType(int newType)
{
    this->serviceConfig.dwStartType = newType;
}


const SERVICE_FAILURE_ACTIONS_UDT* ServiceSettingsTab::getServiceFailureActions()
{
    return const_cast<const SERVICE_FAILURE_ACTIONS_UDT*>(&serviceFailureActions);
}

const QUERY_SERVICE_CONFIG_UDT* ServiceSettingsTab::getServiceConfig()
{
    return const_cast<const QUERY_SERVICE_CONFIG_UDT*>(&serviceConfig);

}

const SERVICE_DESCRIPTION_UDT* ServiceSettingsTab::getServiceDescription()
{
    return const_cast<const SERVICE_DESCRIPTION_UDT*>(&serviceDescription);

}


const SERVICE_STATUS_PROCESS_UDT* ServiceSettingsTab::getServiceStatus()
{
    return const_cast<const SERVICE_STATUS_PROCESS_UDT*>(&serviceStatus);

}


void ServiceSettingsTab::changeFailureAction(int actionNumber, int newAction)
{
   this->applySettingsBtn->setEnabled(true);

    switch (actionNumber) 
    {
    case 1:
        this->serviceFailureActions.lpsaAction1.Type = newAction;
        break;
    case 2:
        this->serviceFailureActions.lpsaAction2.Type = newAction;
        break;
    case 3:
        this->serviceFailureActions.lpsaAction3.Type = newAction;
        break;
    }
}


void ServiceSettingsTab::applySettingsBtnCheck(bool check)
{
    this->applySettingsBtn->setEnabled(check);
}



void ServiceSettingsTab::changeFailureActionDelay(int actionNumber, int delay)
{
    this->applySettingsBtn->setEnabled(true);

    this->serviceFailureActions.cActions = 3;
    switch (actionNumber)
    {
    case 1:
        this->serviceFailureActions.lpsaAction1.Delay = delay;
        break;
    case 2:
        this->serviceFailureActions.lpsaAction2.Delay = delay;
        break;
    case 3:
        this->serviceFailureActions.lpsaAction3.Delay = delay;
        break;
    }
}


void ServiceSettingsTab::changeResetPeriod(int period)
{
    this->serviceFailureActions.dwResetPeriod = period;
}


void ServiceSettingsTab::changeFailureActionOnNonCrashFlag(int newState)
{
    this->serviceFailureActions.fFailureActionsOnNonCrashFailures = (bool)newState;
}


void ServiceSettingsTab::setServiceConfig(int closeTab) {
    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());

    if (serviceFailureActions.lpRebootMsg != NULL)
    {
        SysFreeString(serviceFailureActions.lpRebootMsg);
    }

    if (serviceFailureActions.lpCommand != NULL)
    {
        SysFreeString(serviceFailureActions.lpCommand);
    }

    this->serviceFailureActions.lpRebootMsg = SysAllocString(this->recoverySettingsTab->getRebootMessageText().toStdWString().c_str());
    this->serviceFailureActions.lpCommand = SysAllocString(this->recoverySettingsTab->getRunProgramText().toStdWString().c_str());
 

    this->pIManager->changeServiceConfigSM(szServiceName, &this->serviceConfig);

    this->pIManager->changeServiceFailureActionsSM(szServiceName, &serviceFailureActions);

    this->applySettingsBtn->setDisabled(true);

    if (closeTab == 1) 
    {
        this->close();
    }

    return;
}