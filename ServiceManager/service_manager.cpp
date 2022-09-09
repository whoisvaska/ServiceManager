//#include "stdafx.h"
#include "service_manager.h"
#include "../ServiceManager/server_type_info_i.c"
#include "service_struct_i.c"

#include "locks.h"

#include <stdio.h> 
#include <iostream>
#include <sstream>
#include <string>


void ErrorDescription(DWORD p_dwError);


ServiceManager::ServiceManager() : m_refCount(0)
{
	Lock();
}


ServiceManager::~ServiceManager()
{
	UnLock();
	MessageBox(NULL,
		"ServiceManager is being distructed. Make sure you see this message, if not, you might have memory leak!",
		"Destructor", MB_OK | MB_SETFOREGROUND);
}


STDMETHODIMP ServiceManager::QueryInterface(REFIID riid, void** pIFace)
{
	// Which aspect of me do they want?
	if (riid == IID_IUnknown)
	{
		*pIFace = (IUnknown*)(IManager*)this;
		MessageBox(NULL, "Handed out IUnknown", "QI", MB_OK | MB_SETFOREGROUND);
	}

	else if (riid == IID_IManager)
	{
		*pIFace = (IManager*)this;
		MessageBox(NULL, "Handed out Manager", "QI", MB_OK | MB_SETFOREGROUND);
	}


	else if (riid == IID_ICreateManager)
	{
		*pIFace = (ICreateManager*)this;
		MessageBox(NULL, "Handed out ICreateServiceManager", "QI", MB_OK | MB_SETFOREGROUND);
	}
	else
	{
		*pIFace = NULL;
		return E_NOINTERFACE;
	}

	((IUnknown*)(*pIFace))->AddRef();
	return S_OK;
}

STDMETHODIMP_(DWORD) ServiceManager::AddRef()
{
	++m_refCount;
	return m_refCount;
}

STDMETHODIMP_(DWORD) ServiceManager::Release()
{
	if (--m_refCount == 0)
	{
		delete this;
		return 0;
	}
	else
		return m_refCount;
}

// IManager

//STDMETHODIMP ServiceManager::enumerateServices()


