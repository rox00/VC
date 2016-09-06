#include "stdafx.h"
#include "ImageDataObject.h"

void ImageDataObject::InsertBitmap(IRichEditOle *pRichEditOle,HBITMAP hBitmap)
{
	SCODE sc;
	ImageDataObject *pods=new ImageDataObject;
	LPDATAOBJECT lpDataObject;
	pods->QueryInterface(IID_IDataObject,(void**)&lpDataObject);
	pods->SetBitmap(hBitmap);
	IOleClientSite *pOleClientSite;
	pRichEditOle->GetClientSite(&pOleClientSite);
	IStorage *pStorage;
	LPLOCKBYTES lpLockBytes=NULL;
	sc=CreateILockBytesOnHGlobal(NULL,TRUE,&lpLockBytes); //创建全局存储
	if(sc!=S_OK)
		throw exception("");
	assert(lpLockBytes);
	sc=StgCreateDocfileOnILockBytes(lpLockBytes,STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,
		0,&pStorage);
	if(sc!=S_OK)
		throw exception("");
	assert(pStorage);
	IOleObject *pOleObject;
	pOleObject=pods->GetOleObject(pOleClientSite,pStorage);
	OleSetContainedObject(pOleObject,TRUE);
	REOBJECT reobject={0};
	reobject.cbStruct=sizeof(reobject);
	CLSID clsid;
	sc=pOleObject->GetUserClassID(&clsid);
	if(sc!=S_OK)
		throw exception("");
	reobject.clsid=clsid;
	reobject.cp=REO_CP_SELECTION;
	reobject.dvaspect=DVASPECT_CONTENT;
	reobject.poleobj=pOleObject;
	reobject.polesite=pOleClientSite;
	reobject.pstg=pStorage;
	pRichEditOle->InsertObject(&reobject);
	pOleObject->Release();
	pOleClientSite->Release();
	pStorage->Release();
	lpDataObject->Release();
}
void ImageDataObject::SetBitmap(HBITMAP hBitmap)
{
	assert(hBitmap);
	STGMEDIUM stgm={0};
	stgm.tymed=TYMED_GDI;
	stgm.hBitmap=hBitmap;
	stgm.pUnkForRelease=NULL;
	FORMATETC fm;
	fm.cfFormat=CF_BITMAP;
	fm.ptd=NULL;
	fm.dwAspect=DVASPECT_CONTENT;
	fm.lindex=-1;
	fm.tymed=TYMED_GDI;
	this->SetData(&fm,&stgm,TRUE);
}
IOleObject* ImageDataObject::GetOleObject(IOleClientSite *pOleClientSite,IStorage *pStorage)
{
	assert(m_stgmed.hBitmap);
	SCODE sc;
	IOleObject *pOleObject;
	sc=OleCreateStaticFromData(this,IID_IOleObject,OLERENDER_FORMAT,
		&m_format,pOleClientSite,pStorage,
		(void**)&pOleObject);
	if(sc!=S_OK)
		throw exception("");
	return pOleObject;
}