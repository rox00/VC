//********************************************************************
//	PNGBASE.H 文件注释
//	文件名 : PNGBASE.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	作者 : 张永
//	创建时间: 2014/12/3 10:40
//	文件描述: 
//*********************************************************************	
#pragma once


#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// CPngBase

class CPngBase
{
public:
	CPngBase();
	virtual ~CPngBase();

protected:
	CDC											parentMemDC;
	CBitmap										parentMemBmp;

	UINT 										SafeGetImageWidth(Image* pImage);
	UINT 										SafeGetImageHeight(Image* pImage);
	Image* 										SafeImageFrom(CString str = _T(""));
	void 										MemPaintParentPng(CWnd *pChildrenWnd, CDC &memDC, const CRect &rcClient, Image* pImage, BOOL bParentFillOrTitle);
	void 										MemPaintPng(const CDC &memDC, const CRect &rcClient, Image* pImage);
	void 										MemDrawText(const CDC &memDC, const CRect &rcClient, const CString &textStr, const DWORD &mode);
	Bitmap*										CutBitmapFromSourRc(Image *pImage, CRect sourRect, CRect desRect);

};


