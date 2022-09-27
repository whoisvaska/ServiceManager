#include "recovery_settings_tab.h"

RecoverySettingsTab::RecoverySettingsTab(QWidget* parent)
    : QWidget(parent)
{

    this->serviceSettingsTab = (parent);


    this->failure1actionBox = new QComboBox();
    this->failure2actionBox = new QComboBox();
    this->failure3actionBox = new QComboBox();

    this->rebootMsgTextEdit = new QPlainTextEdit();
    connect(rebootMsgTextEdit, &QPlainTextEdit::textChanged, [this]()
        {
            (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        });


    QStringList commands = {"SC_ACTION_NONE",
        "SC_ACTION_RESTART",
        "SC_ACTION_REBOOT",
        "SC_ACTION_RUN_COMMAND"
    };


    failure1actionBox->addItems(commands);
    failure2actionBox->addItems(commands);
    failure3actionBox->addItems(commands);

    connect(failure1actionBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) 
        {
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeFailureAction(1, index);
        });
    connect(failure2actionBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) 
        {
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeFailureAction(2, index);
        });
    connect(failure3actionBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) 
        {
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeFailureAction(3, index);
        });


    this->failure1actionDelay = new QLineEdit();
    this->failure2actionDelay = new QLineEdit();
    this->failure3actionDelay = new QLineEdit();


    connect(failure1actionDelay, &QLineEdit::textChanged, [this](QString delay) 
        {
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeFailureActionDelay(1, delay.toInt());
        });
    connect(failure2actionDelay, &QLineEdit::textChanged, [this](QString delay) 
        {
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeFailureActionDelay(2, delay.toInt());
        });
    connect(failure3actionDelay, &QLineEdit::textChanged, [this](QString delay) 
        {
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeFailureActionDelay(3, delay.toInt());
        });
    
    this->failureActionOnNonCrashCheckBox = new QCheckBox();
    connect(failureActionOnNonCrashCheckBox, &QCheckBox::stateChanged, [this](int check)
        {
            (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
            (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeFailureActionOnNonCrashFlag(check);
        }
    );

    resetPeriodLineEdit = new QLineEdit;
    resetPeriodLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+")));
    connect(resetPeriodLineEdit, &QLineEdit::textChanged, [this](const QString& period)
        {
            (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
            (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->changeResetPeriod(period.toInt());
        });


    runProgramLineEdit = new QLineEdit;
    connect(runProgramLineEdit, &QLineEdit::textChanged, [this]()
        {
            (qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab))->applySettingsBtnCheck(true);
        }
    );

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
    QLabel* failureOnNonCrashLabel = new QLabel("Enable actions for stops with errors");
    QLabel* runProgramLabel = new QLabel("Run program:");
    QLabel* rebootMsgLabel = new QLabel("Before restart, send this message to computers on the network:");

    layout->addWidget(resetPerionLabel, 3, 0);
    layout->addWidget(resetPeriodLineEdit, 3, 1);

    layout->addWidget(failureOnNonCrashLabel, 4, 0);
    layout->addWidget(failureActionOnNonCrashCheckBox, 4, 1);

    layout->addWidget(runProgramLabel, 5, 0);
    layout->addWidget(runProgramLineEdit, 5, 1);

    layout->addWidget(rebootMsgLabel, 6, 0);
    layout->addWidget(rebootMsgTextEdit, 6, 1);

 
    setLayout(layout);
    setWindowTitle(tr("Tab Dialog"));
    this->updateInfo();
}


RecoverySettingsTab::~RecoverySettingsTab()
{
    if (this->failure1actionBox != NULL) delete (this->failure1actionBox);
    if (this->failure2actionBox != NULL) delete (this->failure2actionBox);
    if (this->failure3actionBox != NULL) delete (this->failure3actionBox);

    if (this->failureActionOnNonCrashCheckBox != NULL) delete (this->failureActionOnNonCrashCheckBox);
    if (this->resetPeriodLineEdit != NULL) delete (this->resetPeriodLineEdit);
    if (this->runProgramLineEdit != NULL) delete (this->runProgramLineEdit);
    if (this->rebootMsgTextEdit != NULL) delete (this->rebootMsgTextEdit);
    if (this->failure1actionDelay != NULL) delete (this->failure1actionDelay);
    if (this->failure2actionDelay != NULL) delete (this->failure2actionDelay);
    if (this->failure3actionDelay != NULL) delete (this->failure3actionDelay);

    //QMessageBox::information(this, "rec", "");

}


QString RecoverySettingsTab::getRebootMessageText()
{
    return this->rebootMsgTextEdit->toPlainText();
}

QString RecoverySettingsTab::getRunProgramText()
{
    return this->runProgramLineEdit->text();
}


void RecoverySettingsTab::updateInfo() 
{
    const SERVICE_FAILURE_ACTIONS_UDT* serviceFailureActions = qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->getServiceFailureActions();

    this->failure1actionBox->setCurrentIndex(serviceFailureActions->lpsaAction1.Type);
    this->failure2actionBox->setCurrentIndex(serviceFailureActions->lpsaAction2.Type);
    this->failure3actionBox->setCurrentIndex(serviceFailureActions->lpsaAction3.Type);

    this->failure1actionDelay->setText(QString::number(serviceFailureActions->lpsaAction1.Delay));
    this->failure2actionDelay->setText(QString::number(serviceFailureActions->lpsaAction2.Delay));
    this->failure3actionDelay->setText(QString::number(serviceFailureActions->lpsaAction3.Delay));

    this->failureActionOnNonCrashCheckBox->setChecked(serviceFailureActions->fFailureActionsOnNonCrashFailures);

    this->resetPeriodLineEdit->setText(QString::number(serviceFailureActions->dwResetPeriod));


    this->rebootMsgTextEdit->setPlainText(QString::fromWCharArray(serviceFailureActions->lpRebootMsg));

    this->runProgramLineEdit->setText(QString::fromWCharArray(serviceFailureActions->lpCommand));
}