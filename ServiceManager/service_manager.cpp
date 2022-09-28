//#include "stdafx.h"
#include "service_manager.h"
#include "../ServiceManager/server_type_info_i.c"
//#include "service_struct_i.c"

#include "locks.h"

#include <stdio.h> 
#include <iostream>
#include <sstream>
#include <string>


void ErrorDescription(DWORD p_dwError);
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);



ServiceManager::ServiceManager() : m_refCount(0)
{
	Lock();
}


ServiceManager::~ServiceManager()
{
	UnLock();

//#ifdef DEBUG == 1
//	MessageBox(NULL,
//		"ServiceManager is being distructed. Make sure you see this message, if not, you might have memory leak!",
//		"Destructor", MB_OK | MB_SETFOREGROUND);
//#endif
}


STDMETHODIMP ServiceManager::QueryInterface(REFIID riid, void** pIFace)
{
	if (riid == IID_IUnknown)
	{
		*pIFace = (IUnknown*)(IManager*)this;

#ifdef DEBUG == 1
		MessageBox(NULL, "Handed out IUnknown", "QI", MB_OK | MB_SETFOREGROUND);
#endif

	}

	else if (riid == IID_IManager)
	{
		*pIFace = (IManager*)this;
#ifdef DEBUG == 1
		MessageBox(NULL, "Handed out Manager", "QI", MB_OK | MB_SETFOREGROUND);
#endif 
	
	}
	else if (riid == IID_ICreateManager)
	{
		*pIFace = (ICreateManager*)this;
	
#ifdef DEBUG == 1
		MessageBox(NULL, "Handed out ICreateServiceManager", "QI", MB_OK | MB_SETFOREGROUND);
#endif

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
		SERVICE_START | SERVICE_QUERY_STATUS);  // full access 

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
	}
	else
	{
		printf("Service not started. \n");
		printf("  Current State: %d\n", ssStatus.dwCurrentState);
		printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
		printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
		printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
	}

	//MessageBoxA(NULL, "Service started successfully ", "QI", MB_OK | MB_SETFOREGROUND);

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

#ifdef DEBUG == 1

	MessageBox(NULL, "OpenSCManager() success", "QI", MB_OK | MB_SETFOREGROUND);
#endif

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

		swprintf_s(buf, L"GetLastError() %d", GetLastError());



		MessageBox(NULL, "OpenServiceW() failed", "QI", MB_OK | MB_SETFOREGROUND);
		MessageBoxW(NULL, buf, L"QI", MB_OK | MB_SETFOREGROUND);

		CloseServiceHandle(schSCManager);
		return -1;
	}

#ifdef DEBUG == 1
	MessageBox(NULL, "OpenServiceW() success", "QI", MB_OK | MB_SETFOREGROUND);
#endif

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

#ifdef DEBUG == 1
	MessageBox(NULL, "changeServiceConfigSM", "QI", MB_OK | MB_SETFOREGROUND);
#endif 

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else 
	{

#ifdef DEBUG == 1
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_CHANGE_CONFIG);
	if (NULL == schService) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else {

#ifdef DEBUG == 1
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	
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
		return -1;
	}
	else
	{
		//MessageBox(NULL, "ChangeServiceConfigW == OK", "QI", MB_OK | MB_SETFOREGROUND);
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	return S_OK;
}


