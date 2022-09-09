//#include "stdafx.h"
#include "../ServiceManager/server_type_info_i.c"
#include "manager_class_factory.h"	
#include <iostream>	
#include <string.h>
#include <string>
#include <filesystem>


// global counter for locks & active objects.
DWORD g_allLocks;

LPCSTR lpTypeLibPath = "D:\\Projects\\com-service\\src\\Service\\ServiceManager\\ServiceManager.tlb";

// for showing the possible mistakes
void ShowErrorMessage(LPCTSTR header, HRESULT);

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    // Init COM.
    HRESULT hr;
    MessageBoxA(NULL, "MyCar Server [LOCAL] is initilizing", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);
    CoInitialize(NULL);
    MessageBox(NULL, "MyCar Server [LOCAL] is initilizing1", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);

    // Let's register the type lib to get the 'free' type library marsahler.
    ITypeLib* pTLib = NULL;


    hr = LoadTypeLibEx(L"D:\\Projects\\com-service\\src\\Service\\ServiceManager\\server_type_info.tlb", REGKIND_REGISTER, &pTLib);

    wchar_t buf[100];
    //swprintf_s(L"error_code = %d", hr);
    //swprintf(buf, 100, L"error_code = %d", access();

    FILE* file;
    file = _wfopen(L"D:\\Projects\\com-service\\src\\Service\\ServiceManager\\server_type_info.tlb", L"r");
    if (file) {
        //file exists and can be opened 
        //...
        // close file when you're done
        fclose(file);
        MessageBoxW(NULL, L"file yes", L"MyCar Server [LOCAL] is initilizing2", MB_OK | MB_SETFOREGROUND);

    }
    else {
        MessageBoxW(NULL, L"file no", L"MyCar Server [LOCAL] is initilizing2", MB_OK | MB_SETFOREGROUND);

        //file doesn't exists or cannot be opened (es. you don't have access permission)
    }




    pTLib->Release();
    MessageBox(NULL, "MyCar Server [LOCAL] is initilizing3", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);

    if (FAILED(hr))
    {
        ShowErrorMessage("LoadTypeLibEx()", hr);
        exit(1);
    }
    MessageBox(NULL, "MyCar Server [LOCAL] is initilizing4", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);
    // Let's see if we were started by SCM.
    if (strstr(lpCmdLine, "/Embedding") || strstr(lpCmdLine, "-Embedding") || TRUE)
    {
        // for debug
        MessageBox(NULL, "MyCar Server[LOCAL] is registering the classes", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);

        // Create the MyCar class object.
        ManagerClassFactory managerClassFactory;

        // Register the MyCar Factory.
        DWORD regID = 0;
        hr = CoRegisterClassObject(CLSID_ServiceManager, (IClassFactory*)&managerClassFactory,
            CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &regID);
        if (FAILED(hr))
        {
            ShowErrorMessage("CoRegisterClassObject()", hr);
            CoUninitialize();
            exit(1);
        }

        // Now just run until a quit message is sent,
        // in responce to the final release.
        MSG ms;
        while (GetMessage(&ms, 0, 0, 0))
        {
            TranslateMessage(&ms);
            DispatchMessage(&ms);
        }

        // All done, so remove class object.
        CoRevokeClassObject(regID);
    }


    // Terminate COM.
    CoUninitialize();
    MessageBox(NULL, "[LOCAL] Server is dying", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);
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

