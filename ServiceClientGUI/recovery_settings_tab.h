#pragma once

#include "qwidget.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qlabel.h"
#include "qcheckbox.h"
#include "qtextedit.h"
#include "qplaintextedit.h"


#include "service_settings_tab.h"

class RecoverySettingsTab : public QWidget
{
    Q_OBJECT

public:
    RecoverySettingsTab(QWidget* parent);
    ~RecoverySettingsTab();

    void updateInfo();
    QString getRunProgramText();
    QString getRebootMessageText();

private slots:

private:
    QComboBox* failure1actionBox;
    QComboBox* failure2actionBox;
    QComboBox* failure3actionBox;

    QCheckBox* failureActionOnNonCrashCheckBox;

    QLineEdit* resetPeriodLineEdit;
    QLineEdit* runProgramLineEdit;

    //QTextEdit* rebootMsgTextEdit;
    QPlainTextEdit* rebootMsgTextEdit;

    QLineEdit* failure1actionDelay;
    QLineEdit* failure2actionDelay;
    QLineEdit* failure3actionDelay;

    QWidget* serviceSettingsTab;
};