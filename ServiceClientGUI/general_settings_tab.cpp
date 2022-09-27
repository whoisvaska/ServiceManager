#include "general_settings_tab.h"


GeneralSettingsTab::GeneralSettingsTab(QWidget* parent, QString serviceName_)
    : QWidget(parent)
{
    //QMessageBox::information(this, "", "GeneralSettingsTab");

    this->serviceSettingsTab = parent;
    this->serviceNameString = serviceName_;

    this->startTypeBox = new QComboBox();
    QStringList commands = { "SERVICE_BOOT_START",
        "SERVICE_SYSTEM_START",
        "SERVICE_AUTO_START",
        "SERVICE_DEMAND_START",
        "SERVICE_DISABLED"
    };
    startTypeBox->addItems(commands);

    connect(this->startTypeBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int newType)
        {
            qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->applySettingsBtnCheck(true);
            qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->changeStartType(newType);

        }
    );

    this->startBtn = new QPushButton();
    this->startBtn->setText("Start");
    connect(this->startBtn, &QPushButton::clicked, [this]()
        {
            qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->startService();
        }
    );

    this->stopBtn = new QPushButton();
    this->stopBtn->setText("Stop");
    connect(this->stopBtn, &QPushButton::clicked, [this]()
        {
            qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->stopService();
        }
    );

    QGridLayout* layout = new QGridLayout();

    QLabel* serviceNameLabel = new QLabel("Service name:");
    QLabel* displayNameLabel = new QLabel("Display name:");

    QLabel* descriptionLabel = new QLabel("Description:");

    QLabel* startTypeLabel = new QLabel("Startup type:");
    QLabel* currentStateLabel = new QLabel("Service status:");

    QLabel* pathNameLabel = new QLabel("Path to executable:");

    serviceName = new QLabel();
    displayName = new QLabel();
    description = new QLabel();
    startType = new QLabel();
    currentState = new QLabel();
    pathName = new QLabel();


    description->setWordWrap(true);
    pathName->setWordWrap(true);


    layout->addWidget(serviceNameLabel, 0, 0);
    layout->addWidget(serviceName, 0, 1);

    layout->addWidget(displayNameLabel, 1, 0);
    layout->addWidget(displayName, 1, 1);

    layout->addWidget(descriptionLabel, 2, 0);
    layout->addWidget(description, 2, 1);

    layout->addWidget(pathNameLabel, 3, 0);
    layout->addWidget(pathName, 3, 1);

    layout->addWidget(startTypeLabel, 4, 0);
    layout->addWidget(startTypeBox, 4, 1);

    layout->addWidget(currentStateLabel, 5, 0);
    layout->addWidget(currentState, 5, 1);

    layout->addWidget(startBtn, 6, 0);
    layout->addWidget(stopBtn, 6, 1);


    setLayout(layout);
    
    setWindowTitle(tr("Tab Dialog"));
    this->updateInfo();
}


GeneralSettingsTab::~GeneralSettingsTab() 
{
    if (this->startTypeBox != NULL) delete (this->startTypeBox);
    if (this->startBtn != NULL) delete (this->startBtn);
    if (this->stopBtn != NULL) delete (this->stopBtn);
    if (this->serviceName != NULL) delete (this->serviceName);
    if (this->displayName != NULL) delete (this->displayName);
    if (this->description != NULL) delete (this->description);
    if (this->startType != NULL) delete (this->startType);
    if (this->currentState != NULL) delete (this->currentState);
    
}



void GeneralSettingsTab::updateInfo() {

    const SERVICE_STATUS_PROCESS_UDT* serviceStatus = qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->getServiceStatus();
    const QUERY_SERVICE_CONFIG_UDT* serviceConfig = qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->getServiceConfig();
    const SERVICE_DESCRIPTION_UDT* serviceDescription = qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->getServiceDescription();

    if (serviceStatus->dwCurrentState == SERVICE_RUNNING) 
    {
        this->startBtn->setDisabled(true);
    }

    if (serviceStatus->dwCurrentState == SERVICE_STOPPED) 
    {
        this->stopBtn->setDisabled(true);
    }

    this->startTypeBox->setCurrentIndex(serviceConfig->dwStartType);

    this->serviceName->setText(serviceNameString);
    this->displayName->setText(QString::fromWCharArray(serviceConfig->lpDisplayName));
    this->description->setText(QString::fromWCharArray(serviceDescription->lpDescription));
    
    this->pathName->setText(QString::fromWCharArray(serviceConfig->lpBinaryPathName));

    this->currentState->setText(currentStateToString(serviceStatus->dwCurrentState));
}
