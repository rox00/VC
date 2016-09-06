#pragma once
#include <comdef.h>
#include <richedit.h>
#include <richole.h>
#include <OleIdl.h>
class ImageDataObject:IDataObject
{
public:
	static void InsertBitmap(IRichEditOle* pRichEditOle,HBITMAP hBitmap);
private:
	ULONG m_ulRefCnt;
	BOOL  m_bRelease;
	STGMEDIUM m_stgmed;
	FORMATETC m_format;
public:
	ImageDataObject():m_ulRefCnt(0)
	{
		m_bRelease = FALSE;
	}
	~ImageDataObject()
	{
		if(m_bRelease)
			::ReleaseStgMedium(&m_stgmed);
	}
	STDMETHOD(QueryInterface)(REFIID iid,void **ppvObject)
	{
		if(iid==IID_IUnknown||iid==IID_IDataObject)
		{
			*ppvObject=this;
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	STDMETHOD_(ULONG,AddRef)(void)
	{
		m_ulRefCnt++;
		return m_ulRefCnt;
	}
	STDMETHOD_(ULONG,Release)(void)
	{
		if(--m_ulRefCnt==0)
			delete this;
		return m_ulRefCnt;
	}
	STDMETHOD(GetData)(FORMATETC *pFormatetc,STGMEDIUM *pMedium)
	{
		HANDLE hDst;
		hDst=::OleDuplicateData(m_stgmed.hBitmap,CF_BITMAP,NULL);
		assert(hDst);
		pMedium->tymed=TYMED_GDI;
		pMedium->hBitmap=(HBITMAP)hDst;
		pMedium->pUnkForRelease=NULL;
		return S_OK;
	}
	STDMETHOD(GetDataHere)(FORMATETC *pFormatetc,STGMEDIUM *pMedium)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(QueryGetData)(FORMATETC *pFormatetc)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *pFormatetcIn,FORMATETC *pFormatetcOut)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetData)(FORMATETC *pFormatetc,STGMEDIUM *pMedium,BOOL fRelase)
	{
		m_format=*pFormatetc;
		m_stgmed=*pMedium;
		return S_OK;
	}
	STDMETHOD(EnumFormatEtc)(DWORD dwDirection,IEnumFORMATETC **ppEnumFormatEtc)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DAdvise)(FORMATETC *pFormatetc,DWORD dwDvf,IAdviseSink *pAdvSink,
		DWORD *pdwConnection)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DUnadvise)(DWORD dwConnection)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppEnumAdvise)
	{
		return E_NOTIMPL;
	}
	void SetBitmap(HBITMAP hBitmap);
	IOleObject* GetOleObject(IOleClientSite *pOleClientSite,IStorage *pStorage);
};