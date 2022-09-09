#include "service_settings_tab.h"

ServiceSettingsTab::ServiceSettingsTab(QString serviceName_, IManager* pIManager_, QWidget* parent)
    : QDialog(parent)
{
    //GeneralSettingsTab*
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
    this->applySettingsBtn->setEnabled(false);
    connect(applySettingsBtn, &QPushButton::clicked, [this]() {
        this->setServiceConfig(false);
        });
   

    GeneralSettingsTab* generalSettingsTab = new GeneralSettingsTab(this);
    tabWidget->addTab(generalSettingsTab, tr("General"));
    
    RecoverySettingsTab* recoverySettingsTab = new RecoverySettingsTab(this);
    tabWidget->addTab(recoverySettingsTab, tr("Recovery"));


    this->closeSettingsBtn = new QPushButton();
    this->closeSettingsBtn->setText("Close");
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
    this->setFixedSize(640, 480);
}


ServiceSettingsTab::~ServiceSettingsTab() {

    delete[] this->applySettingsBtn;
    delete[] this->tabWidget;
}


void ServiceSettingsTab::closeSettings() {
    QMessageBox::question(this, "Test", "Quit?",
        QMessageBox::Yes | QMessageBox::No);
    
    this->close();
}


void ServiceSettingsTab::startService() {

    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());
    this->pIManager->startService(szServiceName);

    return;
}

void ServiceSettingsTab::stopService() {

    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());
    this->pIManager->stopService(szServiceName);

    return;
}

void ServiceSettingsTab::updateServiceInfo() {
    BSTR szServiceName = SysAllocString(this->serviceName.toStdWString().c_str());

    this->pIManager->queryServiceInfo(szServiceName, &this->serviceStatus);

    this->pIManager->queryServiceConfigSM(szServiceName, &this->serviceConfig);

    //this->serviceFailureActions_ = (SERVICE_FAILURE_ACTIONSW*)LocalAlloc(LMEM_FIXED, sizeof(SERVICE_FAILURE_ACTIONSW));

    this->pIManager->queryServiceFailureActionsSM(szServiceName, &this->serviceFailureActions);

    return;
}


void ServiceSettingsTab::setServiceConfig(int closeTab) {
    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());

    QMessageBox::information(
        this,
        "setServiceConfig",
        "setServiceConfig",
        QMessageBox::Ok);

    this->pIManager->changeServiceConfigSM(szServiceName, &this->serviceConfig);

    this->pIManager->changeServiceFailureActionsSM(szServiceName, &serviceFailureActions);

    this->applySettingsBtn->setDisabled(true);

    if (closeTab == 1) {
        this->closeSettings();
    }

    return;
}