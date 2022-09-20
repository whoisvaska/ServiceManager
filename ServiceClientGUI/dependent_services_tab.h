#pragma once

#include "qwidget.h"
#include "qcombobox.h"
#include "qboxlayout.h"
#include "qtreeview.h"
#include "qtreewidget.h"

#include "service_settings_tab.h"


class DependentServicesTab : public QWidget
{
    Q_OBJECT

public:
    DependentServicesTab(QWidget* parent);

    void updateInfo();

private slots:
    void expandDependent(const QModelIndex&);
    void expandDependOn(const QModelIndex&);

private:
    //QWidget* serviceSettingsTab;
    QTreeWidget* dependedServicesTreeWidget;
    QTreeWidget* dependOnServicesTreeWidget;

    QWidget* serviceSettingsTab;
};