STDMETHODIMP ServiceManager::queryServiceConfigSM(BSTR serviceName, QUERY_SERVICE_CONFIG_UDT* serviceConfig) 
{
	HRESULT hr = S_OK;


#ifdef DEBUG == 1
	MessageBox(NULL, "queryServiceConfigSM", "QI", MB_OK | MB_SETFOREGROUND);
#endif

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_QUERY_CONFIG);
	if (NULL == schService) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;
	LPQUERY_SERVICE_CONFIGW lpServiceConfig = NULL;

	if (!QueryServiceConfigW(schService, NULL, 0, &dwBytesNeeded)) 
	{

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}

		lpServiceConfig = (LPQUERY_SERVICE_CONFIGW)LocalAlloc(LMEM_FIXED, dwBytesNeeded);

		if (lpServiceConfig == NULL) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}

		dwBufSize = dwBytesNeeded;
		if (!QueryServiceConfigW(schService, lpServiceConfig, dwBufSize, &dwBytesNeeded)) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}

		serviceConfig->dwServiceType = lpServiceConfig->dwServiceType;
		serviceConfig->dwStartType = lpServiceConfig->dwStartType;
		serviceConfig->dwErrorControl = lpServiceConfig->dwErrorControl;
		serviceConfig->lpBinaryPathName = SysAllocString(lpServiceConfig->lpBinaryPathName);
		serviceConfig->lpLoadOrderGroup = SysAllocString(lpServiceConfig->lpLoadOrderGroup);
		serviceConfig->dwTagId = lpServiceConfig->dwTagId;
		serviceConfig->lpDependencies = SysAllocString(lpServiceConfig->lpDependencies);
		serviceConfig->lpServiceStartName = SysAllocString(lpServiceConfig->lpServiceStartName);
		serviceConfig->lpDisplayName = SysAllocString(lpServiceConfig->lpDisplayName);

		LocalFree(lpServiceConfig);
	}
	else 
	{
		ErrorDescription(GetLastError());
	}


	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return S_OK;
}


STDMETHODIMP ServiceManager::changeServiceFailureActionsSM(BSTR serviceName, struct SERVICE_FAILURE_ACTIONS_UDT* serviceConfig) 
{
	
#ifdef DEBUG
	MessageBox(NULL, "changeServiceFailureActionsSM", "QI", MB_OK | MB_SETFOREGROUND);
#endif // DEBUG
	
	HRESULT hr = S_OK;

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_ALL_ACCESS);
	if (NULL == schService) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;
	LPBYTE lpBuffer = NULL;

	SC_ACTION failureActions[3] = 
	{
		{(SC_ACTION_TYPE)(serviceConfig->lpsaAction1.Type), serviceConfig->lpsaAction1.Delay },
		{(SC_ACTION_TYPE)(serviceConfig->lpsaAction2.Type), serviceConfig->lpsaAction2.Delay },
		{(SC_ACTION_TYPE)(serviceConfig->lpsaAction3.Type), serviceConfig->lpsaAction3.Delay },
	};

	SERVICE_FAILURE_ACTIONSW serviceFailureActions;

	serviceFailureActions.cActions = 3;
	serviceFailureActions.dwResetPeriod = serviceConfig->dwResetPeriod;

	serviceFailureActions.lpCommand = serviceConfig->lpCommand;
	serviceFailureActions.lpRebootMsg = serviceConfig->lpRebootMsg;

	serviceFailureActions.lpsaActions = failureActions;

	HANDLE hProcessToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hProcessToken))
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Error opening token", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	SetPrivilege(hProcessToken, SE_SHUTDOWN_NAME, true);

	if (!ChangeServiceConfig2W(schService, SERVICE_CONFIG_FAILURE_ACTIONS, &serviceFailureActions)) 
	{
		ErrorDescription(GetLastError());
	}
	else 
	{
	}

	SERVICE_FAILURE_ACTIONS_FLAG sfaf;
	//sfaf.fFailureActionsOnNonCrashFailures = ;

	//MessageBox(NULL, std::to_string(serviceConfig->fFailureActionsOnNonCrashFailures).c_str(), "", MB_OK | MB_SETFOREGROUND);

	sfaf.fFailureActionsOnNonCrashFailures = serviceConfig->fFailureActionsOnNonCrashFailures;

	if (!ChangeServiceConfig2W(schService, SERVICE_CONFIG_FAILURE_ACTIONS_FLAG, &sfaf))
	{
		ErrorDescription(GetLastError());
	}
	else
	{
	}

	SetPrivilege(hProcessToken, SE_SHUTDOWN_NAME, false);


	CloseHandle(hProcessToken);
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	
	return S_OK;
}



