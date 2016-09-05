

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Aug 14 00:16:58 2016
 */
/* Compiler settings for SVCBoxMonitor.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


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
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_SVCBoxMonitorLib,0xF7CF1A20,0x6693,0x408C,0x99,0x67,0x0F,0x1C,0x84,0xD8,0x46,0x76);


MIDL_DEFINE_GUID(IID, DIID__DSVCBoxMonitor,0x6F20C68B,0x855E,0x481E,0x9E,0xC4,0xB9,0x38,0xED,0x23,0x4E,0xFE);


MIDL_DEFINE_GUID(IID, DIID__DSVCBoxMonitorEvents,0xC8D375E0,0x900B,0x4427,0xAA,0x44,0xFD,0x41,0xBC,0x68,0xF5,0x3B);


MIDL_DEFINE_GUID(CLSID, CLSID_SVCBoxMonitor,0x6BB1915F,0x3D5E,0x43AD,0xB3,0x8A,0xB4,0xEC,0xE5,0x1C,0x0B,0xA9);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



