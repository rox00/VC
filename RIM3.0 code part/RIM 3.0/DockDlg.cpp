// DockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "DockDlg.h"


// CDockDlg dialog

IMPLEMENT_DYNAMIC(CDockDlg, CDialog)

CDockDlg::CDockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDockDlg::IDD, pParent)
{
	m_pImgLogoTemp		= ImageConfig::GetInstance()->m_pImgLogo;
	m_bMouseMove		= FALSE;
	m_bDisplayChange	= FALSE;
	m_wndAlphaValue		= 255;
}

CDockDlg::~CDockDlg()
{
}

void CDockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDockDlg, CDialog)
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_BN_CLICKED(IDOK, &CDockDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDockDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CDockDlg message handlers


void CDockDlg::UpdateView(Image* pImageLogo, byte Alpha /*= 255*/)
{
	m_wndAlphaValue = Alpha;


	CRect rcClient;
	GetClientRect(&rcClient);

	CRect logoRect;
	logoRect = rcClient;


	CDC			*pDC = GetDC();

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SelectObject(GetFont());


	Graphics graphics(memDC.GetSafeHdc());
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeNone);


	Rect imgLogoRect	= Rect(logoRect.left,	logoRect.top,	pImageLogo->GetWidth(),		pImageLogo->GetHeight());
	graphics.DrawImage(pImageLogo, imgLogoRect);

	//graphics.FillRegion(&SolidBrush(Color::Green), Region::FromHRGN(m_MainPixelRgn));

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	BLENDFUNCTION stBlend = { AC_SRC_OVER, 0, Alpha, AC_SRC_ALPHA };
	::UpdateLayeredWindow(m_hWnd, pDC->GetSafeHdc(), &CPoint(rcWindow.left, rcWindow.top), &CSize(rcWindow.Width(), rcWindow.Height()), 
		memDC.GetSafeHdc(), &CPoint(0, 0), 0, &stBlend, ULW_ALPHA);
	ReleaseDC(pDC);

	graphics.ReleaseHDC(memDC.GetSafeHdc());
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CDockDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CDockDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CWnd *pParent = GetParent();

	if(GetParent())
	{
		GetParent()->PostMessage(WM_COMMAND, IDCANCEL, 0);
		return;
	}
	OnCancel();
}

BOOL CDockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	ModifyStyle(WS_BORDER | WS_SIZEBOX | WS_DLGFRAME | DS_MODALFRAME, WS_POPUP);
	ModifyStyleEx(WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME, WS_EX_LAYERED);

	//::SetLayeredWindowAttributes(m_hWnd, g_transAlphaClr, 0, LWA_COLORKEY); 

	UpdateView(m_pImgLogoTemp, 0);	//停靠初始是隐藏的

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
 	if(pWnd == this)
 		return g_hTransAlphaBrush;
 	else
		return hbr;
}

BOOL CDockDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

void CDockDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

}

void CDockDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonDown(nFlags, point);

	ClientToScreen(&point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); 
}

void CDockDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMouseMove == false)
	{
		//要求接收鼠标离开消息
		TRACKMOUSEEVENT		csTME;
		csTME.cbSize	= sizeof(csTME);
		csTME.dwFlags	= TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&csTME);

		m_bMouseMove = true;
	}

	CDialog::OnMouseMove(nFlags, point);

	//{
	//	CString str;
	//	str.Format(_T("\n(zyo)CDockDlg :IsWindowVisible:  %d, %d \n"), IsWindowVisible(), 0);
	//	OutputDebugString(str);
	//}

	if(GetParent())		//当鼠标离开可显示区域时，发送消息到父窗口判断是否需要停靠到窗口边界上
		GetParent()->PostMessage(WM_SHOWDOCKBOUND, 0, 0);
}

void CDockDlg::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseMove = FALSE;

	CDialog::OnMouseLeave();

	if(GetParent())		//当鼠标离开可显示区域时，发送消息到父窗口判断是否需要停靠到窗口边界上
		GetParent()->PostMessage(WM_SHOWDOCKBOUND, 0, 0);
}

void CDockDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: Add your message handler code here
	if(m_bDisplayChange && GetParent())		//当鼠标离开可显示区域时，发送消息到父窗口判断是否需要停靠到窗口边界上
	{
		GetParent()->PostMessage(WM_SHOWDOCKBOUND, 0, 0);
		m_bDisplayChange = FALSE;
	}
}

LRESULT CDockDlg::OnDisplayChange(WPARAM BitsPerPixel, LPARAM size)
{
	//m_bDisplayChange = TRUE;
	GetParent()->PostMessage(WM_SHOWDOCKBOUND, 0, 0);

	return TRUE;
}