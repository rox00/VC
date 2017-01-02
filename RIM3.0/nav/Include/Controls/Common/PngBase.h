//********************************************************************
//	PNGBASE.H �ļ�ע��
//	�ļ��� : PNGBASE.H
//	�ļ�·��: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS\COMMON/
//	���� : ����
//	����ʱ��: 2014/12/3 10:40
//	�ļ�����: 
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


