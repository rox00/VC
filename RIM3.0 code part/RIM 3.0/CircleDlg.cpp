//********************************************************************
//	CIRCLEDLG.CPP 文件注释
//	文件名		: CIRCLEDLG.CPP
//	文件路径	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	作者		: 张永
//	创建时间	: 2014/12/16 9:36
//	文件描述	: 
//*********************************************************************	
// CircleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "CircleDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>


// CCircleDlg dialog


IMPLEMENT_DYNAMIC(CCircleDlg, CDialog)

CCircleDlg::CCircleDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCircleDlg::IDD, pParent)
{
	m_bDisplayChange				= FALSE;
	m_wndAlphaValue					= 0;
	m_pImgCircleTemp				= ImageConfig::GetInstance()->m_pImgCircle;
	m_pImgLogoTemp					= ImageConfig::GetInstance()->m_pImgLogo;

	m_bAlreadyLogon					= FALSE;
	m_pSubMoudleInfoHover			= NULL;
	m_bDrag							= FALSE;

	m_bMouseMove					= FALSE;
	m_bShowMainWnd					= FALSE;

	m_startAngle					= 10;				//这里仅做为顺时针应当开始显示的初始角度
	m_stepAngle						= 34;
	m_angleMax						= 240;

	m_offSetAngle					= m_startAngle;		//初始实际应该偏移的角度
	m_bDrag							= FALSE;

	m_showText						= _T("欢迎使用RIM3.0");
}

CCircleDlg::~CCircleDlg()
{
	m_moudleShowArrayTemp.clear();
}

void CCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCircleDlg, CDialog)
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_BN_CLICKED(IDOK, &CCircleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCircleDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_NCHITTEST()
	ON_WM_SIZE()

	ON_WM_DESTROY()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CCircleDlg message handlers
void CCircleDlg::Init()
{
	////【zy 2015-01-05 】根据图片路径进行初始化
	DWORD fuLoad					= LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_SHARED;
	m_handDo						= (HCURSOR)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\handdown.cur")).c_str(), IMAGE_CURSOR, 0, 0, fuLoad);
	m_handUndo						= (HCURSOR)LoadImage((HINSTANCE)&__ImageBase, RelativeToFullPath(_T("res\\PNG\\handundown.cur")).c_str(), IMAGE_CURSOR, 0, 0, fuLoad);

	//EnableToolTips();
	m_pngToolTipCtrl.Create(this, TTS_ALWAYSTIP);
	m_pngToolTipCtrl.AddTool(this, _T(""));	//添加一个工具窗口，每增加一个就会生成 一个窗口

}

void CCircleDlg::ReUpdateFromConfig()
{
	ReInitMoudleShowArray();
	RepositionSubMoudle();

	if(GetSafeHwnd())
		Invalidate(FALSE);
}

void CCircleDlg::ReInitMoudleShowArray()
{
	m_moudleShowArrayTemp.clear();

	BOOL bOffLine = (RIMConfig::GetInstance()->m_pLogonUser->userTypeID == USERINFO::USER_ID_UNKNOWN);

	for (UINT i = 0; i < RIMConfig::GetInstance()->m_moudleInfoArray.size(); ++i)
	{
		if(RIMConfig::GetInstance()->m_moudleInfoArray[i]->bShowInList)
		{
			if(bOffLine)
			{	////脱机登录
				if(RIMConfig::GetInstance()->m_moudleInfoArray[i]->bSupportOutLine && RIMConfig::GetInstance()->m_moudleInfoArray[i]->bShowInList)
					m_moudleShowArrayTemp.push_back(RIMConfig::GetInstance()->m_moudleInfoArray[i]);
			}
			else
			{	////非脱机
				if(RIMConfig::GetInstance()->m_moudleInfoArray[i]->bShowInList)
					m_moudleShowArrayTemp.push_back(RIMConfig::GetInstance()->m_moudleInfoArray[i]);
			}
		}
	}

	//switch(m_moudleShowArrayTemp.size())
	//{
	//case 1:
	//	m_startAngle = m_angleMax/2;
	//	break;
	//case 2:
	//case 3:
	//case 4:
	//case 5:
	//case 6:
	//	m_stepAngle		= m_angleMax/(m_moudleShowArrayTemp.size() + 1);
	//	m_offSetAngle	= m_stepAngle - 1;
	//	break;
	//}

	if(m_moudleShowArrayTemp.size() < 7 && m_moudleShowArrayTemp.size() > 0)
		m_stepAngle = (m_angleMax - m_startAngle)/m_moudleShowArrayTemp.size();
}

void CCircleDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CCircleDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	if (GetParent())
	{
		GetParent()->PostMessage(WM_COMMAND, IDCANCEL, 0);
		return;
	}

	OnCancel();
}

