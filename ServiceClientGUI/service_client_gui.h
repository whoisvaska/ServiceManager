#pragma once

#include <QtWidgets/QMainWindow>
#include <iostream>
#include <QTreeView>
#include <qtreewidget.h>
#include <QStandardItemModel>
#include <qmessagebox.h>
#include <qlist.h>
#include <qshortcut.h>

#include "qsizepolicy.h"

#include "service_settings_tab.h"
//#include "service_settings_wnd.h"
//#include "ui_serviceclientgui.h"

//#include "../ServiceManager/server_type_info_i.c"
//#include "../ServiceManager/server_type_info_h.h"

class ServiceSettingsTab;
class DependentServicesTab;
class GeneralSettingsTab;
class RecoverySettingsTab;


class ServiceClientGUI : public QMainWindow
{
    Q_OBJECT

public:
    ServiceClientGUI(QWidget *parent = nullptr);
    ~ServiceClientGUI();

private slots:
    void ServiceClientGUI::settingsMenuRequested(const QModelIndex& index);
    void updateServices();

private:
    void initInterfaces();

    IClassFactory* pICF;
    ICreateManager* pICreateManager;
    IManager* pIManager;

    QMap<QString, QList<QString>> dependentServices;

    QTreeView* servicesTreeView;
    QStandardItemModel* servicesModel;

    ServiceSettingsTab* serviceSettingsTab;

    SAFEARRAY* servicesStatus;
};
