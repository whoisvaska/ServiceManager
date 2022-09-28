
    struct SERVICE_STATUS_PROCESS_UDT
    {
        DWORD   dwServiceType;
        DWORD   dwCurrentState;
        DWORD   dwControlsAccepted;
        DWORD   dwWin32ExitCode;
        DWORD   dwServiceSpecificExitCode;
        DWORD   dwCheckPoint;
        DWORD   dwWaitHint;
    };


    
    struct ENUM_SERVICE_STATUS_PROCESS_UDT
    {
        struct SERVICE_STATUS_PROCESS_UDT ServiceStatus;
        BSTR          lpServiceName;
        BSTR          lpDisplayName;
    };


    
    struct QUERY_SERVICE_CONFIG_UDT
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
    };

    struct SC_ACTION_UDT {
        DWORD Type;
        DWORD Delay;
    };

    struct SERVICE_FAILURE_ACTIONS_UDT {
        DWORD     dwResetPeriod;
        DWORD     cActions;
        struct SC_ACTION_UDT lpsaAction1;
        struct SC_ACTION_UDT lpsaAction2;
        struct SC_ACTION_UDT lpsaAction3;
        BSTR     lpRebootMsg;
        BSTR     lpCommand;
        int      fFailureActionsOnNonCrashFailures;
    };

    struct SERVICE_DESCRIPTION_UDT {
        BSTR lpDescription;
    };

    interface IManager : IUnknown
    {
        HRESULT getServices([out] int* servicesCount, [out] SAFEARRAY(BSTR)* psaServiceName, [out] SAFEARRAY(struct SERVICE_STATUS_PROCESS_UDT) * psaServiceStatus);
        
        HRESULT queryServiceInfo([in] BSTR serviceName, [out] struct SERVICE_STATUS_PROCESS_UDT* psaServiceInfo);

        HRESULT queryServiceConfigSM([in] BSTR serviceName, [out] struct QUERY_SERVICE_CONFIG_UDT* serviceConfig);
        
        HRESULT queryServiceFailureActionsSM([in] BSTR serviceName, [out] struct SERVICE_FAILURE_ACTIONS_UDT* serviceConfig);

        HRESULT changeServiceFailureActionsSM([in] BSTR serviceName, [in, out] struct SERVICE_FAILURE_ACTIONS_UDT* serviceConfig);

        HRESULT queryServiceDescriptionSM([in] BSTR serviceName, [out] struct SERVICE_DESCRIPTION_UDT* serviceDescription);

        HRESULT enumDependentServicesSM([out] int* servicesCount, [in] BSTR serviceName, [out] SAFEARRAY(BSTR)* psaDependentServices);

        HRESULT changeServiceConfigSM([in] BSTR serviceName, [in, out] struct QUERY_SERVICE_CONFIG_UDT* serviceConfig);

        HRESULT stopService([in] BSTR serviceName);
        HRESULT startService([in] BSTR serviceName);

    };
