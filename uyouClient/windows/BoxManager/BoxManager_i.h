

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jun 16 18:30:18 2016
 */
/* Compiler settings for BoxManager.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __BoxManager_i_h__
#define __BoxManager_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBoxFinder_FWD_DEFINED__
#define __IBoxFinder_FWD_DEFINED__
typedef interface IBoxFinder IBoxFinder;
#endif 	/* __IBoxFinder_FWD_DEFINED__ */


#ifndef __BoxFinder_FWD_DEFINED__
#define __BoxFinder_FWD_DEFINED__

#ifdef __cplusplus
typedef class BoxFinder BoxFinder;
#else
typedef struct BoxFinder BoxFinder;
#endif /* __cplusplus */

#endif 	/* __BoxFinder_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IBoxFinder_INTERFACE_DEFINED__
#define __IBoxFinder_INTERFACE_DEFINED__

/* interface IBoxFinder */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IBoxFinder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("47A5E8DE-DCC0-4E41-9F1D-331FCE5802AB")
    IBoxFinder : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCable( 
            /* [out] */ DWORD *nCableId,
            /* [retval][out] */ BSTR *bstrCableName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AttachDevice( 
            /* [in] */ BSTR lpszCableName,
            /* [in] */ LONG hWnd) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DetachDevice( 
            /* [in] */ BSTR lpszCableName,
            /* [in] */ LONG hWnd) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBoxFinderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBoxFinder * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBoxFinder * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBoxFinder * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBoxFinder * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBoxFinder * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBoxFinder * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBoxFinder * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCable )( 
            IBoxFinder * This,
            /* [out] */ DWORD *nCableId,
            /* [retval][out] */ BSTR *bstrCableName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AttachDevice )( 
            IBoxFinder * This,
            /* [in] */ BSTR lpszCableName,
            /* [in] */ LONG hWnd);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DetachDevice )( 
            IBoxFinder * This,
            /* [in] */ BSTR lpszCableName,
            /* [in] */ LONG hWnd);
        
        END_INTERFACE
    } IBoxFinderVtbl;

    interface IBoxFinder
    {
        CONST_VTBL struct IBoxFinderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBoxFinder_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBoxFinder_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBoxFinder_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBoxFinder_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBoxFinder_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBoxFinder_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBoxFinder_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBoxFinder_GetCable(This,nCableId,bstrCableName)	\
    ( (This)->lpVtbl -> GetCable(This,nCableId,bstrCableName) ) 

#define IBoxFinder_AttachDevice(This,lpszCableName,hWnd)	\
    ( (This)->lpVtbl -> AttachDevice(This,lpszCableName,hWnd) ) 

#define IBoxFinder_DetachDevice(This,lpszCableName,hWnd)	\
    ( (This)->lpVtbl -> DetachDevice(This,lpszCableName,hWnd) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBoxFinder_INTERFACE_DEFINED__ */



#ifndef __BoxManagerLib_LIBRARY_DEFINED__
#define __BoxManagerLib_LIBRARY_DEFINED__

/* library BoxManagerLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_BoxManagerLib;

EXTERN_C const CLSID CLSID_BoxFinder;

#ifdef __cplusplus

class DECLSPEC_UUID("448D27BC-A872-4ED2-8A0C-9DDC1F204F05")
BoxFinder;
#endif
#endif /* __BoxManagerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


