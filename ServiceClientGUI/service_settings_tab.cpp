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
   

    generalSettingsTab = new GeneralSettingsTab(this);
    tabWidget->addTab(generalSettingsTab, tr("General"));
    
    recoverySettingsTab = new RecoverySettingsTab(this);
    tabWidget->addTab(recoverySettingsTab, tr("Recovery"));

    dependentServicesTab = new DependentServicesTab(this);
    tabWidget->addTab(dependentServicesTab, tr("Dependent"));

    this->closeSettingsBtn = new QPushButton();
    this->closeSettingsBtn->setText("Close");
    //this->closeSettingsBtn->setStyle
    connect(this->closeSettingsBtn, SIGNAL(clicked()), this, SLOT(closeSettings()));

    /*GeneralSettingsTab* generalSettingsTab = new GeneralSettingsTab(this);
    tabWidget->addTab(generalSettingsTab, tr("General"));*/

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);


    mainLayout->addWidget(okSettingsBtn);
    mainLayout->addWidget(closeSettingsBtn);

    mainLayout->addWidget(applySettingsBtn);
    setLayout(mainLayout);
    this->setWindowTitle(serviceName);
    //this->setFixedSize(640, 480);
}


ServiceSettingsTab::~ServiceSettingsTab() 
{
    delete[] this->applySettingsBtn;
    delete[] this->tabWidget;
}


void ServiceSettingsTab::closeSettings() 
{
    this->close();
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
        this->closeSettings();
    }

    return;
}