STDMETHODIMP ServiceManager::queryServiceDescriptionSM(BSTR serviceName, struct SERVICE_DESCRIPTION_UDT* serviceDescription) 
{
	HRESULT hr = S_OK;

#ifdef DEBUG == 1
	MessageBox(NULL, "queryServiceDescriptionSM", "QI", MB_OK | MB_SETFOREGROUND);
#endif

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open SCM sucessfully2 (Description)", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS);
	if (NULL == schService) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open Service sucessfully2(Description)", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;
	LPBYTE lpBuffer = NULL;

	if (!QueryServiceConfig2W(schService, SERVICE_CONFIG_DESCRIPTION, lpBuffer, 0, &dwBytesNeeded)) 
	{

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}
		lpBuffer = (BYTE*)LocalAlloc(LMEM_FIXED, dwBytesNeeded);

		if (lpBuffer == NULL) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}

		dwBufSize = dwBytesNeeded;
		if (!QueryServiceConfig2W(schService, SERVICE_CONFIG_DESCRIPTION, lpBuffer, dwBufSize, &dwBytesNeeded)) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}
		
		serviceDescription->lpDescription = SysAllocString(reinterpret_cast<SERVICE_DESCRIPTIONW*>(lpBuffer)->lpDescription);

		LocalFree(lpBuffer);
	}
	else 
	{
		ErrorDescription(GetLastError());
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return S_OK;
}



STDMETHODIMP ServiceManager::queryServiceFailureActionsSM(BSTR serviceName, SERVICE_FAILURE_ACTIONS_UDT* serviceConfig) 
{
	HRESULT hr = S_OK;

#ifdef DEBUG == 1
	MessageBox(NULL, "queryServiceFailureActionsSM", "QI", MB_OK | MB_SETFOREGROUND);
#endif

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "queryServiceFailureActionsSM", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_QUERY_CONFIG);
	if (NULL == schService) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;
	LPBYTE lpBuffer = NULL;

	if (!QueryServiceConfig2W(schService, SERVICE_CONFIG_FAILURE_ACTIONS, lpBuffer, 0, &dwBytesNeeded)) 
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}
		lpBuffer = (BYTE*)LocalAlloc(LMEM_FIXED, dwBytesNeeded);

		if (lpBuffer == NULL) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}

		dwBufSize = dwBytesNeeded;
		if (!QueryServiceConfig2W(schService, SERVICE_CONFIG_FAILURE_ACTIONS, lpBuffer, dwBufSize, &dwBytesNeeded)) 
		{
			ErrorDescription(GetLastError());
			return -1;
		}
	
		serviceConfig->cActions = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->cActions;
		serviceConfig->dwResetPeriod = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->dwResetPeriod;

		

		if (reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions != NULL) 
		{
			serviceConfig->lpsaAction1.Type = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[0].Type;
			serviceConfig->lpsaAction1.Delay = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[0].Delay;

			serviceConfig->lpsaAction2.Type = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[1].Type;
			serviceConfig->lpsaAction2.Delay = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[1].Delay;

			serviceConfig->lpsaAction3.Type = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[2].Type;
			serviceConfig->lpsaAction3.Delay = reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpsaActions[2].Delay;
		}

		serviceConfig->lpCommand = SysAllocString(reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpCommand);
		serviceConfig->lpRebootMsg = SysAllocString(reinterpret_cast<SERVICE_FAILURE_ACTIONSW*>(lpBuffer)->lpRebootMsg);

		LocalFree(lpBuffer);

	}
	else 
	{
	}

	SERVICE_FAILURE_ACTIONS_FLAG sfaf;
	dwBytesNeeded = 0;
	if (!QueryServiceConfig2(schService, SERVICE_CONFIG_FAILURE_ACTIONS_FLAG, reinterpret_cast<LPBYTE>(&sfaf), sizeof(sfaf), &dwBytesNeeded))
	{
		ErrorDescription(GetLastError());
	}
	else
	{
		serviceConfig->fFailureActionsOnNonCrashFailures = sfaf.fFailureActionsOnNonCrashFailures;
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return S_OK;
}