HBRUSH CCircleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if (pWnd == this)
		return g_hTransAlphaBrush;
	else
		return hbr;
}

BOOL CCircleDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

void CCircleDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	dc.SelectClipRgn(&m_wndRoleRgn);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SelectObject(GetFont());
	memDC.FillSolidRect(rcClient, g_transAlphaClr);

	TextureBrush textBrush(m_pImgCircleTemp, WrapModeTile);
	Graphics graphics(memDC.GetSafeHdc());
	graphics.FillRegion(&textBrush, Region::FromHRGN(m_wndRoleRgn));

	if(m_bAlreadyLogon)
	{
		CPoint ptCur;
		GetCursorPos(&ptCur);
		ScreenToClient(&ptCur);
		if (m_bMouseMove)
		{
			SolidBrush solidbrush(Color(128, 209, 255, 255));
			graphics.FillRegion(&solidbrush, Region::FromHRGN(m_wndRoleRgn));
		}

		DWORD count = m_moudleShowArrayTemp.size();
		for (UINT i = 0; i < count; i++)
		{
			SUBMOUDLEINFO *pSubMoudleInfo = m_moudleShowArrayTemp[i];
			if (pSubMoudleInfo->rect.Width())
			{
				if (pSubMoudleInfo == m_pSubMoudleInfoHover && pSubMoudleInfo->rect.PtInRect(ptCur))
				{
					CRect tempRect = pSubMoudleInfo->rect;// tempRect.InflateRect(3, 3);
					MemPaintPng(memDC, tempRect, pSubMoudleInfo->pImagesel);
				}
				else
				{
					MemPaintPng(memDC, pSubMoudleInfo->rect, pSubMoudleInfo->pImageUnsel);
				}
			}
		}
	}
	else
	{	////特殊情况，特殊绘制
		CStringW	strText			= CStringW(RIMConfig::GetInstance()->m_welcomeText.c_str());
		double		stepAngle		= 34;
		double		offSetAngle		= 15;
		double		maxAngle		= 230;
		float		fontSize		= float(RIMConfig::GetInstance()->m_welcomeFontSize);
		///////这里计算间隔
		stepAngle = (maxAngle - offSetAngle) * 1.0 /(strText.GetLength());

		double imgStepRange		= (m_pImgCircleTemp->GetWidth() - m_pImgLogoTemp->GetWidth()) / 2.0;	//圆环的宽度

		const PointF startPt	= PointF(float(imgStepRange / 2.0), float(m_pImgCircleTemp->GetHeight() / 2.0));
		const PointF centerPt	= PointF(float(m_pImgCircleTemp->GetWidth() / 2.0), float(m_pImgCircleTemp->GetHeight() / 2.0));

		Matrix	matrix;

		int		startIndex		= abs(int(offSetAngle / stepAngle));
		double	stepOff			= int(offSetAngle) % int(stepAngle);
		double	curAngle		= stepOff;

		Graphics graphics(memDC.GetSafeHdc());
		graphics.SetSmoothingMode(SmoothingModeHighQuality);
		graphics.SetTextRenderingHint(TextRenderingHintSingleBitPerPixel);

		StringFormat stringformat;
		stringformat.SetAlignment(StringAlignmentNear);
		stringformat.SetLineAlignment(StringAlignmentNear);

		FontFamily		faimily(L"微软雅黑");
		Gdiplus::Font	font(&faimily, fontSize, FontStyleRegular, UnitPixel);

		int count = strText.GetLength();
		for (int i = 0; i < count; i++)
		{
			if (i >= startIndex && curAngle < maxAngle)
			{
				matrix.Reset();
				matrix.Translate(float(startPt.X), float(startPt.Y), MatrixOrderAppend);
				matrix.RotateAt(float(curAngle), PointF(float(centerPt.X), float(centerPt.Y)), MatrixOrderAppend);
				PointF btnPt 	= PointF(matrix.OffsetX(), matrix.OffsetY());

				CStringW str 	= CStringW(strText[i]);

				GraphicsPath	path;
				Rect			bound;

				path.AddString(str, -1, &faimily, FontStyleBold, font.GetSize() * 96.0f / 72.0f, Point(0, 0), &stringformat);
				path.GetBounds(&bound);

				matrix.Reset();
				matrix.Translate(btnPt.X - bound.X - bound.Width / 2.0f, btnPt.Y - bound.Y - bound.Height / 2.0f);
				path.Transform(&matrix);

				float ra = 0;
				{
					float ret;
					if(btnPt.X == centerPt.X)		
						ret = 90;
					else if(btnPt.Y == centerPt.Y) 
						ret = 0;
					else	
						ret = (float)(atan((centerPt.Y - btnPt.Y) / (centerPt.X - btnPt.X)) * 180 / M_PI);
					ra = ret + (btnPt.X > centerPt.X ? 90 : -90);
				}

				matrix.Reset(); //////先将文字的中心移动到预定位置
				matrix.RotateAt(ra, PointF(btnPt.X, btnPt.Y));
				path.Transform(&matrix);
				graphics.FillPath(&SolidBrush(Color::Black), &path);

				curAngle += stepAngle; 
			}
		}
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
}

