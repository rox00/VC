//********************************************************************
//	PNGBASE.CPP 文件注释
//	文件名 : PNGBASE.CPP
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	作者 : 张永
//	创建时间: 2014/12/3 11:12
//	文件描述: 
//*********************************************************************	

#include "stdafx.h"
#include "PngBase.h"


// CPngBase



CPngBase::CPngBase()
{

}

CPngBase::~CPngBase()
{
	parentMemDC.DeleteDC();
	parentMemBmp.DeleteObject();
}

// CPngBase message handlers


UINT CPngBase::SafeGetImageWidth(Image* pImage)
{
	if(	pImage == NULL)
		return 0;
	return pImage->GetWidth();
}

UINT CPngBase::SafeGetImageHeight(Image* pImage)
{
	if(	pImage == NULL)
		return 0;
	return pImage->GetHeight();
}

Image* CPngBase::SafeImageFrom(CString str /*= _T("")*/)
{
	if(str.GetLength())
	{
		Image* pImgTemp = Image::FromFile(CStringW(str));
		if(pImgTemp->GetLastStatus() == Ok)
			return pImgTemp;
		else
			return NULL;
	}

	return NULL;
}

void CPngBase::MemPaintParentPng(CWnd *pChildrenWnd, CDC &memDC, const CRect &rcClient, Image* pImage, BOOL bParentFillOrTitle)
{	//本函数是将父窗口的背景图片的相对本控件的位置贴到本控件背景上实现透明效果
	if(	pImage == NULL						|| 
		pImage->GetWidth() == 0				|| 
		pImage->GetHeight() == 0			|| 
		pChildrenWnd == NULL				|| 
		pChildrenWnd->GetSafeHwnd() == NULL ||
		pChildrenWnd->GetParent() == NULL
		)
		return;

	CRect parentRcClient = CRect(0, 0, 0, 0);
	pChildrenWnd->GetParent()->GetClientRect(parentRcClient);

	////【zy 2014-11-24 ↓】	//兼容父窗口缩放
	BOOL bRePaint = FALSE;
	if(parentMemBmp.GetSafeHandle() == NULL || parentMemDC.GetSafeHdc() == NULL)
		bRePaint = TRUE;
	else
	{
		BITMAP bitmap;
		parentMemBmp.GetBitmap(&bitmap);
		if(bitmap.bmWidth < parentRcClient.Width() || bitmap.bmHeight < parentRcClient.Height())
		{
			bRePaint = TRUE;
		}
	}

	if(bRePaint)
	{
		if(parentMemDC.GetSafeHdc())
			parentMemDC.DeleteDC();
		if(parentMemBmp.GetSafeHandle())
			parentMemBmp.DeleteObject();

		parentMemDC.CreateCompatibleDC(&memDC);
		parentMemBmp.CreateCompatibleBitmap(&memDC, parentRcClient.Width(), parentRcClient.Height());
		parentMemDC.SelectObject(&parentMemBmp);
		parentMemDC.SetBkMode(TRANSPARENT);
		Graphics parentGraphics(parentMemDC.GetSafeHdc());
		ImageAttributes imageattributes;
		imageattributes.SetWrapMode(WrapModeTile);

		CStringW	imagePathW	= CStringW(_T(""));
		UINT		iWidth		= 0;
		UINT		iHeight		= 0;
		Rect		desRect		= Rect(0, 0, 0, 0);
		Rect		sourceRectF = Rect(0, 0, 0, 0);

		iWidth		= pImage->GetWidth();
		iHeight		= pImage->GetHeight();
		if(bParentFillOrTitle)
			desRect	= Rect(parentRcClient.left, parentRcClient.top, parentRcClient.Width(), parentRcClient.Height());	//填充
		else
			desRect	= Rect(parentRcClient.left, parentRcClient.top, pImage->GetWidth(), pImage->GetHeight());	//平铺
		sourceRectF	= Rect(0, 0, pImage->GetWidth(), pImage->GetHeight());
		parentGraphics.DrawImage(pImage,desRect,sourceRectF.X,sourceRectF.Y,sourceRectF.Width,sourceRectF.Height,UnitPixel,&imageattributes);

		parentGraphics.ReleaseHDC(parentMemDC.GetSafeHdc());
	}
	////【zy 2014-11-24 ↑】


	CRect sourRect = CRect(0, 0, 0, 0);
	pChildrenWnd->GetWindowRect(sourRect);
	sourRect.left	+= rcClient.left;
	sourRect.top	+= rcClient.top;
	sourRect.right	= sourRect.left + rcClient.Width();
	sourRect.bottom	= sourRect.top + rcClient.Height();
	pChildrenWnd->GetParent()->ScreenToClient(sourRect);

	memDC.StretchBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), 
		&parentMemDC, sourRect.left, sourRect.top, sourRect.Width(), sourRect.Height(), SRCCOPY);
}

void CPngBase::MemPaintPng(const CDC &memDC, const CRect &rcClient, Image* pImage)
{
	if(pImage == NULL || pImage->GetWidth() == 0 || pImage->GetHeight() == 0 )
		return;

	Graphics graphics(memDC.GetSafeHdc());
	ImageAttributes imageattributes;
	imageattributes.SetWrapMode(WrapModeTile);

	CStringW	imagePathW	= CStringW(_T(""));
	UINT		iWidth		= 0;
	UINT		iHeight		= 0;
	Rect		desRect		= Rect(0, 0, 0, 0);
	Rect		sourceRectF = Rect(0, 0, 0, 0);

	iWidth		= pImage->GetWidth();
	iHeight		= pImage->GetHeight();
	desRect		= Rect(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());	//填充
	sourceRectF	= Rect(0, 0, pImage->GetWidth(), pImage->GetHeight());

	graphics.DrawImage(pImage,desRect,sourceRectF.X,sourceRectF.Y,sourceRectF.Width,sourceRectF.Height,UnitPixel,&imageattributes);

	graphics.ReleaseHDC(memDC.GetSafeHdc());
}

void CPngBase::MemDrawText(const CDC &memDC, const CRect &rcClient, const CString &textStr, const DWORD &mode)
{
	CRect rect = rcClient;
	::DrawText(memDC.GetSafeHdc(), textStr, textStr.GetLength(), rect, mode);
}

Bitmap* CPngBase::CutBitmapFromSourRc(Image *pImage, CRect sourRect,  CRect desRect)
{
	if(
		pImage == NULL					|| 
		pImage->GetWidth() == 0			|| 
		pImage->GetHeight() == 0		|| 
		sourRect.Width() == 0			|| 
		sourRect.Height() == 0			|| 
		desRect.Width() == 0			|| 
		desRect.Height() == 0
		)
		return NULL;

	//画原区域
	Bitmap *pBitmapSour = new Bitmap(sourRect.Width(), sourRect.Height());
	Graphics sourGraphics(pBitmapSour);
	sourGraphics.DrawImage(pImage, Rect(0, 0, sourRect.Width(), sourRect.Height()),  0, 0, pImage->GetWidth(), pImage->GetHeight(), UnitPixel);
	sourGraphics.ReleaseHDC(NULL);
	
	//画目的区域
	Bitmap *pBitmapNew = new Bitmap(desRect.Width(), desRect.Height());
	Graphics newGraphics(pBitmapNew);
	newGraphics.DrawImage(pBitmapSour, Rect(0, 0, desRect.Width(), desRect.Height()),  desRect.left, desRect.top, desRect.Width(), desRect.Height(), UnitPixel);
	newGraphics.ReleaseHDC(NULL);

	delete pBitmapSour;
	return pBitmapNew;
}
