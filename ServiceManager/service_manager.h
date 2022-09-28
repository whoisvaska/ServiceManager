#pragma once

#include "../ServiceManager/server_type_info_h.h"
//#include "service_struct.h"

class ServiceManager :
	public IManager,
	public ICreateManager
{
public:
	ServiceManager();
	virtual ~ServiceManager();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** pIFace);
	STDMETHODIMP_(DWORD)AddRef();
	STDMETHODIMP_(DWORD)Release();

	// IManager
	STDMETHODIMP getServices(int* servicesCount, SAFEARRAY ** psaServiceName, SAFEARRAY ** psaServiceStatus);
	STDMETHODIMP stopService(BSTR serviceName);
	STDMETHODIMP startService(BSTR serviceName);

	STDMETHODIMP queryServiceInfo(BSTR serviceName, SERVICE_STATUS_PROCESS_UDT* psaServiceInfo);

	STDMETHODIMP queryServiceConfigSM(BSTR serviceName, struct QUERY_SERVICE_CONFIG_UDT* serviceConfig);
	
	STDMETHODIMP queryServiceFailureActionsSM(BSTR serviceName, SERVICE_FAILURE_ACTIONS_UDT* serviceConfig);

	STDMETHODIMP changeServiceFailureActionsSM(BSTR serviceName, struct SERVICE_FAILURE_ACTIONS_UDT* serviceConfig);

	STDMETHODIMP queryServiceDescriptionSM(BSTR serviceName, struct SERVICE_DESCRIPTION_UDT* serviceDescription);
	
	STDMETHODIMP enumDependentServicesSM(int* servicesCount, BSTR serviceName, SAFEARRAY** psaDependentServices);

	STDMETHODIMP changeServiceConfigSM(BSTR serviceName, struct QUERY_SERVICE_CONFIG_UDT* serviceConfig);
	
private:
	DWORD	m_refCount;
};