BOOL CCircleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	ModifyStyle(WS_BORDER | WS_SIZEBOX | WS_DLGFRAME | DS_MODALFRAME, WS_POPUP);
	ModifyStyleEx(WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME, WS_EX_LAYERED);

	SetLayeredAttributes(g_transAlphaClr, 255, LWA_COLORKEY | LWA_ALPHA);

	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCircleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetCapture();
	if(m_bAlreadyLogon)
	{	//只有登录的情况下才能进行拖动及打开子系统
		if (m_pSubMoudleInfoHover)
		{
			clock_t clkTemp = clock();

			MSG msg;
			while(::GetMessage(&msg, NULL, 0, 0))
			{
				if (msg.hwnd == m_hWnd)
				{
					if(msg.message == WM_MOUSEMOVE && (clock() - clkTemp > 300))
					{	//如果移动就不是启动子系统了 300毫秒以内的WM_MOUSEMOVE不处理
						CPoint ptCur = CPoint(0, 0);
						ptCur.x = LOWORD(msg.lParam);
						ptCur.y = HIWORD(msg.lParam);

						::SetCursor(m_handDo);

						m_bDrag = TRUE;
						m_dragPt = ptCur;

						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}

					if(msg.message == WM_LBUTTONUP)
					{
						if(clock() - clkTemp < 300)
						{
							OpenSubSys(m_pSubMoudleInfoHover);
						}

						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}
				}
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else
		{
			if (!m_wndLogoRgn.PtInRegion(point))
			{
				::SetCursor(m_handDo);

				m_bDrag = TRUE;
				m_dragPt = point;
			}
		}
	}
}

void CCircleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bDrag = FALSE;
	RedrawWindow(NULL, &m_wndRoleRgn, RDW_INVALIDATE | RDW_UPDATENOW);
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CCircleDlg::RepositionSubMoudle(float offAngle/* = 0*/)
{
	DWORD imgStepRange = (m_pImgCircleTemp->GetWidth() - m_pImgLogoTemp->GetWidth()) / 2;	//圆环的宽度

	const CPoint startPt = CPoint(imgStepRange / 2, m_pImgCircleTemp->GetHeight() / 2);
	const CPoint centerPt = CPoint(m_pImgCircleTemp->GetWidth() / 2, m_pImgCircleTemp->GetHeight() / 2);

	Matrix matrix;

	int startIndex	= abs(int(m_offSetAngle / m_stepAngle));
	double stepOff	= int(m_offSetAngle) % int(m_stepAngle);
	double curAngle	= stepOff;

	int count = m_moudleShowArrayTemp.size();
	for (int i = 0; i < count; i++)
	{
		SUBMOUDLEINFO *pSubMoudleInfo = m_moudleShowArrayTemp[i];
		if (i >= startIndex && curAngle < m_angleMax)
		{
			matrix.Reset();
			matrix.Translate(float(startPt.x), float(startPt.y), MatrixOrderAppend);
			matrix.RotateAt(float(curAngle), PointF(float(centerPt.x), float(centerPt.y)), MatrixOrderAppend);
			CPoint btnPt = CPoint(int(matrix.OffsetX()), int(matrix.OffsetY()));

			pSubMoudleInfo->rect = CRect(btnPt.x - imgStepRange / 4, btnPt.y - imgStepRange / 4, btnPt.x + imgStepRange / 4, btnPt.y + imgStepRange / 4);

			curAngle += m_stepAngle; 
		}
		else
		{
			pSubMoudleInfo->rect = CRect(0, 0, 0, 0);
		}
	}
}

void CCircleDlg::OpenSubSys(LPSUBMOUDLEINFO pSubMoudleInfo)
{
	if(pSubMoudleInfo == NULL || pSubMoudleInfo->moudleEXEPath.size() == 0)
		return;

	if(PathIsURL(pSubMoudleInfo->moudleEXEPath.c_str()))
	{
		ShellExecute(NULL, _T("open"), pSubMoudleInfo->moudleEXEPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	else 
	{
		tstring strfullPath = RelativeToFullPath(pSubMoudleInfo->moudleEXEPath);
		if(strfullPath.size() && IsFileExist(strfullPath.c_str()))
		{
				{	/////第一次启动默认启动第一个站点的第一个设备的第一个功能
					////////////Test 【zy 2015-05-26 ↓】
					DWORD data[3] = {0};
					BOOL bGoON = TRUE;
					for(UINT i = 0; i < RIMConfig::GetInstance()->m_stationArray.size() && bGoON; ++i)
					{
						LPSTATIONINFO pStationInfo = RIMConfig::GetInstance()->m_stationArray.at(i);
						data[0] = atoi(pStationInfo->pRimStationTemp->sStationNO.c_str());
						for(UINT j = 0; j < pStationInfo->devicesInfos.size() && bGoON; ++j)
						{
							LPDEVICEINFO pDeviceInfo = pStationInfo->devicesInfos.at(j);
							data[1] = atoi(pDeviceInfo->pRimDeviceTemp->sDeviceNO.c_str());
							for(UINT k = 0; k < pDeviceInfo->measureInfos.size() && bGoON; ++k)
							{
								LPMEASUREINFO pMeasureInfo = pDeviceInfo->measureInfos.at(k);
								data[2] = pMeasureInfo->pRtmDeviceFunTemp->nFunNO;
								bGoON = FALSE;
							}
						}
					}

					LPRIM30MSG pMsgTemp = RIM30MSG::MakeMsg(RIM30GUID_INITPROCESS, RIM30GUID_INITPROCESS, RIM30MSG_MEASUREDF_START_ASK, 
						RIM30MSG::DATATYPE_BINARY, (LPBYTE)data, sizeof(data));

					////////////Test 【zy 2015-05-26 ↑】
					GetRIM30Server()->OpenSubSystem(string(CT2A(strfullPath.c_str())), pMsgTemp);
				}

			//SHELLEXECUTEINFO ShExecInfo = { 0 };
			//ShExecInfo.cbSize			= sizeof(SHELLEXECUTEINFO);
			//ShExecInfo.fMask			= SEE_MASK_NOCLOSEPROCESS;
			//ShExecInfo.hwnd				= NULL;
			//ShExecInfo.lpVerb			= NULL;
			//ShExecInfo.lpFile			= strfullPath.c_str();
			//ShExecInfo.lpDirectory		= NULL;
			//ShExecInfo.nShow			= SW_HIDE;
			//ShExecInfo.hInstApp 		= NULL;

			//BOOL hr = ShellExecuteEx(&ShExecInfo);
		}
	}
}

BOOL CCircleDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	if(m_bAlreadyLogon)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		if (m_bDrag == FALSE)
		{
			if (m_pSubMoudleInfoHover)
			{
				HCURSOR   hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
				::SetCursor(hCursor);
				return TRUE;
			}
			else if (m_wndRoleRgn.PtInRegion(point))
			{
				::SetCursor(m_handUndo);
				return TRUE;
			}
		}
		else
		{
			::SetCursor(m_handUndo);
		}
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CCircleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bMouseMove == false)
	{
		//要求接收鼠标离开消息
		TRACKMOUSEEVENT		csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&csTME);

		m_bMouseMove = true;
		RedrawWindow(NULL, &m_wndRoleRgn, RDW_INVALIDATE | RDW_UPDATENOW);
	}


	if (m_bDrag == FALSE)
	{
		CRect tempRect;

		if (!(m_pSubMoudleInfoHover && m_pSubMoudleInfoHover->rect.PtInRect(point)))//之前没有停留过
		{	//不在原来的区域了。
			SUBMOUDLEINFO *pSubMoudleInfoTemp = m_pSubMoudleInfoHover;

			DWORD count = m_moudleShowArrayTemp.size();
			for (UINT i = 0; i < count; i++)
			{
				SUBMOUDLEINFO *pSubMoudleInfo = m_moudleShowArrayTemp[i];
				if (pSubMoudleInfo && pSubMoudleInfo->rect.Width())
				{
					if (pSubMoudleInfo->rect.PtInRect(point) && m_pSubMoudleInfoHover != pSubMoudleInfo)
					{
						tempRect = pSubMoudleInfo->rect; tempRect.InflateRect(3, 3);
						InvalidateRect(tempRect, FALSE);
						m_pSubMoudleInfoHover = pSubMoudleInfo;

						//有选择变更时要更新模块说明提示
						m_pngToolTipCtrl.UpdateTipText( m_pSubMoudleInfoHover->moudleName.c_str(), this);
					}
				}
			}

			if (pSubMoudleInfoTemp != m_pSubMoudleInfoHover && pSubMoudleInfoTemp)	//选中了其它的
			{
				tempRect = pSubMoudleInfoTemp->rect; tempRect.InflateRect(3, 3);
				InvalidateRect(tempRect, FALSE);	//把之前的状态更新过来
			}
			else if (pSubMoudleInfoTemp == m_pSubMoudleInfoHover)	//没选中其它的
			{
				m_pSubMoudleInfoHover = NULL;
				if (pSubMoudleInfoTemp)
				{
					tempRect = pSubMoudleInfoTemp->rect; tempRect.InflateRect(3, 3);
					InvalidateRect(tempRect, FALSE);	//把之前的状态更新过来
				}

			}
		}
	}
	else
	{
		if(m_moudleShowArrayTemp.size())
		{
			const CPoint centerPt = CPoint(m_pImgCircleTemp->GetWidth() / 2, m_pImgCircleTemp->GetHeight() / 2);

			double curAngle = atan2(double(centerPt.y - point.y), double(point.x - centerPt.x)) * 180 / M_PI;	//Y轴向上为正  第12象限 0~180 第34象限-180~0象限
			double startAngle = atan2(double(centerPt.y - m_dragPt.y), double(m_dragPt.x - centerPt.x)) * 180 / M_PI;

			double offAngle = 0;										//逆时针移动的角度
			if (point.y < centerPt.y && m_dragPt.y < centerPt.y)		//都在上面0~180
			{
				offAngle = curAngle - startAngle;						//越来越大
			}
			else if (point.y > centerPt.y && m_dragPt.y > centerPt.y)	//都在下面-180~0
			{
				offAngle = curAngle - startAngle;						//越来越大
			}
			else if (point.y < centerPt.y && m_dragPt.y > centerPt.y)	//从下到上
			{
				if (point.x < centerPt.x)								//即顺时针 
				{
					offAngle = (curAngle - 180) - (startAngle + 180);
				}
				else if (point.x > centerPt.x)							//逆时针
				{
					offAngle = curAngle - startAngle;					//越来越大 负到正
				}
			}
			else if (point.y > centerPt.y && m_dragPt.y < centerPt.y)	//从上到下
			{
				if (point.x < centerPt.x)								//逆时针
				{
					offAngle = (startAngle + 180) + (180 - curAngle);
				}
				else if (point.x > centerPt.x)							//即顺时针 
				{
					offAngle = curAngle - startAngle;					//越来越小 正到负
				}
			}

			m_offSetAngle -= offAngle;

			//如果逆时针偏移（包括整个圆环）的角度大于所有的元素个数  m_angleMax决定圆环尾端显示
			if (m_angleMax - m_offSetAngle > (m_moudleShowArrayTemp.size() - 1)* m_stepAngle)
				m_offSetAngle += offAngle;

			//如果顺时针偏移（不包括整个圆环）的角度小于一个的初始数值  m_angleMax决定圆环首端显示
			if (m_offSetAngle >= m_startAngle)
				m_offSetAngle = m_startAngle;

			RepositionSubMoudle();
			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			m_dragPt = point;
		}
	}

	////【zy 2015-01-06 ↓】		工具提示是否显示
	if(m_pSubMoudleInfoHover)
		m_pngToolTipCtrl.Activate(TRUE);
 	else
 		m_pngToolTipCtrl.Activate(FALSE);
	////【zy 2015-01-06 ↑】

	CDialog::OnMouseMove(nFlags, point);
}