STDMETHODIMP ServiceManager::startService(BSTR szServiceName)
{
	SERVICE_STATUS_PROCESS ssStatus;
	DWORD dwOldCheckPoint;
	DWORD dwStartTickCount;
	DWORD dwWaitTime;
	DWORD dwBytesNeeded;
	SC_HANDLE schSCManager;
	SC_HANDLE schService;

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // servicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager)
	{
		MessageBoxA(NULL, "OpenSCManager failed", "QI", MB_OK | MB_SETFOREGROUND);
		return -1;
	}

	// Get a handle to the service.

	schService = OpenServiceW(
		schSCManager,         // SCM database 
		szServiceName,            // name of service 
		SERVICE_ALL_ACCESS);  // full access 

	if (schService == NULL)
	{
		MessageBoxA(NULL, "OpenService failed", "QI", MB_OK | MB_SETFOREGROUND);

		CloseServiceHandle(schSCManager);
		return -1;
	}

	// Check the status in case the service is not stopped. 

	if (!QueryServiceStatusEx(
		schService,                     // handle to service 
		SC_STATUS_PROCESS_INFO,         // information level
		(LPBYTE)&ssStatus,             // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded))              // size needed if buffer is too small
	{
		MessageBoxA(NULL, "QueryServiceStatusEx failed", "QI", MB_OK | MB_SETFOREGROUND);

		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return -1;
	}

	// Check if the service is already running. It would be possible 
	// to stop the service here, but for simplicity this example just returns. 

	if (ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
	{
		//printf("\n");
		MessageBoxA(NULL, "Cannot start the service because it is already running", "QI", MB_OK | MB_SETFOREGROUND);

		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return S_OK;
	}

	// Save the tick count and initial checkpoint.

	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	// Wait for the service to stop before attempting to start it.

	while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
	{
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds. 

		dwWaitTime = ssStatus.dwWaitHint / 10;

		if (dwWaitTime < 1000)
			dwWaitTime = 1000;
		else if (dwWaitTime > 10000)
			dwWaitTime = 10000;

		Sleep(dwWaitTime);

		// Check the status until the service is no longer stop pending. 

		if (!QueryServiceStatusEx(
			schService,                     // handle to service 
			SC_STATUS_PROCESS_INFO,         // information level
			(LPBYTE)&ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS), // size of structure
			&dwBytesNeeded))              // size needed if buffer is too small
		{
			//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			MessageBoxA(NULL, "QueryServiceStatusEx failed", "QI", MB_OK | MB_SETFOREGROUND);

			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			return -1;
		}

		if (ssStatus.dwCheckPoint > dwOldCheckPoint)
		{
			// Continue to wait and check.

			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
			{
				MessageBoxA(NULL, "Timeout waiting for service to stop", "QI", MB_OK | MB_SETFOREGROUND);

				//printf("Timeout waiting for service to stop\n");
				CloseServiceHandle(schService);
				CloseServiceHandle(schSCManager);
				return -1;
			}
		}
	}

	// Attempt to start the service.

	if (!StartService(
		schService,  // handle to service 
		0,           // number of arguments 
		NULL))      // no arguments 
	{
		MessageBoxA(NULL, "StartService failed ", "QI", MB_OK | MB_SETFOREGROUND);

		//printf("StartService failed (%d)\n", GetLastError());

		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return -1;
	}
	else printf("Service start pending...\n");

	// Check the status until the service is no longer start pending. 

	if (!QueryServiceStatusEx(
		schService,                     // handle to service 
		SC_STATUS_PROCESS_INFO,         // info level
		(LPBYTE)&ssStatus,             // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded))              // if buffer too small
	{
		MessageBoxA(NULL, "QueryServiceStatusEx failed", "QI", MB_OK | MB_SETFOREGROUND);

		//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return -1;
	}

	// Save the tick count and initial checkpoint.

	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
	{
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth the wait hint, but no less than 1 second and no 
		// more than 10 seconds. 

		dwWaitTime = ssStatus.dwWaitHint / 10;

		if (dwWaitTime < 1000)
			dwWaitTime = 1000;
		else if (dwWaitTime > 10000)
			dwWaitTime = 10000;

		Sleep(dwWaitTime);

		// Check the status again. 

		if (!QueryServiceStatusEx(
			schService,             // handle to service 
			SC_STATUS_PROCESS_INFO, // info level
			(LPBYTE)&ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS), // size of structure
			&dwBytesNeeded))              // if buffer too small
		{
			printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			break;
		}

		if (ssStatus.dwCheckPoint > dwOldCheckPoint)
		{
			// Continue to wait and check.

			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
			{
				// No progress made within the wait hint.
				break;
			}
		}
	}

	// Determine whether the service is running.

	if (ssStatus.dwCurrentState == SERVICE_RUNNING)
	{
		MessageBoxA(NULL, "Service started successfully ", "QI", MB_OK | MB_SETFOREGROUND);


		//printf("Service started successfully.\n");
	}
	else
	{
		printf("Service not started. \n");
		printf("  Current State: %d\n", ssStatus.dwCurrentState);
		printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
		printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
		printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
	}


	MessageBoxA(NULL, "Service started successfully ", "QI", MB_OK | MB_SETFOREGROUND);

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return S_OK;
}


STDMETHODIMP ServiceManager::stopService(BSTR szServiceName) {
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS_PROCESS ssp;
    DWORD dwStartTime = GetTickCount();
    DWORD dwBytesNeeded;
    DWORD dwTimeout = 30000; // 30-second time-out
    DWORD dwWaitTime;


	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager)
	{
		MessageBox(NULL, "OpenSCManager() failed", "QI", MB_OK | MB_SETFOREGROUND);

		return -1;
	}

	MessageBox(NULL, "OpenSCManager() success", "QI", MB_OK | MB_SETFOREGROUND);


	schService = OpenServiceW(
		schSCManager,         // SCM database 
		szServiceName,            // name of service 
		SERVICE_STOP |
		SERVICE_QUERY_STATUS |
		SERVICE_ENUMERATE_DEPENDENTS);

	if (schService == NULL)
	{
		MessageBoxW(NULL, szServiceName, L"QI", MB_OK | MB_SETFOREGROUND);

		wchar_t buf[128];
		//wsprintf("GetLastError() %d", buf);

		swprintf_s(buf, L"GetLastError() %d", GetLastError());



		MessageBox(NULL, "OpenServiceW() failed", "QI", MB_OK | MB_SETFOREGROUND);
		MessageBoxW(NULL, buf, L"QI", MB_OK | MB_SETFOREGROUND);

		CloseServiceHandle(schSCManager);
		return -1;
	}
	MessageBox(NULL, "OpenServiceW() success", "QI", MB_OK | MB_SETFOREGROUND);


	if (!QueryServiceStatusEx(
		schService,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded))
	{
		MessageBox(NULL, "QueryServiceStatusEx failed()", "QI", MB_OK | MB_SETFOREGROUND);

		//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		goto stop_cleanup;
	}

	if (ssp.dwCurrentState == SERVICE_STOPPED)
	{
		MessageBox(NULL, "Service is already stopped", "QI", MB_OK | MB_SETFOREGROUND);

		//printf("Service is already stopped.\n");
		goto stop_cleanup;
	}

	// If a stop is pending, wait for it.

	while (ssp.dwCurrentState == SERVICE_STOP_PENDING)
	{
		//printf("Service stop pending...\n");

		MessageBox(NULL, "Service stop pending...", "QI", MB_OK | MB_SETFOREGROUND);

		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds. 

		dwWaitTime = ssp.dwWaitHint / 10;

		if (dwWaitTime < 1000)
			dwWaitTime = 1000;
		else if (dwWaitTime > 10000)
			dwWaitTime = 10000;

		Sleep(dwWaitTime);

		if (!QueryServiceStatusEx(
			schService,
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp,
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded))
		{
			MessageBox(NULL, "QueryServiceStatusEx failed()", "QI", MB_OK | MB_SETFOREGROUND);

			//printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			goto stop_cleanup;
		}

		if (ssp.dwCurrentState == SERVICE_STOPPED)
		{
			MessageBox(NULL, "Service stopped successfully", "QI", MB_OK | MB_SETFOREGROUND);

			//printf("Service stopped successfully.\n");
			goto stop_cleanup;
		}

		if (GetTickCount() - dwStartTime > dwTimeout)
		{

			MessageBox(NULL, "Service stop timed out.", "QI", MB_OK | MB_SETFOREGROUND);

			//printf("Service stop timed out.\n");
			goto stop_cleanup;
		}
	}

	// If the service is running, dependencies must be stopped first.


	// Send a stop code to the service.

	if (!ControlService(
		schService,
		SERVICE_CONTROL_STOP,
		(LPSERVICE_STATUS)&ssp))
	{
		printf("ControlService failed (%d)\n", GetLastError());
		goto stop_cleanup;
	}

	// Wait for the service to stop.

	while (ssp.dwCurrentState != SERVICE_STOPPED)
	{
		Sleep(ssp.dwWaitHint);
		if (!QueryServiceStatusEx(
			schService,
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp,
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded))
		{
			printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			goto stop_cleanup;
		}

		if (ssp.dwCurrentState == SERVICE_STOPPED)
			break;

		if (GetTickCount() - dwStartTime > dwTimeout)
		{
			printf("Wait timed out\n");
			goto stop_cleanup;
		}
	}
	printf("Service stopped successfully\n");
stop_cleanup:
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return S_OK;
}


STDMETHODIMP ServiceManager::changeServiceConfigSM(BSTR serviceName, QUERY_SERVICE_CONFIG_UDT* serviceConfig) {
	HRESULT hr = S_OK;

	//MessageBox(NULL, "changeServiceConfigSM", "QI", MB_OK | MB_SETFOREGROUND);


	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_ALL_ACCESS);
	if (NULL == schService) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	if (!ChangeServiceConfigW(
		schService,

		//serviceConfig->dwServiceType,
		SERVICE_NO_CHANGE,

		serviceConfig->dwStartType,
		//SERVICE_NO_CHANGE,

		//serviceConfig->dwErrorControl,
		SERVICE_NO_CHANGE,

		//serviceConfig->lpBinaryPathName,
		NULL,

		//serviceConfig->lpLoadOrderGroup,
		NULL,

		//&serviceConfig->dwTagId,
		NULL,

		//serviceConfig->lpDependencies,
		NULL,

		//serviceConfig->lpServiceStartName,
		NULL,

		//lpPassword
		NULL,

		//serviceConfig->lpDisplayName
		NULL

	)) 
	{
		ErrorDescription(GetLastError());

		MessageBox(NULL, "ChangeServiceConfigW == not OK", "QI", MB_OK | MB_SETFOREGROUND);
		return -1;
	}
	else{
		MessageBox(NULL, "ChangeServiceConfigW == OK", "QI", MB_OK | MB_SETFOREGROUND);
	}
	return S_OK;
}


STDMETHODIMP ServiceManager::queryServiceConfigSM(BSTR serviceName, QUERY_SERVICE_CONFIG_UDT* serviceConfig) {
	HRESULT hr = S_OK;

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_ALL_ACCESS);
	if (NULL == schService) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;
	LPQUERY_SERVICE_CONFIGW lpServiceConfig = NULL;

	if (!QueryServiceConfigW(schService, NULL, 0, &dwBytesNeeded)) {

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			ErrorDescription(GetLastError());
			MessageBox(NULL, "GetLsatErr != ERROR_INSUFFICIENT_BUFFER", "QI", MB_OK | MB_SETFOREGROUND);

			return -1;
		}

		lpServiceConfig = (LPQUERY_SERVICE_CONFIGW)LocalAlloc(LMEM_FIXED, dwBytesNeeded);

		if (lpServiceConfig == NULL) {
			ErrorDescription(GetLastError());
			MessageBox(NULL, "lpServiceConfig == NULL", "QI", MB_OK | MB_SETFOREGROUND);

			return -1;
		}



		dwBufSize = dwBytesNeeded;
		if (!QueryServiceConfigW(schService, lpServiceConfig, dwBufSize, &dwBytesNeeded)) {
			ErrorDescription(GetLastError());
			MessageBox(NULL, "QueryServiceConfigW == Error", "QI", MB_OK | MB_SETFOREGROUND);

			return -1;
		}
		//*serviceConfig.
		
		serviceConfig->dwServiceType = lpServiceConfig->dwServiceType;
		serviceConfig->dwStartType = lpServiceConfig->dwStartType;
		serviceConfig->dwErrorControl = lpServiceConfig->dwErrorControl;
		serviceConfig->lpBinaryPathName = SysAllocString(lpServiceConfig->lpBinaryPathName);
		serviceConfig->lpLoadOrderGroup = SysAllocString(lpServiceConfig->lpLoadOrderGroup);
		serviceConfig->dwTagId = lpServiceConfig->dwTagId;
		serviceConfig->lpDependencies = SysAllocString(lpServiceConfig->lpDependencies);
		serviceConfig->lpServiceStartName = SysAllocString(lpServiceConfig->lpServiceStartName);
		
		
		serviceConfig->lpDisplayName = SysAllocString(lpServiceConfig->lpDisplayName);



		std::string s = std::to_string(serviceConfig->dwStartType);
		MessageBox(NULL, s.c_str(), "QI", MB_OK | MB_SETFOREGROUND);




		LocalFree(lpServiceConfig);
	}
	else {
		MessageBox(NULL, "QueryServiceConfigWSM == NOtOK", "QI", MB_OK | MB_SETFOREGROUND);
	}

	MessageBox(NULL, "QueryServiceConfigWSM == OK", "QI", MB_OK | MB_SETFOREGROUND);

	return S_OK;
}

