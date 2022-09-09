#pragma once
#include "qwidget.h"
#include "qcombobox.h"
#include "qboxlayout.h"

#include "service_settings_tab.h"


class GeneralSettingsTab : public QWidget
{
    Q_OBJECT

public:
    GeneralSettingsTab(QWidget* parent);
    void updateInfo();

private slots:
    void changeStartType(int newIndex);

    void startService();
    void stopService();


private:
    
    QPushButton* startBtn;
    QPushButton* stopBtn;

    QComboBox* startTypeBox;
    QWidget* serviceSettingsTab;

};