void CCircleDlg::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseMove = FALSE;
	m_pSubMoudleInfoHover = NULL;
	RedrawWindow(NULL, &m_wndRoleRgn, RDW_INVALIDATE | RDW_UPDATENOW);
}

LRESULT CCircleDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CPoint ptTemp = point;
	ScreenToClient(&ptTemp);
	if (!m_wndRoleRgn.PtInRegion(ptTemp))
		return HTTRANSPARENT;	//除了控件就是透明

	return CDialog::OnNcHitTest(point);
}

void CCircleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_wndLogoRgn.GetSafeHandle() == NULL)
	{	//第一次修改尺寸时对各区域变量进行初始化
		DWORD circleWidth	= m_pImgCircleTemp->GetWidth();
		DWORD circleHeight	= m_pImgCircleTemp->GetHeight();
		DWORD logoWidth		= m_pImgLogoTemp->GetWidth();
		DWORD logoHeight	= m_pImgLogoTemp->GetHeight();

		int subRange = 1;
		m_wndLogoRgn.CreateEllipticRgn((circleWidth - logoWidth) / 2 - subRange, (circleHeight - logoHeight) / 2 - subRange, (circleWidth + logoWidth) / 2 + subRange, (circleHeight + logoHeight) / 2 + subRange);

		CRgn *pRgnTemp = CRgn::FromHandle(AlphaImageToRgn(m_pImgCircleTemp, 254));
		m_wndCircleRgn.CreateRectRgn(0, 0, 0, 0);
		m_wndCircleRgn.CopyRgn(pRgnTemp);

		m_wndRoleRgn.CreateRectRgn(0, 0, 0, 0);
		m_wndLogoRgn.CombineRgn(&m_wndCircleRgn, &m_wndLogoRgn, RGN_AND);	//先取相交的部分
		m_wndRoleRgn.CombineRgn(&m_wndCircleRgn, &m_wndLogoRgn, RGN_XOR);	//再取异或部分

		m_wndUnRoleRgn.CreateRectRgn(0, 0, cx, cy);
		m_wndUnRoleRgn.CombineRgn(&m_wndUnRoleRgn, &m_wndRoleRgn, RGN_XOR);	//再取异或部分
		pRgnTemp->DeleteObject();
	}
}

void CCircleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if(m_pngToolTipCtrl.GetSafeHwnd())
		m_pngToolTipCtrl.DestroyWindow();
}

BOOL CCircleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message	== WM_LBUTTONDOWN	||
		pMsg->message	== WM_LBUTTONUP		||
		pMsg->message	== WM_MOUSEMOVE
		)
	{
		m_pngToolTipCtrl.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CCircleDlg::OnDisplayChange(WPARAM BitsPerPixel, LPARAM size)
{
	AfxGetMainWnd()->PostMessage(WM_REPOSITIONSUBDLG);
	//m_bDisplayChange = TRUE;
	return TRUE;
}

void CCircleDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	if (m_bDisplayChange && AfxGetMainWnd())
	{
		AfxGetMainWnd()->PostMessage(WM_REPOSITIONSUBDLG);
		m_bDisplayChange = FALSE;
	}
}

void CCircleDlg::DoLogon(BOOL bAreadyLogon)
{
	m_bAlreadyLogon = bAreadyLogon;

	if(m_bAlreadyLogon)
	{
		////【zy 2014-12-31 ↓】默认打开子系统

		ReUpdateFromConfig();   ////初始化时已经更新了圆环显示，在系统配置的时候应用也会实时更新圆环的状态，此处可以不用更新了
		for (UINT i = 0; i < m_moudleShowArrayTemp.size(); ++i)
		{
			if(m_moudleShowArrayTemp[i]->bDefaultOpen)
			{
				OpenSubSys(m_moudleShowArrayTemp[i]);
			}
		}
	}
	else
	{
		///////模块信息退出时不删除，固这里不做删除操作
		m_moudleShowArrayTemp.clear();
		Invalidate(FALSE);
	}
}

void CCircleDlg::SetLayeredAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	m_wndAlphaValue = bAlpha;
	SetLayeredWindowAttributes(crKey, bAlpha, dwFlags);
}