STDMETHODIMP ServiceManager::queryServiceInfo(BSTR serviceName, SERVICE_STATUS_PROCESS_UDT* psaServiceInfo) 
{
	HRESULT hr = S_OK;

#ifdef DEBUG == 1
	MessageBox(NULL, "queryServiceInfo", "QI", MB_OK | MB_SETFOREGROUND);
#endif

	GUID GUID_ENUM_SERVICE_STATUS_PROCESS_UDT = __uuidof(ENUM_SERVICE_STATUS_PROCESS_UDT);

	IRecordInfo* recordInfo;

	hr = GetRecordInfoFromGuids(LIBID_ServiceLib, 1, 0, 0,
		GUID_ENUM_SERVICE_STATUS_PROCESS_UDT, &recordInfo);


	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_QUERY_STATUS);
	if (NULL == schService) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	SERVICE_STATUS serviceStatus;

	BOOL retVal = QueryServiceStatus(schService, &serviceStatus);
	if (!retVal) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
	}

	psaServiceInfo->dwCheckPoint = serviceStatus.dwCheckPoint;
	psaServiceInfo->dwControlsAccepted = serviceStatus.dwControlsAccepted;
	psaServiceInfo->dwCurrentState = serviceStatus.dwCurrentState;
	psaServiceInfo->dwServiceSpecificExitCode = serviceStatus.dwServiceSpecificExitCode;
	psaServiceInfo->dwServiceType = serviceStatus.dwServiceType;
	psaServiceInfo->dwWaitHint = serviceStatus.dwWaitHint;
	psaServiceInfo->dwWin32ExitCode = serviceStatus.dwWin32ExitCode;

	return S_OK;
}



