#include "service_settings_wnd.h"

ServiceSettingsWnd::ServiceSettingsWnd(QString serviceName_, IManager* pIManager_, QWidget* parent)
    : QMainWindow (parent)
{
    //ui.setupUi(this);
    this->pIManager = pIManager_;
    this->serviceName = serviceName_;
   
   
    QWidget* wdg = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout(wdg);

    this->okSettingsBtn = new QPushButton();
    this->okSettingsBtn->setText("OK");

    this->okSettingsBtn->setGeometry(QRect(QPoint(70, 10), QSize(50, 30)));
    vlay->addWidget(okSettingsBtn);


    this->applySettingsBtn = new QPushButton();
    this->applySettingsBtn->setText("Apply");
    this->applySettingsBtn->setDisabled(true);
    this->applySettingsBtn->setGeometry(QRect(QPoint(70, 10), QSize(50, 30)));
    connect(this->applySettingsBtn, SIGNAL(clicked()), this, SLOT(setServiceConfig()));
    vlay->addWidget(applySettingsBtn);


    this->startServiceBtn = new QPushButton("Start");
    this->startServiceBtn->setGeometry(QRect(QPoint(70, 10), QSize(50, 30)));
    connect(this->startServiceBtn, SIGNAL(clicked()), this, SLOT(startService()));
    vlay->addWidget(startServiceBtn);


    this->stopServiceBtn = new QPushButton("Stop");
    this->stopServiceBtn->setGeometry(QRect(QPoint(70, 10), QSize(50, 30)));
    connect(this->stopServiceBtn, SIGNAL(clicked()), this, SLOT(stopService()));
    vlay->addWidget(stopServiceBtn);

    this->startTypeBox = new QComboBox();
    /*
        команды расположены в порядке соответсвующем значению задефайненной константы
        что позволяет передавать выбранный тип сразу в структуру сервиса
        https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-changeserviceconfiga
    */
    QStringList commands = { "SERVICE_BOOT_START", 
        "SERVICE_SYSTEM_START", 
        "SERVICE_AUTO_START", 
        "SERVICE_DEMAND_START",
        "SERVICE_DISABLED"
    };

    startTypeBox->addItems(commands);
    connect(this->startTypeBox, SIGNAL(currentIndexChanged(int)), SLOT(changeStartType(int)));
    vlay->addWidget(startTypeBox);


    wdg->setLayout(vlay);
    setCentralWidget(wdg);

    this->updateServiceInfo();

    this->setWindowTitle(serviceName);
    this->setFixedSize(640, 480);
}


void ServiceSettingsWnd::changeStartType(int newIndex) 
{
    this->applySettingsBtn->setEnabled(true);
    this->serviceConfig.dwStartType = newIndex;
}


void ServiceSettingsWnd::setServiceConfig() 
{
    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());

    //QMessageBox::information(
    //    this,
    //    "setServiceConfig",
    //    "setServiceConfig",
    //    QMessageBox::Ok);

    this->pIManager->changeServiceConfigSM(szServiceName, &this->serviceConfig);

    this->applySettingsBtn->setDisabled(true);

    return;
}

void ServiceSettingsWnd::startService() 
{
    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());
    this->pIManager->startService(szServiceName);

    return;
}

void ServiceSettingsWnd::stopService() 
{

    BSTR szServiceName = SysAllocString(serviceName.toStdWString().c_str());
    this->pIManager->stopService(szServiceName);

    return;
}


void ServiceSettingsWnd::updateServiceInfo() 
{
    BSTR szServiceName = SysAllocString(this->serviceName.toStdWString().c_str());

    this->pIManager->queryServiceInfo(szServiceName, &this->serviceStatus);
    

    if (this->serviceStatus.dwCurrentState == SERVICE_RUNNING) 
    {
        this->startServiceBtn->setDisabled(true);
    }

    if (this->serviceStatus.dwCurrentState == SERVICE_STOPPED) 
    {
        this->stopServiceBtn->setDisabled(true);
    }

    this->pIManager->queryServiceConfigSM(szServiceName, &this->serviceConfig);

    this->startTypeBox->setCurrentIndex(this->serviceConfig.dwStartType);
    
    return;
}