STDMETHODIMP ServiceManager::changeServiceFailureActionsSM(BSTR serviceName, struct SERVICE_FAILURE_ACTIONS_UDT* serviceConfig) {
	HRESULT hr = S_OK;

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_ALL_ACCESS);
	if (NULL == schService) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;

	LPBYTE lpBuffer = NULL;


	SC_ACTION failureActions[3] = {
		{(SC_ACTION_TYPE)(serviceConfig->lpsaAction1.Type), 0 },
		{(SC_ACTION_TYPE)(serviceConfig->lpsaAction2.Type), 0 },
		{(SC_ACTION_TYPE)(serviceConfig->lpsaAction3.Type), 0 },
	};

	SERVICE_FAILURE_ACTIONSW serviceFailureActions;
	//INFINITE
	serviceFailureActions.cActions = 3;
	serviceFailureActions.dwResetPeriod = serviceConfig->dwResetPeriod;

	serviceFailureActions.lpCommand = serviceConfig->lpCommand;
	serviceFailureActions.lpRebootMsg = serviceConfig->lpRebootMsg;

	serviceFailureActions.lpsaActions = failureActions;

	//serviceFailureActions

	if (!ChangeServiceConfig2W(schService, SERVICE_CONFIG_FAILURE_ACTIONS, &serviceFailureActions)) {

		ErrorDescription(GetLastError());
		MessageBox(NULL, "ChangeServiceConfig2W == Error", "QI", MB_OK | MB_SETFOREGROUND);

	}
	else {
		MessageBox(NULL, "ChangeServiceConfig2W == OK", "QI", MB_OK | MB_SETFOREGROUND);
	}

	
	return S_OK;
}


