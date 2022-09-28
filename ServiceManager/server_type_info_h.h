

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 06:14:07 2038
 */
/* Compiler settings for server_type_info.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __server_type_info_h_h__
#define __server_type_info_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IManager_FWD_DEFINED__
#define __IManager_FWD_DEFINED__
typedef interface IManager IManager;

#endif 	/* __IManager_FWD_DEFINED__ */


#ifndef __ICreateManager_FWD_DEFINED__
#define __ICreateManager_FWD_DEFINED__
typedef interface ICreateManager ICreateManager;

#endif 	/* __ICreateManager_FWD_DEFINED__ */


#ifndef __ServiceManager_FWD_DEFINED__
#define __ServiceManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class ServiceManager ServiceManager;
#else
typedef struct ServiceManager ServiceManager;
#endif /* __cplusplus */

#endif 	/* __ServiceManager_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ServiceLib_LIBRARY_DEFINED__
#define __ServiceLib_LIBRARY_DEFINED__

/* library ServiceLib */
/* [helpstring][version][uuid] */ 

/* [uuid] */ struct  DECLSPEC_UUID("D7695D47-090D-4F50-A202-8665C06EEC49") SERVICE_STATUS_PROCESS_UDT
    {
    DWORD dwServiceType;
    DWORD dwCurrentState;
    DWORD dwControlsAccepted;
    DWORD dwWin32ExitCode;
    DWORD dwServiceSpecificExitCode;
    DWORD dwCheckPoint;
    DWORD dwWaitHint;
    } ;
/* [uuid] */ struct  DECLSPEC_UUID("B2A07117-3FA6-479A-9A7B-A6414DC4C8F7") ENUM_SERVICE_STATUS_PROCESS_UDT
    {
    struct SERVICE_STATUS_PROCESS_UDT ServiceStatus;
    BSTR lpServiceName;
    BSTR lpDisplayName;
    } ;
/* [uuid] */ struct  DECLSPEC_UUID("B2A07117-3FA6-479A-9A7B-A6414DC4C2FA") QUERY_SERVICE_CONFIG_UDT
    {
    DWORD dwServiceType;
    DWORD dwStartType;
    DWORD dwErrorControl;
    DWORD dwTagId;
    BSTR lpBinaryPathName;
    BSTR lpLoadOrderGroup;
    BSTR lpDependencies;
    BSTR lpServiceStartName;
    BSTR lpDisplayName;
    } ;
struct SC_ACTION_UDT
    {
    DWORD Type;
    DWORD Delay;
    } ;
struct SERVICE_FAILURE_ACTIONS_UDT
    {
    DWORD dwResetPeriod;
    DWORD cActions;
    struct SC_ACTION_UDT lpsaAction1;
    struct SC_ACTION_UDT lpsaAction2;
    struct SC_ACTION_UDT lpsaAction3;
    BSTR lpRebootMsg;
    BSTR lpCommand;
    int fFailureActionsOnNonCrashFailures;
    } ;
struct SERVICE_DESCRIPTION_UDT
    {
    BSTR lpDescription;
    } ;

EXTERN_C const IID LIBID_ServiceLib;

#ifndef __IManager_INTERFACE_DEFINED__
#define __IManager_INTERFACE_DEFINED__

