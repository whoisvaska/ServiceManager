#pragma once
#include <qmainwindow.h>
#include "qpushbutton.h"
#include <qlayout.h>
#include <qmessagebox.h>
#include <qcombobox.h>

#include "../ServiceManager/server_type_info_h.h"
#include "../ServiceManager/server_type_info_i.c"


//#include "service_client_gui.h"
//#include "ui"

class ServiceSettingsWnd : public QMainWindow 
{
    Q_OBJECT

public:
    ServiceSettingsWnd(QString serviceName, IManager* pIManager_, QWidget* parent = nullptr);
    //~ServiceSettingsWnd();



private slots:
    void stopService();
    void startService();
    void changeStartType(int);
    void setServiceConfig();

private:

    void updateServiceInfo();

    QPushButton* applySettingsBtn;
    QPushButton* okSettingsBtn;
    QPushButton* closeSettingsBtn;

    QPushButton* startServiceBtn;
    QPushButton* stopServiceBtn;

    QComboBox* startTypeBox;

    QTabWidget* tabWidget;

    QString serviceName;

    IManager* pIManager;

    bool isSettingsChanged;

    SERVICE_STATUS_PROCESS_UDT serviceStatus;
    QUERY_SERVICE_CONFIG_UDT serviceConfig;
};