STDMETHODIMP ServiceManager::queryServiceFailureActionsSM(BSTR serviceName, SERVICE_FAILURE_ACTIONS_UDT* serviceConfig) {
	HRESULT hr = S_OK;

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open SCM sucessfully2", "QI", MB_OK | MB_SETFOREGROUND);
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_ALL_ACCESS);
	if (NULL == schService) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open Service sucessfully2", "QI", MB_OK | MB_SETFOREGROUND);
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;

	LPBYTE lpBuffer = NULL;

	if (!QueryServiceConfig2W(schService, SERVICE_CONFIG_FAILURE_ACTIONS, lpBuffer, 0, &dwBytesNeeded)) {

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			ErrorDescription(GetLastError());
			MessageBox(NULL, "GetLsatErr != ERROR_INSUFFICIENT_BUFFER", "QI", MB_OK | MB_SETFOREGROUND);

			return -1;
		}
		lpBuffer = (BYTE*)LocalAlloc(LMEM_FIXED, dwBytesNeeded);

		if (lpBuffer == NULL) {
			ErrorDescription(GetLastError());
			MessageBox(NULL, "lpServiceConfig == NULL", "QI", MB_OK | MB_SETFOREGROUND);

			return -1;
		}

		dwBufSize = dwBytesNeeded;
		if (!QueryServiceConfig2W(schService, SERVICE_CONFIG_FAILURE_ACTIONS, lpBuffer, dwBufSize, &dwBytesNeeded)) {
			ErrorDescription(GetLastError());
			MessageBox(NULL, "QueryServiceConfig2W == Error", "QI", MB_OK | MB_SETFOREGROUND);

			return -1;
		}
		std::string s;
		//s = std::to_string(reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(serviceConfig)->lpsaActions[0].Type);
		//MessageBox(NULL, s.c_str(), "action", MB_OK | MB_SETFOREGROUND);
		//serviceConfig->lpsaActions[0].Type = 22;

		serviceConfig->cActions = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->cActions;
		serviceConfig->dwResetPeriod = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->dwResetPeriod;

		MessageBox(NULL, "Before alloc", "QI", MB_OK | MB_SETFOREGROUND);


		serviceConfig->lpCommand = SysAllocString( reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpCommand);
		serviceConfig->lpRebootMsg = SysAllocString(reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpRebootMsg);

		MessageBox(NULL, "after alloc", "QI", MB_OK | MB_SETFOREGROUND);


		serviceConfig->lpsaAction1.Type = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[0].Type;
		serviceConfig->lpsaAction1.Delay = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[0].Delay;
		serviceConfig->lpsaAction2.Type = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[1].Type;
		serviceConfig->lpsaAction2.Delay = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[1].Delay;
		serviceConfig->lpsaAction3.Type = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[2].Type;
		serviceConfig->lpsaAction3.Delay = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[2].Delay;

	}
	else {
		MessageBox(NULL, "QueryServiceConfigW2SM == NOtOK", "QI", MB_OK | MB_SETFOREGROUND);
	}

	MessageBox(NULL, "QueryServiceConfigW2SM == OK", "QI", MB_OK | MB_SETFOREGROUND);

	return S_OK;
}


