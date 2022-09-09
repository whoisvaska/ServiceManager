//#include "stdafx.h"
#include "service_manager.h"
#include "manager_class_factory.h"
#include "locks.h"

ManagerClassFactory::ManagerClassFactory()
{
	m_refCount = 0;
}

ManagerClassFactory::~ManagerClassFactory()
{
	MessageBox(NULL,
		"ManagerClassFactory is being distructed. Make sure you see this message, if not, you might have memory leak!",
		"Destructor", MB_OK | MB_SETFOREGROUND);
}

STDMETHODIMP_(ULONG) ManagerClassFactory::AddRef()
{
	//return ++m_refCount;
	return 10;
}

STDMETHODIMP_(ULONG) ManagerClassFactory::Release()
{
	/*
	if ( --m_refCount == 0 )
	{
		delete this;
		return 0;
	}
	return m_refCount;
	*/
	return 20;
}

STDMETHODIMP ManagerClassFactory::QueryInterface(REFIID riid, void** pIFace)
{
	if (riid == IID_IUnknown)
		*pIFace = (IUnknown*)this;
	else if (riid == IID_IClassFactory)
		*pIFace = (IClassFactory*)this;
	else
	{
		*pIFace = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*pIFace))->AddRef();
	return S_OK;
}

STDMETHODIMP ManagerClassFactory::LockServer(BOOL fLock)
{
	/*if (fLock) Lock();
	else UnLock()*/;
	return S_OK;
}

STDMETHODIMP ManagerClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv)
{
	if (pUnkOuter != NULL) return CLASS_E_NOAGGREGATION;

	ServiceManager* pServiceManagerObj = NULL;
	HRESULT hr;

	pServiceManagerObj = new ServiceManager();
	hr = pServiceManagerObj->QueryInterface(riid, ppv);

	if (FAILED(hr)) delete pServiceManagerObj;
	return hr;
}
