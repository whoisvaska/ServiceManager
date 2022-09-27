#pragma once
#include "qwidget.h"
#include "qcombobox.h"
#include "qboxlayout.h"

#include "service_settings_tab.h"


class GeneralSettingsTab : public QWidget
{
    Q_OBJECT

public:
    GeneralSettingsTab(QWidget* parent, QString serviceName);
    ~GeneralSettingsTab();
    void updateInfo();

private slots:

private:
    QString serviceNameString;

    QLabel* serviceName;
    QLabel* displayName;
    QLabel* startType;
    QLabel* currentState;
    QLabel* description;
    QLabel* pathName;



    QPushButton* startBtn;
    QPushButton* stopBtn;

    QComboBox* startTypeBox;
    QWidget* serviceSettingsTab;

};