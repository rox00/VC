

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SVCBoxidl_h__
#define __SVCBoxidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSVCBox_FWD_DEFINED__
#define ___DSVCBox_FWD_DEFINED__
typedef interface _DSVCBox _DSVCBox;
#endif 	/* ___DSVCBox_FWD_DEFINED__ */


#ifndef ___DSVCBoxEvents_FWD_DEFINED__
#define ___DSVCBoxEvents_FWD_DEFINED__
typedef interface _DSVCBoxEvents _DSVCBoxEvents;
#endif 	/* ___DSVCBoxEvents_FWD_DEFINED__ */


#ifndef __SVCBox_FWD_DEFINED__
#define __SVCBox_FWD_DEFINED__

#ifdef __cplusplus
typedef class SVCBox SVCBox;
#else
typedef struct SVCBox SVCBox;
#endif /* __cplusplus */

#endif 	/* __SVCBox_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SVCBoxLib_LIBRARY_DEFINED__
#define __SVCBoxLib_LIBRARY_DEFINED__

/* library SVCBoxLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_SVCBoxLib;

#ifndef ___DSVCBox_DISPINTERFACE_DEFINED__
#define ___DSVCBox_DISPINTERFACE_DEFINED__

/* dispinterface _DSVCBox */
/* [uuid] */ 


EXTERN_C const IID DIID__DSVCBox;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2273438B-C05A-432D-943E-14C205915C12")
    _DSVCBox : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSVCBoxVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSVCBox * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSVCBox * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSVCBox * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSVCBox * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSVCBox * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSVCBox * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSVCBox * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSVCBoxVtbl;

    interface _DSVCBox
    {
        CONST_VTBL struct _DSVCBoxVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSVCBox_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSVCBox_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSVCBox_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSVCBox_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSVCBox_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSVCBox_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSVCBox_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSVCBox_DISPINTERFACE_DEFINED__ */


#ifndef ___DSVCBoxEvents_DISPINTERFACE_DEFINED__
#define ___DSVCBoxEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DSVCBoxEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DSVCBoxEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BA088AF6-F635-447C-99E7-8372F67B1E99")
    _DSVCBoxEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSVCBoxEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSVCBoxEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSVCBoxEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSVCBoxEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSVCBoxEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSVCBoxEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSVCBoxEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSVCBoxEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSVCBoxEventsVtbl;

    interface _DSVCBoxEvents
    {
        CONST_VTBL struct _DSVCBoxEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSVCBoxEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSVCBoxEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSVCBoxEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSVCBoxEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSVCBoxEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSVCBoxEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSVCBoxEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSVCBoxEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SVCBox;

#ifdef __cplusplus

class DECLSPEC_UUID("9175A113-DD75-4E95-8A27-D0917AC9DE10")
SVCBox;
#endif
#endif /* __SVCBoxLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


