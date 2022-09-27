#include "dependent_services_tab.h"

DependentServicesTab::DependentServicesTab(QWidget* parent, QMap<QString, QList<QString>> dependentServices_)
    : QWidget(parent)
{

    this->serviceSettingsTab = (parent);
    this->dependentServices = dependentServices_;

    this->dependOnServicesTreeWidget = new QTreeWidget();
    dependOnServicesTreeWidget->setHeaderLabel(QString("This service depends on the following system components"));
    connect(this->dependOnServicesTreeWidget, SIGNAL(clicked(const QModelIndex&)), this, SLOT(expandDependOn(const QModelIndex&)));

    this->dependedServicesTreeWidget = new QTreeWidget();
    dependedServicesTreeWidget->setHeaderLabel(QString("The following system components depend on this service"));

    connect(this->dependedServicesTreeWidget, SIGNAL(clicked(const QModelIndex&)), this, SLOT(expandDependent(const QModelIndex&)));
    
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(this->dependOnServicesTreeWidget, 0, 1);
        gridLayout->addWidget(this->dependedServicesTreeWidget, 1, 1);
    setLayout(gridLayout);

    this->updateInfo();
}



DependentServicesTab::~DependentServicesTab()
{
    if (this->dependedServicesTreeWidget != NULL) delete (this->dependedServicesTreeWidget);
    if (this->dependOnServicesTreeWidget != NULL) delete (this->dependOnServicesTreeWidget);
}


void DependentServicesTab::expandDependent(const QModelIndex& index)
{   
    QTreeWidgetItem* treeItem = dependedServicesTreeWidget->currentItem();

    if (treeItem->isExpanded() == true)
    {
        treeItem->setExpanded(false);
        return;
    }

    QString displayName = treeItem->text(0);

    ServiceSettingsTab* sst = qobject_cast<ServiceSettingsTab*>(serviceSettingsTab);

    QList<QString> dependentServices = this->dependentServices[displayName];

    for (auto v : dependentServices)
    {
        QTreeWidgetItem* twi = new QTreeWidgetItem();
    
        twi->setText(0, v);

        treeItem->addChild(twi);
    }

    treeItem->setExpanded(true);
}


void DependentServicesTab::expandDependOn(const QModelIndex& index)
{
    QTreeWidgetItem* treeItem = dependOnServicesTreeWidget->currentItem();

    if (treeItem->isExpanded() == true)
    {
        treeItem->setExpanded(false);
        return;
    }

    if (treeItem->childCount() > 0) {
        return;
    }

    QString displayName = treeItem->text(0);

    ServiceSettingsTab* sst = qobject_cast<ServiceSettingsTab*>(serviceSettingsTab);

    QList<QString> dependOnServices;

    QMap<QString, QList<QString>>::iterator it;

    for (it = this->dependentServices.begin(); it != this->dependentServices.end(); ++it)
    {
        if (it.value().indexOf(displayName) >= 0)
        {
            dependOnServices.push_back(it.key());
        }
    }


    for (auto v : dependOnServices)
    {
        QTreeWidgetItem* twi = new QTreeWidgetItem();

        twi->setText(0, v);

        treeItem->addChild(twi);
    }

    treeItem->setExpanded(true);
}

void DependentServicesTab::updateInfo()
{
    const QUERY_SERVICE_CONFIG_UDT* serviceConfig = qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->getServiceConfig();

    QString displayName = QString::fromWCharArray(serviceConfig->lpDisplayName);
    
    QList<QString> dependentServices = this->dependentServices[displayName];

    for (auto v : dependentServices)
    {
        QTreeWidgetItem* twi = new QTreeWidgetItem(dependedServicesTreeWidget);
        twi->setText(0, v);
    }

    QList<QString> dependOnServices;

    QMap<QString, QList<QString>>::iterator it;

    for (it = this->dependentServices.begin(); it != this->dependentServices.end(); ++it)
    {
        if (it.value().indexOf(displayName) >= 0)
        {
            dependOnServices.push_back(it.key());
        }
    }

    for (auto v : dependOnServices)
    {

        QTreeWidgetItem* twi = new QTreeWidgetItem(dependOnServicesTreeWidget);

        twi->setText(0, v);

    }
}


