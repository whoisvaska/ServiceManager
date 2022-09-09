// the class object (class factory) for CoMyCar class

#if !defined(ManagerClassFactory_H)
#define ManagerClassFactory_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif