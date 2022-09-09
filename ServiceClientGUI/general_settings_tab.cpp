#include "general_settings_tab.h"


GeneralSettingsTab::GeneralSettingsTab(QWidget* parent)
    : QWidget(parent)
{
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

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(startTypeBox);
    mainLayout->addWidget(startBtn);
    mainLayout->addWidget(stopBtn);


    mainLayout->addStretch(1);
    setLayout(mainLayout);
    setWindowTitle(tr("Tab Dialog"));
    this->setFixedSize(640, 480);
    this->updateInfo();
}


void GeneralSettingsTab::startService() {
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

    if (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceStatus.dwCurrentState == SERVICE_RUNNING) {
        this->startBtn->setDisabled(true);
    }

    if (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceStatus.dwCurrentState == SERVICE_STOPPED) {
        this->stopBtn->setDisabled(true); 
    }

    this->startTypeBox->setCurrentIndex(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceConfig.dwStartType);
}