/* interface IManager */
/* [helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("993CD48A-7763-4C8E-A919-BCB81CDFCAD4")
    IManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getServices( 
            /* [out] */ int *servicesCount,
            /* [out] */ SAFEARRAY * *psaServiceName,
            /* [out] */ SAFEARRAY * *psaServiceStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE queryServiceInfo( 
            /* [in] */ BSTR serviceName,
            /* [out] */ struct SERVICE_STATUS_PROCESS_UDT *psaServiceInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE queryServiceConfigSM( 
            /* [in] */ BSTR serviceName,
            /* [out] */ struct QUERY_SERVICE_CONFIG_UDT *serviceConfig) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE queryServiceFailureActionsSM( 
            /* [in] */ BSTR serviceName,
            /* [out] */ struct SERVICE_FAILURE_ACTIONS_UDT *serviceConfig) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE changeServiceFailureActionsSM( 
            /* [in] */ BSTR serviceName,
            /* [out][in] */ struct SERVICE_FAILURE_ACTIONS_UDT *serviceConfig) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE queryServiceDescriptionSM( 
            /* [in] */ BSTR serviceName,
            /* [out] */ struct SERVICE_DESCRIPTION_UDT *serviceDescription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE enumDependentServicesSM( 
            /* [out] */ int *servicesCount,
            /* [in] */ BSTR serviceName,
            /* [out] */ SAFEARRAY * *psaDependentServices) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE changeServiceConfigSM( 
            /* [in] */ BSTR serviceName,
            /* [out][in] */ struct QUERY_SERVICE_CONFIG_UDT *serviceConfig) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE stopService( 
            /* [in] */ BSTR serviceName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE startService( 
            /* [in] */ BSTR serviceName) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *getServices )( 
            IManager * This,
            /* [out] */ int *servicesCount,
            /* [out] */ SAFEARRAY * *psaServiceName,
            /* [out] */ SAFEARRAY * *psaServiceStatus);
        
        HRESULT ( STDMETHODCALLTYPE *queryServiceInfo )( 
            IManager * This,
            /* [in] */ BSTR serviceName,
            /* [out] */ struct SERVICE_STATUS_PROCESS_UDT *psaServiceInfo);
        
        HRESULT ( STDMETHODCALLTYPE *queryServiceConfigSM )( 
            IManager * This,
            /* [in] */ BSTR serviceName,
            /* [out] */ struct QUERY_SERVICE_CONFIG_UDT *serviceConfig);
        
        HRESULT ( STDMETHODCALLTYPE *queryServiceFailureActionsSM )( 
            IManager * This,
            /* [in] */ BSTR serviceName,
            /* [out] */ struct SERVICE_FAILURE_ACTIONS_UDT *serviceConfig);
        
        HRESULT ( STDMETHODCALLTYPE *changeServiceFailureActionsSM )( 
            IManager * This,
            /* [in] */ BSTR serviceName,
            /* [out][in] */ struct SERVICE_FAILURE_ACTIONS_UDT *serviceConfig);
        
        HRESULT ( STDMETHODCALLTYPE *queryServiceDescriptionSM )( 
            IManager * This,
            /* [in] */ BSTR serviceName,
            /* [out] */ struct SERVICE_DESCRIPTION_UDT *serviceDescription);
        
        HRESULT ( STDMETHODCALLTYPE *enumDependentServicesSM )( 
            IManager * This,
            /* [out] */ int *servicesCount,
            /* [in] */ BSTR serviceName,
            /* [out] */ SAFEARRAY * *psaDependentServices);
        
        HRESULT ( STDMETHODCALLTYPE *changeServiceConfigSM )( 
            IManager * This,
            /* [in] */ BSTR serviceName,
            /* [out][in] */ struct QUERY_SERVICE_CONFIG_UDT *serviceConfig);
        
        HRESULT ( STDMETHODCALLTYPE *stopService )( 
            IManager * This,
            /* [in] */ BSTR serviceName);
        
        HRESULT ( STDMETHODCALLTYPE *startService )( 
            IManager * This,
            /* [in] */ BSTR serviceName);
        
        END_INTERFACE
    } IManagerVtbl;

    interface IManager
    {
        CONST_VTBL struct IManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IManager_getServices(This,servicesCount,psaServiceName,psaServiceStatus)	\
    ( (This)->lpVtbl -> getServices(This,servicesCount,psaServiceName,psaServiceStatus) ) 

#define IManager_queryServiceInfo(This,serviceName,psaServiceInfo)	\
    ( (This)->lpVtbl -> queryServiceInfo(This,serviceName,psaServiceInfo) ) 

#define IManager_queryServiceConfigSM(This,serviceName,serviceConfig)	\
    ( (This)->lpVtbl -> queryServiceConfigSM(This,serviceName,serviceConfig) ) 

#define IManager_queryServiceFailureActionsSM(This,serviceName,serviceConfig)	\
    ( (This)->lpVtbl -> queryServiceFailureActionsSM(This,serviceName,serviceConfig) ) 

#define IManager_changeServiceFailureActionsSM(This,serviceName,serviceConfig)	\
    ( (This)->lpVtbl -> changeServiceFailureActionsSM(This,serviceName,serviceConfig) ) 

#define IManager_queryServiceDescriptionSM(This,serviceName,serviceDescription)	\
    ( (This)->lpVtbl -> queryServiceDescriptionSM(This,serviceName,serviceDescription) ) 

#define IManager_enumDependentServicesSM(This,servicesCount,serviceName,psaDependentServices)	\
    ( (This)->lpVtbl -> enumDependentServicesSM(This,servicesCount,serviceName,psaDependentServices) ) 

#define IManager_changeServiceConfigSM(This,serviceName,serviceConfig)	\
    ( (This)->lpVtbl -> changeServiceConfigSM(This,serviceName,serviceConfig) ) 

#define IManager_stopService(This,serviceName)	\
    ( (This)->lpVtbl -> stopService(This,serviceName) ) 

#define IManager_startService(This,serviceName)	\
    ( (This)->lpVtbl -> startService(This,serviceName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IManager_INTERFACE_DEFINED__ */


#ifndef __ICreateManager_INTERFACE_DEFINED__
#define __ICreateManager_INTERFACE_DEFINED__

/* interface ICreateManager */
/* [oleautomation][uuid][object] */ 


EXTERN_C const IID IID_ICreateManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52DA7150-1BC7-4DA1-AD03-CB1147DF9FD9")
    ICreateManager : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ICreateManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICreateManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICreateManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICreateManager * This);
        
        END_INTERFACE
    } ICreateManagerVtbl;

    interface ICreateManager
    {
        CONST_VTBL struct ICreateManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICreateManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICreateManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICreateManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICreateManager_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ServiceManager;

#ifdef __cplusplus

class DECLSPEC_UUID("53BA7C94-C5BC-42D8-933A-BB08F220849E")
ServiceManager;
#endif
#endif /* __ServiceLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


