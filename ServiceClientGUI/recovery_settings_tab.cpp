#include "recovery_settings_tab.h"

RecoverySettingsTab::RecoverySettingsTab(QWidget* parent)
    : QWidget(parent)
{

    //QMessageBox::information(this, "", "RecoverySettingsTab");

    this->serviceSettingsTab = (parent);


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


    this->failure1actionDelay = new QLineEdit();
    this->failure2actionDelay = new QLineEdit();
    this->failure3actionDelay = new QLineEdit();


    connect(failure1actionDelay, &QLineEdit::textChanged, [this](QString delay) {
        changeFailureActionDelay(1, delay);
        });
    connect(failure2actionDelay, &QLineEdit::textChanged, [this](QString delay) {
        changeFailureActionDelay(2, delay);
        });
    connect(failure3actionDelay, &QLineEdit::textChanged, [this](QString delay) {
        changeFailureActionDelay(3, delay);
        });
    

    resetPeriodLineEdit = new QLineEdit;
    resetPeriodLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
    connect(resetPeriodLineEdit, &QLineEdit::textChanged, this, &RecoverySettingsTab::changeResetPeriod);


    runProgramLineEdit = new QLineEdit;
    connect(runProgramLineEdit, &QLineEdit::textChanged, this, &RecoverySettingsTab::changeRunProgram);

    QGridLayout* layout = new QGridLayout();
    
    QLabel* failure1label = new QLabel("First failrure:");
    QLabel* failure2label = new QLabel("Second failrure:");
    QLabel* failure3label = new QLabel("Subsequent failures:");
    QLabel* failureActionDelayLabel = new QLabel("Delay in ms:");

    layout->addWidget(failure1label, 0, 0);
    layout->addWidget(failure2label, 1, 0);
    layout->addWidget(failure3label, 2, 0);

    layout->addWidget(failure1actionBox, 0, 1);
    layout->addWidget(failure2actionBox, 1, 1);
    layout->addWidget(failure3actionBox, 2, 1);

    layout->addWidget(failureActionDelayLabel, 0, 2);
    layout->addWidget(failureActionDelayLabel, 1, 2);
    layout->addWidget(failureActionDelayLabel, 2, 2);

    layout->addWidget(failure1actionDelay, 0, 3);
    layout->addWidget(failure2actionDelay, 1, 3);
    layout->addWidget(failure3actionDelay, 2, 3);

    QLabel* resetPerionLabel = new QLabel("Reset fail count after (seconds):");
    QLabel* runProgramLabel = new QLabel("Run program:");

    layout->addWidget(resetPerionLabel, 3, 0);
    layout->addWidget(runProgramLabel, 4, 0);

    layout->addWidget(resetPeriodLineEdit, 3, 1);
    layout->addWidget(runProgramLineEdit, 4, 1);

  /*  QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addWidget(failure1actionBox);
    mainLayout->addWidget(failure2actionBox);
    mainLayout->addWidget(failure3actionBox);
    mainLayout->addWidget(resetPeriodLineEdit);
    mainLayout->addWidget(runProgramLineEdit);


    mainLayout->addStretch(1);
    setLayout(mainLayout);*/
    setLayout(layout);
    setWindowTitle(tr("Tab Dialog"));
    //this->setFixedSize(640, 480);
    
    this->updateInfo();
}



void RecoverySettingsTab::changeResetPeriod(const QString& period) {
    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtn->setEnabled(true);
    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.dwResetPeriod = period.toInt();
}


void RecoverySettingsTab::changeFailureActionDelay(int actionNumber, const QString& delay) {
    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtn->setEnabled(true);

    (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.cActions = 3;
    switch (actionNumber) {
    case 1:
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpsaAction1.Delay = delay.toInt();
        break;
    case 2:
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpsaAction2.Delay = delay.toInt();
        break;
    case 3:
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->serviceFailureActions.lpsaAction3.Delay = delay.toInt();
        break;
    }
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

    this->failure1actionDelay->setText(QString::number(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpsaAction1.Delay));
    this->failure2actionDelay->setText(QString::number(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpsaAction2.Delay));
    this->failure3actionDelay->setText(QString::number(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpsaAction3.Delay));


    this->resetPeriodLineEdit->setText(QString::number(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.dwResetPeriod));




    this->runProgramLineEdit->setText(QString::fromWCharArray(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceFailureActions.lpCommand));
}