STDMETHODIMP ServiceManager::queryServiceInfo(BSTR serviceName, SERVICE_STATUS_PROCESS_UDT* psaServiceInfo) {
	HRESULT hr = S_OK;

	MessageBoxW(NULL, serviceName, L"QI", MB_OK | MB_SETFOREGROUND);


	//? почему тут ENUM_SERVICE_STATUS_PROCESS_UDT, если возвращаем SERVICE_STATUS_PROCESS_UDT ?
	GUID GUID_ENUM_SERVICE_STATUS_PROCESS_UDT = __uuidof(ENUM_SERVICE_STATUS_PROCESS_UDT);

	IRecordInfo* recordInfo;

	hr = GetRecordInfoFromGuids(LIBID_ServiceLib, 1, 0, 0,
		GUID_ENUM_SERVICE_STATUS_PROCESS_UDT, &recordInfo);


	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}



	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_ALL_ACCESS);
	if (NULL == schService) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}


	//ENUM_SERVICE_STATUS service;
	SERVICE_STATUS serviceStatus;

	//DWORD dwBytesNeeded = 0;
	//DWORD dwServicesReturned = 0;
	//DWORD dwResumedHandle = 0;

	ENUM_SERVICE_STATUS* pServices = NULL;

	DWORD dwServiceType = SERVICE_WIN32 | SERVICE_DRIVER;

	BOOL retVal = QueryServiceStatus(schService, &serviceStatus);
	if (!retVal) {
		ErrorDescription(GetLastError());
		return -1;
	}
	else {
		//MessageBox(NULL, "Query Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	memcpy_s(psaServiceInfo, sizeof(SERVICE_STATUS_PROCESS_UDT), &serviceStatus, sizeof(SERVICE_STATUS));

	return S_OK;
}



