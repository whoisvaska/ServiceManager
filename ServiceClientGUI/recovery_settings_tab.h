#pragma once


#pragma once
#include "qwidget.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qlineedit.h"

#include "service_settings_tab.h"


class RecoverySettingsTab : public QWidget
{
    Q_OBJECT

public:
    RecoverySettingsTab(QWidget* parent);
    void updateInfo();

private slots:
    void changeFailureAction(int actionNumber, int newAction);
    void changeResetPeriod(const QString& period);
    void changeRunProgram(const QString& program);

private:
    QComboBox* failure1actionBox;
    QComboBox* failure2actionBox;
    QComboBox* failure3actionBox;

    QLineEdit* resetPeriodLineEdit;
    QLineEdit* runProgramLineEdit;

    QWidget* serviceSettingsTab;

};