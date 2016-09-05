

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jun 16 18:24:32 2016
 */
/* Compiler settings for SVCBox.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_SVCBoxLib,0xBF7F61E6,0x2A82,0x4AEE,0xBC,0x25,0x4B,0xF5,0x5D,0x92,0xC4,0x19);


MIDL_DEFINE_GUID(IID, DIID__DSVCBox,0x2273438B,0xC05A,0x432D,0x94,0x3E,0x14,0xC2,0x05,0x91,0x5C,0x12);


MIDL_DEFINE_GUID(IID, DIID__DSVCBoxEvents,0xBA088AF6,0xF635,0x447C,0x99,0xE7,0x83,0x72,0xF6,0x7B,0x1E,0x99);


MIDL_DEFINE_GUID(CLSID, CLSID_SVCBox,0x9175A113,0xDD75,0x4E95,0x8A,0x27,0xD0,0x91,0x7A,0xC9,0xDE,0x10);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