STDMETHODIMP ServiceManager::getServices(int* servicesCount, SAFEARRAY ** psaServiceName, SAFEARRAY ** psaServiceStatus)
{
#ifdef DEBUG
	MessageBox(NULL, "getServices", "QI", MB_OK | MB_SETFOREGROUND);
#endif // DEBUG



	HRESULT hr = S_OK;

	GUID GUID_ENUM_SERVICE_STATUS_PROCESS_UDT = __uuidof(ENUM_SERVICE_STATUS_PROCESS_UDT);

	IRecordInfo* recordInfo;

	hr = GetRecordInfoFromGuids(LIBID_ServiceLib, 1, 0, 0,
		GUID_ENUM_SERVICE_STATUS_PROCESS_UDT, &recordInfo);


	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else 
	{
#ifdef DEBUG == 1
		MessageBox(NULL, "Open SCM sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}

	ENUM_SERVICE_STATUSW service;

	DWORD dwBytesNeeded = 0;
	DWORD dwServicesReturned = 0;
	DWORD dwResumedHandle = 0;

	ENUM_SERVICE_STATUSW* pServices = NULL;


	DWORD dwServiceType = SERVICE_WIN32 | SERVICE_DRIVER;

	BOOL retVal = EnumServicesStatusW(schSCManager, dwServiceType, SERVICE_STATE_ALL,
		&service, sizeof(ENUM_SERVICE_STATUS), &dwBytesNeeded, &dwServicesReturned,
		&dwResumedHandle);

	if (!retVal) 
	{
		// Need big buffer
		if (ERROR_MORE_DATA == GetLastError()) 
		{
			// Set the buffer
			DWORD dwBytes = sizeof(ENUM_SERVICE_STATUSW) + dwBytesNeeded;
			pServices = new ENUM_SERVICE_STATUSW[dwBytes];
			// Now query again for services
			EnumServicesStatusW(schSCManager, SERVICE_WIN32 | SERVICE_DRIVER, SERVICE_STATE_ALL,
				pServices, dwBytes, &dwBytesNeeded, &dwServicesReturned, &dwResumedHandle);
			// now traverse each service to get information

			SAFEARRAYBOUND bounds;

			bounds.cElements = dwServicesReturned;
			bounds.lLbound = 0;

			SAFEARRAYBOUND bounds_;

			bounds_.cElements = dwServicesReturned;
			bounds_.lLbound = 0;

			*psaServiceName = SafeArrayCreate(VT_BSTR, 1, &bounds);
			const int iLBound = 0, iUBound = dwServicesReturned;

			SAFEARRAY* psaEnumServiceStatus = SafeArrayCreateVectorEx(VT_RECORD, iLBound,
				iUBound - iLBound + 1, recordInfo);

			ENUM_SERVICE_STATUS_PROCESS_UDT* pServiceStatus = NULL;

			hr = SafeArrayAccessData(psaEnumServiceStatus, (void**)&pServiceStatus);

			long int i = 0;
			long int j = 0;
			long int k = 0;


			for (unsigned iIndex = 0; iIndex < dwServicesReturned; iIndex++) 
			{

				BSTR serviceName = SysAllocString((pServices + iIndex)->lpServiceName);

				hr = SafeArrayPutElement(*psaServiceName, &i, serviceName); 

				pServiceStatus[iIndex].lpServiceName = SysAllocString((pServices + iIndex)->lpServiceName);
				pServiceStatus[iIndex].lpDisplayName = SysAllocString((pServices + iIndex)->lpDisplayName);

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
		else 
		{
			ErrorDescription(GetLastError());
		}

		return S_OK;
	}

	if (!CloseServiceHandle(schSCManager)) 
	{
		ErrorDescription(GetLastError());
	}

	else 
	{
	}

	return 0;
}


STDMETHODIMP ServiceManager::enumDependentServicesSM(int* servicesCount, BSTR serviceName, SAFEARRAY ** psaDependentServices)
{
#ifdef DEBUG
	MessageBox(NULL, "enumDependentServicesSM", "QI", MB_OK | MB_SETFOREGROUND);
#endif // DEBUG

	HRESULT hr = S_OK;

	GUID GUID_ENUM_SERVICE_STATUS_PROCESS_UDT = __uuidof(ENUM_SERVICE_STATUS_PROCESS_UDT);

	IRecordInfo* recordInfo;

	hr = GetRecordInfoFromGuids(LIBID_ServiceLib, 1, 0, 0,
		GUID_ENUM_SERVICE_STATUS_PROCESS_UDT, &recordInfo);


	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}
	else 
	{
	}

	SC_HANDLE schService = OpenServiceW(schSCManager, serviceName, SERVICE_ENUMERATE_DEPENDENTS);
	if (NULL == schService) 
	{
		ErrorDescription(GetLastError());
		return -1;
	}

	else 
	{
#ifdef DEBUG
		MessageBox(NULL, "Open Service sucessfully", "QI", MB_OK | MB_SETFOREGROUND);
#endif
	}


	DWORD dwServicesReturned = 0;
	DWORD dwBytesNeeded = 0;
	DWORD dwBufSize = 0;
	LPENUM_SERVICE_STATUSW lpDependencies = NULL;

	BOOL retVal = EnumDependentServicesW(schService, SERVICE_STATE_ALL, NULL,
		0, &dwBytesNeeded, &dwServicesReturned);

	if (!retVal) 
	{
		if (ERROR_MORE_DATA == GetLastError()) 
		{
			lpDependencies = (LPENUM_SERVICE_STATUSW)HeapAlloc(GetProcessHeap(), 0,	dwBytesNeeded);

			if (!EnumDependentServicesW(schService, SERVICE_STATE_ALL, lpDependencies,
				dwBytesNeeded, &dwBytesNeeded, &dwServicesReturned))
			{
				ErrorDescription(GetLastError());
				return -1;
			}

			SAFEARRAYBOUND bounds;

			bounds.cElements = dwServicesReturned;
			bounds.lLbound = 0;

			*psaDependentServices = SafeArrayCreate(VT_BSTR, 1, &bounds);

			ENUM_SERVICE_STATUS_PROCESS_UDT* pServiceStatus = NULL;

			long int i = 0;

			for (unsigned iIndex = 0; iIndex < dwServicesReturned; iIndex++)
			{
				BSTR serviceName = SysAllocString(lpDependencies[iIndex].lpDisplayName);

				hr = SafeArrayPutElement(*psaDependentServices, &i, serviceName); //Right

				i++;

				SysFreeString(serviceName);
			}

			*servicesCount = dwServicesReturned;
		}
		else 
		{
			ErrorDescription(GetLastError());
		}
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return S_OK;
}


void ErrorDescription(DWORD p_dwError) 
{

	HLOCAL hLocal = NULL;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, p_dwError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPTSTR)&hLocal,
		0, NULL);

	MessageBox(NULL, (LPCTSTR)LocalLock(hLocal), TEXT("Error"), MB_OK | MB_ICONERROR);
	LocalFree(hLocal);
}



BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}