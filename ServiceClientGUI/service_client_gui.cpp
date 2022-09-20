#include "service_client_gui.h"
#include "value_to_str.h"

ServiceClientGUI::ServiceClientGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QWidget* widget = new QWidget;
    setCentralWidget(widget);


    static const QStringList HEADERS = QStringList()
        << "ServiceName"
        << "DisplayName"
        << "ServiceType"
        << "CurrentState"
        << "ControlsAccepted"
        << "Win32ExitCode"
        << "ServiceSpecificExitCode"
        << "CheckPoint"
        << "WaitHint";

    this->servicesModel = new QStandardItemModel(this);

    this->servicesModel->setHorizontalHeaderLabels(HEADERS);

    this->initInterfaces();

    this->updateServices();

    QTreeWidget* treeWidget = new QTreeWidget();


    //QBoxLayout* x = new QBoxLayout(treeWidget);

    QHBoxLayout* layout = new QHBoxLayout;



    this->servicesTreeView = new QTreeView(this);
    
    layout->addWidget(servicesTreeView);


    this->servicesTreeView->setModel(this->servicesModel);
    this->servicesTreeView->setSortingEnabled(true);
    //servicesTreeView->setMinimumSize(QSize(1, 1));
    //this->servicesTreeView->setGeometry(QRect(QPoint(0, 50), QSize(1024, 718)));
    this->servicesTreeView->setContextMenuPolicy(Qt::CustomContextMenu);


    this->servicesTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->servicesTreeView->installEventFilter(this);

    this->servicesTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this->servicesTreeView, &QTreeView::doubleClicked, this, &ServiceClientGUI::settingsMenuRequested);

    widget->setLayout(layout);
    //this->setFixedSize(1024, 768);
}



void ServiceClientGUI::settingsMenuRequested(const QModelIndex& index) 
{ 
    QString serviceName = index.siblingAtColumn(1).data().toString();

    this->serviceSettingsTab = new ServiceSettingsTab(serviceName, this->pIManager, dependentServices, this);
    this->serviceSettingsTab->show();

}


void ServiceClientGUI::updateServices() {
    this->servicesModel->removeRows(0, this->servicesModel->rowCount());


    int servicesCount = 0;
    
    HRESULT hr = S_OK;

    SAFEARRAY* psaServiceName;

    hr = this->pIManager->getServices(&servicesCount, &psaServiceName, &servicesStatus);
    
    ENUM_SERVICE_STATUS_PROCESS_UDT* pServiceStatus = NULL;



    hr = SafeArrayAccessData(this->servicesStatus, (void**)&pServiceStatus);

    for (int i = 0; i < servicesCount; ++i)
    {
        QList<QStandardItem*> serviceItem;


        QString displayName = QString::fromWCharArray(pServiceStatus[i].lpDisplayName);
        serviceItem.append(new QStandardItem(displayName));

        SAFEARRAY* psaDependentServices;
        int dependentServicesCount = 0;
        BSTR* pDependentServices = NULL;

        hr = this->pIManager->enumDependentServicesSM(&dependentServicesCount, pServiceStatus[i].lpServiceName, &psaDependentServices);

        hr = SafeArrayAccessData(psaDependentServices, (void**)&pDependentServices);

        
        for (int j = 0; j < dependentServicesCount; ++j)
        {
            QString dependentServiceName = QString::fromWCharArray (pDependentServices[j]);
            //QMessageBox::information(this, displayName, dependentServiceName);

            this->dependentServices[displayName].push_back(dependentServiceName);
        }
        hr = SafeArrayUnaccessData(psaDependentServices);
        //Safearra
        //SafeArrayDestroy(psaDependentServices);

        QString serviceName = QString::fromWCharArray(pServiceStatus[i].lpServiceName);
        serviceItem.append(new QStandardItem(serviceName));

        QString serviceType = serviceTypeToString(pServiceStatus[i].ServiceStatus.dwServiceType) +
            QString("/") +
            QString::number(pServiceStatus[i].ServiceStatus.dwServiceType);
        serviceItem.append(new QStandardItem(serviceType));
     
        QString currentState = currentState = currentStateToString(pServiceStatus[i].ServiceStatus.dwCurrentState) +
            QString("/") +
            QString::number(pServiceStatus[i].ServiceStatus.dwCurrentState);
        serviceItem.append(new QStandardItem(currentState));

        QString controlsAccepted = QString::number(pServiceStatus[i].ServiceStatus.dwControlsAccepted);
        serviceItem.append(new QStandardItem(controlsAccepted));

        QString win32ExitCode = QString::number(pServiceStatus[i].ServiceStatus.dwWin32ExitCode);
        serviceItem.append(new QStandardItem(win32ExitCode));

        QString serviceSpecificExitCode = QString::number(pServiceStatus[i].ServiceStatus.dwServiceSpecificExitCode);
        serviceItem.append(new QStandardItem(serviceSpecificExitCode));

        QString checkPoint = QString::number(pServiceStatus[i].ServiceStatus.dwCheckPoint);
        serviceItem.append(new QStandardItem(checkPoint));

        QString waitHint = QString::number(pServiceStatus[i].ServiceStatus.dwWaitHint);
        serviceItem.append(new QStandardItem(waitHint));

        this->servicesModel->appendRow((serviceItem));
    }

    hr = SafeArrayUnaccessData(this->servicesStatus);
}



void ServiceClientGUI::initInterfaces() {

    HRESULT hr = S_OK;
    hr = CoGetClassObject(CLSID_ServiceManager, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&this->pICF);

    if (FAILED(hr))
    {
        std::cout << "CoGetClassObject() error!" << std::endl;
        exit(1);
    }
    std::cout << "CoGetClassObject() Success." << std::endl;

    std::cout << "pICF->CreateInstance()";
    hr = pICF->CreateInstance(NULL, IID_ICreateManager, (void**)&this->pICreateManager);
    if (FAILED(hr))
    {
        std::cout << "pICF->CreateInstance() error!" << std::endl;
        exit(1);
    }
    std::cout << "pICF->CreateInstance() Success." << std::endl;


    std::cout << "pICreateManager->QueryInterface()" << std::endl;
    hr = this->pICreateManager->QueryInterface(IID_IManager, (void**)&pIManager);
    if (FAILED(hr))
    {
        std::cout << "pICreateManager->QueryInterface() error!" << std::endl;
        exit(1);
    }
    std::cout << "pICreateManager->QueryInterface() Success." << std::endl;

}


ServiceClientGUI::~ServiceClientGUI()
{
    if (this->pICF) {
        this->pICF->Release();

    }
    if (this->pICreateManager) {
        this->pICreateManager->Release();
    }
    if (this->pIManager) {
        this->pIManager->Release();
    }

}
