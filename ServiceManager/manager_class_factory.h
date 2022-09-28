#pragma once

#include <windows.h>

class ManagerClassFactory : public IClassFactory
{
public:
	ManagerClassFactory();
	virtual ~ManagerClassFactory();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** pIFace);
	STDMETHODIMP_(ULONG)AddRef();
	STDMETHODIMP_(ULONG)Release();

	// IClassFactory
	STDMETHODIMP LockServer(BOOL fLock);
	STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv);

private:

	ULONG m_refCount;

};
