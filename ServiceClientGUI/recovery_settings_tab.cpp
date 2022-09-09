#include "recovery_settings_tab.h"

RecoverySettingsTab::RecoverySettingsTab(QWidget* parent)
    : QWidget(parent)
{
    this->serviceSettingsTab = parent;

    this->failure1actionBox = new QComboBox();
    this->failure2actionBox = new QComboBox();
    this->failure3actionBox = new QComboBox();

    QStringList commands = {"SC_ACTION_NONE",
        "SC_ACTION_RESTART",
        "SC_ACTION_REBOOT",
        "SC_ACTION_RUN_COMMAND"
    };

    failure1actionBox->addItems(commands);
    failure2actionBox->addItems(commands);
    failure3actionBox->addItems(commands);

    //connect(failure1actionBox, &SIGNAL(currentIndexChanged(int)), [this, failure1actionBox]
    connect(failure1actionBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
        changeFailureAction(1, index);
        });
    connect(failure2actionBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
        changeFailureAction(2, index);
        });
    connect(failure3actionBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
        changeFailureAction(3, index);
        });

    resetPeriodLineEdit = new QLineEdit;
    resetPeriodLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
    connect(resetPeriodLineEdit, &QLineEdit::textChanged, this, &RecoverySettingsTab::changeResetPeriod);


    runProgramLineEdit = new QLineEdit;
    connect(runProgramLineEdit, &QLineEdit::textChanged, this, &RecoverySettingsTab::changeRunProgram);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    mainLayout->addWidget(failure1actionBox);
    mainLayout->addWidget(failure2actionBox);
    mainLayout->addWidget(failure3actionBox);

    mainLayout->addWidget(resetPeriodLineEdit);

    mainLayout->addWidget(runProgramLineEdit);


    mainLayout->addStretch(1);
    setLayout(mainLayout);
    setWindowTitle(tr("Tab Dialog"));
    this->setFixedSize(640, 480);
    
    this->updateInfo();
}



void RecoverySettingsTab::changeResetPeriod(const QString& period) {
    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtn->setEnabled(true);
    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.dwResetPeriod = period.toInt();
}


void RecoverySettingsTab::changeRunProgram(const QString& program) {

    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtn->setEnabled(true);
    
    if ((qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpCommand != NULL) {
        SysFreeString(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpCommand);
    }

    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpCommand = SysAllocString(program.toStdWString().c_str());

}


void RecoverySettingsTab::changeFailureAction(int actionNumber, int newAction) {
    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtn->setEnabled(true);

    switch (actionNumber) {
    case 1:
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpsaAction1.Type = newAction;
        break;
    case 2:
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpsaAction2.Type = newAction;
        break;
    case 3:
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpsaAction3.Type = newAction;
        break;
    }
}


void RecoverySettingsTab::updateInfo() {
  
    this->failure1actionBox->setCurrentIndex(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpsaAction1.Type);
    this->failure2actionBox->setCurrentIndex(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpsaAction2.Type);
    this->failure3actionBox->setCurrentIndex(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpsaAction3.Type);

    this->resetPeriodLineEdit->setText(QString::number(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.dwResetPeriod));

    this->runProgramLineEdit->setText(QString::fromWCharArray(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpCommand));
}