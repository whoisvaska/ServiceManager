//#include "stdafx.h"
#include "../ServiceManager/server_type_info_i.c"
#include "manager_class_factory.h"  
#include <iostream> 
#include <string.h>
#include <string>
#include <filesystem>
#include <windows.h>


#if defined(_WIN64)

#define CROSS_ACCESS KEY_WOW64_32KEY

#else

#define CROSS_ACCESS KEY_WOW64_64KEY

#endif

// global counter for locks & active objects.
DWORD g_allLocks;

bool regExe(wchar_t exePath[MAX_PATH]);

BOOL
CreateRegistryKeyValue(
    HKEY hKeyParent,
    PWCHAR KeyName,
    REGSAM rsAccessMask,
    REGSAM rsSamViewFlag,
    PBYTE pValue,
    DWORD dwValueSize
);


// for showing the possible mistakes
void ShowErrorMessage(LPCTSTR header, HRESULT);

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    HRESULT hr = S_OK;

    wchar_t exePath[MAX_PATH];

    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule == NULL)
    {
        // Use GetModuleFileName() with module handle to get the path
    }

    GetModuleFileNameW(hModule, exePath, (sizeof(exePath)));
    regExe(exePath);

    std::wstring exeDir = std::wstring(exePath);
    int pos = exeDir.find(L"ServiceManager.exe");
    exeDir.erase(pos, std::wstring(L"ServiceManager.exe").length());

    //MessageBoxW(NULL, exeDir.c_str(), L"", MB_OK | MB_SETFOREGROUND);

    std::wstring tlbPath = exeDir + L"server_type_info.tlb";

    CoInitialize(NULL);


    ITypeLib* pTLib = NULL;

    hr = LoadTypeLibEx(tlbPath.c_str(), REGKIND_REGISTER, &pTLib);
    if (FAILED(hr))
    {
        ShowErrorMessage("LoadTypeLibEx()", hr);
        exit(1);
    }

    /*FILE* file;
    file = _wfopen(tlbPath.c_str(), L"r");
    if (file) 
    {
        fclose(file);
    }
    else 
    {
        MessageBoxW(NULL, tlbPath.c_str(), L"File not found", MB_OK | MB_SETFOREGROUND);
        exit(1);
    }*/


    pTLib->Release();

    //MessageBox(NULL, "Server is initilizing", "Message!", MB_OK | MB_SETFOREGROUND);
    if (strstr(lpCmdLine, "/Embedding") || strstr(lpCmdLine, "-Embedding") || TRUE)
    {
        //MessageBox(NULL, "Server is registering the classes", "Message!", MB_OK | MB_SETFOREGROUND);

        ManagerClassFactory managerClassFactory;

        DWORD regID = 0;
        hr = CoRegisterClassObject(CLSID_ServiceManager, (IClassFactory*)&managerClassFactory,
            CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &regID);
        if (FAILED(hr))
        {
            ShowErrorMessage("CoRegisterClassObject()", hr);
            CoUninitialize();
            exit(1);
        }

        MSG ms;
        while (GetMessage(&ms, 0, 0, 0))
        {
            TranslateMessage(&ms);
            DispatchMessage(&ms);
        }
        CoRevokeClassObject(regID);
    }

    CoUninitialize();
    //MessageBox(NULL, "Server is dying", "Message!", MB_OK | MB_SETFOREGROUND);
    return 0;
}

void ShowErrorMessage(LPCTSTR header, HRESULT hr)
{
    void* pMsg;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pMsg, 0, NULL);

    std::cout << header << ": Error(" << std::hex << hr << "): "
        << (LPTSTR)pMsg << std::endl;

    MessageBoxA(NULL, header, NULL, MB_SETFOREGROUND | MB_OK);
    LocalFree(pMsg);

    return;
}



bool regExe(wchar_t exePath[MAX_PATH])
{

    wchar_t clsid[MAX_PATH] = L"{53BA7C94-C5BC-42D8-933A-BB08F220849E}";
    wchar_t keyValue[MAX_PATH] = L"test.ServiceManager.Manager";

    typedef struct
    {
        HKEY hkRoot;
        LPCWSTR szRootName;
        LPCWSTR szName;
        LPWSTR szData;
    }KEYDATA;

    KEYDATA Keys[] =
    {
        { HKEY_CLASSES_ROOT,  L"HKCR", L"test.ServiceManager.Manager\\CLSID", clsid},
        { HKEY_CLASSES_ROOT,  L"HKCR", L"CLSID\\{53BA7C94-C5BC-42D8-933A-BB08F220849E}", keyValue},
        { HKEY_CLASSES_ROOT,  L"HKCR", L"CLSID\\{53BA7C94-C5BC-42D8-933A-BB08F220849E}\\LocalServer32", exePath}
    };

    bool Res;

    for (int i = 0; i < _countof(Keys); i++)
    {
        Res =
            CreateRegistryKeyValue(
                Keys[i].hkRoot,
                const_cast<LPWSTR>(Keys[i].szName),
                KEY_READ | KEY_WRITE,
                CROSS_ACCESS,
                (PBYTE)(Keys[i].szData),
                MAX_PATH);
        //MessageBoxW(NULL, (LPWSTR)(PBYTE)(Keys[i].szData), std::to_wstring(sizeof(Keys[i].szData)).c_str(), MB_OK | MB_SETFOREGROUND);
        if (Res == FALSE)
        {
            printf("Could not create keys in alternate view of the registry.\n");
            //return 1;
        }

        Res =
            CreateRegistryKeyValue(
                Keys[i].hkRoot,
                const_cast<LPWSTR>(Keys[i].szName),
                KEY_READ | KEY_WRITE,
                0,
                (PBYTE)(Keys[i].szData),
                MAX_PATH);
    }
    return true;
}


BOOL
CreateRegistryKeyValue(
    HKEY hKeyParent,
    PWCHAR KeyName,
    REGSAM rsAccessMask,
    REGSAM rsSamViewFlag,
    PBYTE pValue,
    DWORD dwValueSize
)
{
    DWORD dwDisposition;
    HKEY  hKey;
    DWORD dwRet;

    dwRet =
        RegCreateKeyExW(
            hKeyParent,
            KeyName,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            rsAccessMask | rsSamViewFlag,
            NULL,
            &hKey,
            &dwDisposition);

    if (dwRet != ERROR_SUCCESS)
    {
        printf("Error opening or creating key.\n");
        return FALSE;
    }

    // Attempt to set the value of the key.
    // If the call fails, close the key and return.
    if (ERROR_SUCCESS !=
        RegSetValueExW(
            hKey,
            NULL,
            0,
            REG_SZ,
            pValue,
            dwValueSize))
    {
        printf("Could not set registry value.\n");
        RegCloseKey(hKey);
        return FALSE;
    }

    RegCloseKey(hKey);
    return TRUE;
}
