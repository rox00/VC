

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Oct 15 15:12:48 2016
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SVCBoxMonitoridl_h__
#define __SVCBoxMonitoridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSVCBoxMonitor_FWD_DEFINED__
#define ___DSVCBoxMonitor_FWD_DEFINED__
typedef interface _DSVCBoxMonitor _DSVCBoxMonitor;
#endif 	/* ___DSVCBoxMonitor_FWD_DEFINED__ */


#ifndef ___DSVCBoxMonitorEvents_FWD_DEFINED__
#define ___DSVCBoxMonitorEvents_FWD_DEFINED__
typedef interface _DSVCBoxMonitorEvents _DSVCBoxMonitorEvents;
#endif 	/* ___DSVCBoxMonitorEvents_FWD_DEFINED__ */


#ifndef __SVCBoxMonitor_FWD_DEFINED__
#define __SVCBoxMonitor_FWD_DEFINED__

#ifdef __cplusplus
typedef class SVCBoxMonitor SVCBoxMonitor;
#else
typedef struct SVCBoxMonitor SVCBoxMonitor;
#endif /* __cplusplus */

#endif 	/* __SVCBoxMonitor_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SVCBoxMonitorLib_LIBRARY_DEFINED__
#define __SVCBoxMonitorLib_LIBRARY_DEFINED__

/* library SVCBoxMonitorLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_SVCBoxMonitorLib;

#ifndef ___DSVCBoxMonitor_DISPINTERFACE_DEFINED__
#define ___DSVCBoxMonitor_DISPINTERFACE_DEFINED__

/* dispinterface _DSVCBoxMonitor */
/* [uuid] */ 


EXTERN_C const IID DIID__DSVCBoxMonitor;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6F20C68B-855E-481E-9EC4-B938ED234EFE")
    _DSVCBoxMonitor : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSVCBoxMonitorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSVCBoxMonitor * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSVCBoxMonitor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSVCBoxMonitor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSVCBoxMonitor * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSVCBoxMonitor * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSVCBoxMonitor * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSVCBoxMonitor * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSVCBoxMonitorVtbl;

    interface _DSVCBoxMonitor
    {
        CONST_VTBL struct _DSVCBoxMonitorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSVCBoxMonitor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSVCBoxMonitor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSVCBoxMonitor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSVCBoxMonitor_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSVCBoxMonitor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSVCBoxMonitor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSVCBoxMonitor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSVCBoxMonitor_DISPINTERFACE_DEFINED__ */


#ifndef ___DSVCBoxMonitorEvents_DISPINTERFACE_DEFINED__
#define ___DSVCBoxMonitorEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DSVCBoxMonitorEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DSVCBoxMonitorEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C8D375E0-900B-4427-AA44-FD41BC68F53B")
    _DSVCBoxMonitorEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSVCBoxMonitorEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSVCBoxMonitorEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSVCBoxMonitorEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSVCBoxMonitorEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSVCBoxMonitorEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSVCBoxMonitorEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSVCBoxMonitorEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSVCBoxMonitorEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSVCBoxMonitorEventsVtbl;

    interface _DSVCBoxMonitorEvents
    {
        CONST_VTBL struct _DSVCBoxMonitorEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSVCBoxMonitorEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSVCBoxMonitorEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSVCBoxMonitorEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSVCBoxMonitorEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSVCBoxMonitorEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSVCBoxMonitorEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSVCBoxMonitorEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSVCBoxMonitorEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SVCBoxMonitor;

#ifdef __cplusplus

class DECLSPEC_UUID("6BB1915F-3D5E-43AD-B38A-B4ECE51C0BA9")
SVCBoxMonitor;
#endif
#endif /* __SVCBoxMonitorLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


