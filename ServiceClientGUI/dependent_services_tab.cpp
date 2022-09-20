#include "dependent_services_tab.h"

DependentServicesTab::DependentServicesTab(QWidget* parent)
    : QWidget(parent)
{

    //QMessageBox::information(this, "", "DependentServicesTab");

    this->serviceSettingsTab = (parent);


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


void DependentServicesTab::expandDependent(const QModelIndex& index)
{   
    //index.column();
    //QTreeWidgetItem* treeItem = this->dependedServicesTreeWidget->findItems(index.data().toString(), Qt::MatchExactly)[0];
    
    QTreeWidgetItem* treeItem = dependedServicesTreeWidget->currentItem();

    //QMessageBox::information(this, "", (dependedServicesTreeWidget->currentItem()->text(0)));

    
    if (treeItem->isExpanded() == true)
    {
        treeItem->setExpanded(false);
        return;
    }

    QString displayName = treeItem->text(0);

    ServiceSettingsTab* sst = qobject_cast<ServiceSettingsTab*>(serviceSettingsTab);

    QList<QString> dependentServices = sst->dependentServices[displayName];

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
    //index.column();
    //QTreeWidgetItem* treeItem = this->dependedServicesTreeWidget->findItems(index.data().toString(), Qt::MatchExactly)[0];

    QTreeWidgetItem* treeItem = dependOnServicesTreeWidget->currentItem();

    //QMessageBox::information(this, "", (dependedServicesTreeWidget->currentItem()->text(0)));


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

    for (it = sst->dependentServices.begin(); it != sst->dependentServices.end(); ++it)
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
    BSTR szServiceName = SysAllocString(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceName.toStdWString().c_str());
    QString displayName = QString::fromWCharArray(qobject_cast<ServiceSettingsTab*>(this->serviceSettingsTab)->serviceConfig.lpDisplayName);
    //serviceSettingsTab->serviceClientGUI.

    //QMessageBox::information(this, "aaaa", displayName);

    ServiceSettingsTab* sst = qobject_cast<ServiceSettingsTab*>(serviceSettingsTab);
    //ServiceClientGUI* scg = qobject_cast<ServiceClientGUI*>(sst->serviceClientGUI);

    QList<QString> dependentServices = sst->dependentServices[displayName];

    for (auto v : dependentServices)
    {
        QTreeWidgetItem* twi = new QTreeWidgetItem(dependedServicesTreeWidget);

        //QMessageBox::information(this, "", v);

        twi->setText(0, v);
        
    }


    QList<QString> dependOnServices;

    QMap<QString, QList<QString>>::iterator it;

    for (it = sst->dependentServices.begin(); it != sst->dependentServices.end(); ++it)
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


