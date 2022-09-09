#include "../ServiceManager/server_type_info_h.h"
#include "../ServiceManager/server_type_info_i.c"


#include <iostream>

// for showing possible mistakes
void ShowErrorMessage(LPCTSTR, HRESULT);

int main()
{
    HRESULT hr = S_OK;
    
    
    std::cout << "CoInitialize()" << std::endl;
    hr = CoInitialize(NULL);

    if (FAILED(hr)) {
        ShowErrorMessage("CoInitialize()", hr);
    }
    std::cout << "CoInitialize() Success." << std::endl;

    IClassFactory* pICF = NULL;
    ICreateManager* pICreateManager = NULL;
    IManager* pIManager = NULL;

    std::cout << "CoGetClassObject()" << std::endl;
    hr = CoGetClassObject(CLSID_ServiceManager, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&pICF);
    if (FAILED(hr))
    {
        ShowErrorMessage("CoGetClassObject()", hr);
        exit(1);
    }
    std::cout << "CoGetClassObject() Success." << std::endl;

    std::cout << "pICF->CreateInstance()";
    hr = pICF->CreateInstance(NULL, IID_ICreateManager, (void**)&pICreateManager);
    if (FAILED(hr))
    {
        ShowErrorMessage("CreateInstance()", hr);
        exit(1);
    }
    std::cout << "pICF->CreateInstance() Success." << std::endl;


    std::cout << "pICreateManager->QueryInterface()" << std::endl;
    hr = pICreateManager->QueryInterface(IID_IManager, (void**)&pIManager);
    if (FAILED(hr))
    {
        ShowErrorMessage("QueryInterface()", hr);
        exit(1);
    }
    std::cout << "pICreateManager->QueryInterface() Success." << std::endl;

    
    //LoadRegTypeLib()

    int servicesCount = 0;

    //SAFEARRAY* psaServiceInfo;




    std::cout << "servicesCount = " << servicesCount << ", psaServiceInfo = NULL" << std::endl;

    
    SAFEARRAY* psaServiceName;
    SAFEARRAY* psaServiceStatus;



    std::cout << "pIManager->enumerateServices()" << std::endl;
    hr = pIManager->getServices(&servicesCount, &psaServiceName, &psaServiceStatus);
    if (FAILED(hr))
    {
        ShowErrorMessage("enumerateServices()", hr);
        exit(1);
    }
    std::cout << "pIManager->enumerateServices() Success." << std::endl;
    std::cout << "servicesCount = " << servicesCount << ", psaServiceInfo = NULL" << std::endl;



    long int k = 0;
    //EnumServiceStatusUDT* pServiceStatus;
   

    ENUM_SERVICE_STATUS_PROCESS_UDT* pServiceStatus = NULL;
    hr = SafeArrayAccessData(psaServiceStatus, (void**)&pServiceStatus);


    long int i = 0;

    for (int j = 0; j < servicesCount; ++j) {
        BSTR s = NULL;
        SafeArrayGetElement(psaServiceName, &i, &s);       //std::wcout << "ServiceName: " << reinterpret_cast<BSTR>(services[i].pvData) << std::endl;
        wprintf(L"sname = %s\n", s);
        //SafeArrayGetElement(psaServiceStatus, &k, pServiceStatus);       //std::wcout << "ServiceName: " << reinterpret_cast<BSTR>(services[i].pvData) << std::endl;
        //std::cout << "kkkk " << pServiceStatus[k].lpServiceName << std::endl;
        wprintf(L"%d = %s\n",i, pServiceStatus[k].lpServiceName);
        wprintf(L"status = %d\n", pServiceStatus[k].ServiceStatus.dwCurrentState);


        ++i;
        ++k;
    }

     //BSTR s = SysAllocString(OLESTR("FoxitPhantomPDFUpdateService"));


    //std::wcout << *(BSTR*)&s << std::endl;

    std::cout << "pIManager->stopService()" << std::endl;


    wprintf(L"stop %d %s\n", i, pServiceStatus[376].lpServiceName);

    //BSTR s = SysAllocString(pServiceStatus[439].lpServiceName);

    hr = pIManager->stopService(pServiceStatus[376].lpServiceName);


    Sleep(10000);


    hr = pIManager->startService(pServiceStatus[376].lpServiceName);


    if (pICF)         pICF->Release();
    if (pICreateManager) pICreateManager->Release();
    if (pIManager)      pIManager->Release();

    std::cout << std::endl << "Close the COM runtime...";
    CoUninitialize();
    std::cout << "success." << std::endl;

    return 0;
}

void ShowErrorMessage(LPCTSTR header, HRESULT hr)
{
    void* pMsg;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pMsg, 0, NULL);

    std::cout << header << ": Error(" << std::hex << hr << "): "
        << (LPTSTR)pMsg << std::endl;

    LocalFree(pMsg);
}

