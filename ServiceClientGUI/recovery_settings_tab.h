#pragma once

#include "qwidget.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qlabel.h"

#include "service_settings_tab.h"

class RecoverySettingsTab : public QWidget
{
    Q_OBJECT

public:
    RecoverySettingsTab(QWidget* parent);
    void updateInfo();

private slots:
    void changeFailureAction(int actionNumber, int newAction);
    void changeFailureActionDelay(int actionNumber, const QString& delay);

    void changeResetPeriod(const QString& period);
    void changeRunProgram(const QString& program);
private:
    QComboBox* failure1actionBox;
    QComboBox* failure2actionBox;
    QComboBox* failure3actionBox;

    QLineEdit* resetPeriodLineEdit;
    QLineEdit* runProgramLineEdit;

    QLineEdit* failure1actionDelay;
    QLineEdit* failure2actionDelay;
    QLineEdit* failure3actionDelay;

    QWidget* serviceSettingsTab;
};