#include "value_to_str.h"

QString serviceTypeToString(unsigned long dwServiceType) {
    QString serviceType = "";

    switch (dwServiceType) {
    case 0x00000002:
        serviceType = "SERVICE_FILE_SYSTEM_DRIVER";
        break;
    case 0x00000001:
        serviceType = "SERVICE_KERNEL_DRIVER";
        break;
    case 0x00000010:
        serviceType = "SERVICE_WIN32_OWN_PROCESS";
        break;
    case 0x00000020:
        serviceType = "SERVICE_WIN32_SHARE_PROCESS";
        break;
    }

    return serviceType;
}

QString currentStateToString(unsigned long dwCurrentState) {
    QString currentState = "";

    switch (dwCurrentState) {
    case 0x00000005:
        currentState = "SERVICE_CONTINUE_PENDING";
        break;
    case 0x00000006:
        currentState = "SERVICE_PAUSE_PENDING";
        break;
    case 0x00000007:
        currentState = "SERVICE_PAUSED";
        break;
    case 0x00000004:
        currentState = "SERVICE_RUNNING";
        break;
    case 0x00000002:
        currentState = "SERVICE_START_PENDING";
        break;
    case 0x00000003:
        currentState = "SERVICE_STOP_PENDING";
        break;
    case 0x00000001:
        currentState = "SERVICE_STOPPED";
        break;
    }
    
    return currentState;
}