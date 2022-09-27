#include "general_settings_tab.h"


GeneralSettingsTab::GeneralSettingsTab(QWidget* parent)
    : QWidget(parent)
{
    //QMessageBox::information(this, "", "GeneralSettingsTab");

    this->serviceSettingsTab = parent;

    this->startTypeBox = new QComboBox();
    QStringList commands = { "SERVICE_BOOT_START",
        "SERVICE_SYSTEM_START",
        "SERVICE_AUTO_START",
        "SERVICE_DEMAND_START",
        "SERVICE_DISABLED"
    };
    startTypeBox->addItems(commands);
    connect(this->startTypeBox, SIGNAL(currentIndexChanged(int)), SLOT(changeStartType(int)));

    this->startBtn = new QPushButton();
    this->startBtn->setText("Start");
    connect(this->startBtn, SIGNAL(clicked()), this, SLOT(startService()));

    this->stopBtn = new QPushButton();
    this->stopBtn->setText("Stop");
    connect(this->stopBtn, SIGNAL(clicked()), this, SLOT(stopService()));

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
    delete startTypeBox;
    delete startBtn;
    delete stopBtn;
    delete serviceName;
    delete displayName; 
    delete description; 
    delete startType;
    delete currentState;
}


void GeneralSettingsTab::startService() 
{
    qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->startService();
}

void GeneralSettingsTab::stopService() {
    //return;
    //this->serviceSettingsTab->stopService();
    /*QMessageBox::question(this, "Test", "Quit?",
        QMessageBox::Yes | QMessageBox::No);*/
    
    qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->stopService();
}

void GeneralSettingsTab::changeStartType(int newIndex) 
{   
    qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->applySettingsBtn->setEnabled(true);
    qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceConfig.dwStartType = newIndex;
}


void GeneralSettingsTab::updateInfo() {

    if (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceStatus.dwCurrentState == SERVICE_RUNNING) 
    {
        this->startBtn->setDisabled(true);
    }

    if (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceStatus.dwCurrentState == SERVICE_STOPPED) 
    {
        this->stopBtn->setDisabled(true);
    }

    this->startTypeBox->setCurrentIndex(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceConfig.dwStartType);

    this->serviceName->setText(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceName);
    this->displayName->setText(QString::fromWCharArray(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceConfig.lpDisplayName));
    this->description->setText(QString::fromWCharArray(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceDescription.lpDescription));
    
    this->pathName->setText(QString::fromWCharArray(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceConfig.lpBinaryPathName));

    this->currentState->setText(currentStateToString(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceStatus.dwCurrentState));
}
