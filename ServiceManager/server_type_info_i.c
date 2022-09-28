

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_ServiceLib,0x6BDAB14C,0x5569,0x4C1A,0x96,0xAD,0x54,0xC6,0xB4,0x8B,0x37,0x64);


MIDL_DEFINE_GUID(IID, IID_IManager,0x993CD48A,0x7763,0x4C8E,0xA9,0x19,0xBC,0xB8,0x1C,0xDF,0xCA,0xD4);


MIDL_DEFINE_GUID(IID, IID_ICreateManager,0x52DA7150,0x1BC7,0x4DA1,0xAD,0x03,0xCB,0x11,0x47,0xDF,0x9F,0xD9);


MIDL_DEFINE_GUID(CLSID, CLSID_ServiceManager,0x53BA7C94,0xC5BC,0x42D8,0x93,0x3A,0xBB,0x08,0xF2,0x20,0x84,0x9E);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