STDMETHODIMP ServiceManager::getServices(int* servicesCount, SAFEARRAY ** psaServiceName, SAFEARRAY ** psaServiceStatus)
{

	HRESULT hr = S_OK;

	//struct DECLSPEC_UUID("D8B3861A-74C6-11d2-0000-00C04FB17CCA")
		            //EnumServiceStatusUDT;

	GUID GUID_ENUM_SERVICE_STATUS_PROCESS_UDT = __uuidof(ENUM_SERVICE_STATUS_PROCESS_UDT);

	IRecordInfo* recordInfo;

	hr = GetRecordInfoFromGuids(LIBID_ServiceLib, 1, 0, 0,
		GUID_ENUM_SERVICE_STATUS_PROCESS_UDT, &recordInfo);


	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) {
		ErrorDescription(GetLastError());
		return -1;
	}

	else {
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}


	ENUM_SERVICE_STATUS service;

	DWORD dwBytesNeeded = 0;
	DWORD dwServicesReturned = 0;
	DWORD dwResumedHandle = 0;

	ENUM_SERVICE_STATUS* pServices = NULL;


	DWORD dwServiceType = SERVICE_WIN32 | SERVICE_DRIVER;

	BOOL retVal = EnumServicesStatus(schSCManager, dwServiceType, SERVICE_STATE_ALL,
		&service, sizeof(ENUM_SERVICE_STATUS), &dwBytesNeeded, &dwServicesReturned,
		&dwResumedHandle);

	if (!retVal) {
		// Need big buffer
		if (ERROR_MORE_DATA == GetLastError()) {
			// Set the buffer
			DWORD dwBytes = sizeof(ENUM_SERVICE_STATUS) + dwBytesNeeded;
			pServices = new ENUM_SERVICE_STATUS[dwBytes];
			// Now query again for services
			EnumServicesStatus(schSCManager, SERVICE_WIN32 | SERVICE_DRIVER, SERVICE_STATE_ALL,
				pServices, dwBytes, &dwBytesNeeded, &dwServicesReturned, &dwResumedHandle);
			// now traverse each service to get information

			SAFEARRAYBOUND bounds;

			bounds.cElements = dwServicesReturned;
			bounds.lLbound = 0;

			SAFEARRAYBOUND bounds_;

			bounds_.cElements = dwServicesReturned;
			bounds_.lLbound = 0;

			*psaServiceName = SafeArrayCreate(VT_BSTR, 1, &bounds);
			//*psaServiceStatus = SafeArrayCreate(VT_VARIANT, 1, &bounds_);
			const int iLBound = 0, iUBound = dwServicesReturned;
			SAFEARRAY* psaEnumServiceStatus = SafeArrayCreateVectorEx(VT_RECORD, iLBound,
				iUBound - iLBound + 1, recordInfo);

			ENUM_SERVICE_STATUS_PROCESS_UDT* pServiceStatus = NULL;

			hr = SafeArrayAccessData(psaEnumServiceStatus, (void**)&pServiceStatus);

			long int i = 0;
			long int j = 0;
			long int k = 0;


			for (unsigned iIndex = 0; iIndex < dwServicesReturned; iIndex++) {

				wchar_t wcServiceName[128];
				wchar_t wcDisplayName[128];


				mbstowcs(wcServiceName, ((pServices + iIndex)->lpServiceName), 128);
				mbstowcs(wcDisplayName, ((pServices + iIndex)->lpDisplayName), 128);

				BSTR serviceName = SysAllocString(wcServiceName);

				hr = SafeArrayPutElement(*psaServiceName, &i, serviceName); //Right


				pServiceStatus[iIndex].lpServiceName = SysAllocString(wcServiceName);
				pServiceStatus[iIndex].lpDisplayName = SysAllocString(wcDisplayName);

				memcpy(&pServiceStatus[iIndex].ServiceStatus, &(pServices + iIndex)->ServiceStatus, sizeof((pServices + iIndex)->ServiceStatus));


				k++;
				i++;

				SysFreeString(serviceName);
			}

			SafeArrayDestroy(*psaServiceStatus);


			hr = SafeArrayUnaccessData(psaEnumServiceStatus);

			*psaServiceStatus = psaEnumServiceStatus;

			*servicesCount = dwServicesReturned;

			delete[] pServices;
			pServices = NULL;
		}
		// there is any other reason
		else {
			ErrorDescription(GetLastError());
		}
		MessageBox(NULL, "Close SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);


		return S_OK;
	}



	if (!CloseServiceHandle(schSCManager)) {
		ErrorDescription(GetLastError());
	}

	else {
		MessageBox(NULL, "Close SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
	}

	*servicesCount= 3;

	return 0;
}

void ErrorDescription(DWORD p_dwError) {

	HLOCAL hLocal = NULL;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, p_dwError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPTSTR)&hLocal,
		0, NULL);

	MessageBox(NULL, (LPCTSTR)LocalLock(hLocal), TEXT("Error"), MB_OK | MB_ICONERROR);
	LocalFree(hLocal);
}



