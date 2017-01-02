// InfoTipDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RIM 3.0.h"
#include "InfoTipDlg.h"
#include "afxdialogex.h"


// CInfoTipDlg �Ի���

IMPLEMENT_DYNAMIC(CInfoTipDlg, CDialog)

CInfoTipDlg::CInfoTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoTipDlg::IDD, pParent)
{
	m_font.CreatePointFont(100, _T("΢���ź�"));
	m_pInfo = NULL;
}

CInfoTipDlg::~CInfoTipDlg()
{
	m_font.DeleteObject();
}

void CInfoTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoTipDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CInfoTipDlg ��Ϣ�������


void CInfoTipDlg::UpdateView(byte Alpha /*= 255*/)
{
	CRect textRect = CRect(0, 0, 0, 0);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC			*pDC = GetDC();

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	memDC.SelectObject(&memBmp);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SelectObject(&m_font);

	Graphics graphics(memDC.GetSafeHdc());

	if (m_pInfo)
	{
		RectF rcClientF = RectF(float(rcClient.left), float(rcClient.top), float(rcClient.Width()), float(rcClient.Height()));
		graphics.FillRectangle(&SolidBrush(Color(254, 255, 255, 255)), rcClientF);

		StringFormat stringformat;
		stringformat.SetAlignment(StringAlignmentNear);
		stringformat.SetLineAlignment(StringAlignmentNear);
		float tabStop = 40; stringformat.SetTabStops(0, 1, &tabStop);
		RectF boundRect(0, 0, 0, 0);
		///�������ָ߶�
		graphics.MeasureString(L"���", -1, &Gdiplus::Font(memDC.GetSafeHdc()), 
			RectF(float(rcClient.left), float(rcClient.top), float(rcClient.Width()), float(rcClient.Height())), &boundRect);
		float textHeight = boundRect.Height;

		switch (m_infoType)
		{
		case INFOTYPE::INFOTYPE_USER:
			{
				LPUSERINFO pUserInfo = (LPUSERINFO)m_pInfo;

				CStringW strNameW;		strNameW	.Format(L"  �û�����:\t%.12s", CT2W(pUserInfo->UserName.c_str()));
				CStringW strIDW;		strIDW		.Format(L"  �û�ID:\t%.12s", CT2W(pUserInfo->UserID.c_str()));
				CStringW strBranchW;	strBranchW	.Format(L"  ����:\t\t%.12s", CT2W(pUserInfo->branch.c_str()));
				CStringW strIPW;		strIPW		.Format(L"  �û�IP:\t");

				USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_READ);
				int i = 0;
				for(set<tstring>::iterator it = pUserInfo->ipList.begin(); it != pUserInfo->ipList.end(); ++it, ++i)
				{
					if(i == 0)
						strIPW += CT2W((*it).c_str());
					else if(i == 1)
					{
						strIPW += L" ,"; strIPW += CT2W((*it).c_str()); 
					}
					else if(i == 2)
					{
						strIPW += L" ,"; strIPW += L"...";
					}
				}
				USERINFO::IPListLock.unlock();

				CStringW strUserW;		strUserW	.Format(L"  �û�״̬:\t%.12s", CT2W(pUserInfo->state ? _T("����") : _T("����")));

				///�����߶� - �������������߶� - ���һ�е��ַ����߶�  �ٳ����пհ׼����
				float lineSpace = (rcClient.Height() - textHeight - 10) / 4;

				RectF rectF = rcClientF;

				rectF.Offset(0, 5);
				graphics.DrawString(strNameW, strNameW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strIDW, strIDW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strBranchW, strBranchW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.MeasureString(strIPW, -1, &Gdiplus::Font(memDC.GetSafeHdc()), PointF(0, 0), &boundRect);
				graphics.DrawString(strIPW, strIPW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strUserW, strUserW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

			}
			break;
		case INFOTYPE::INFOTYPE_STATION:
			{
				LPSTATIONINFO pStationInfo = (LPSTATIONINFO)m_pInfo;

				CStringW strNameW;		strNameW	.Format(L"  վ������:\t%s", CA2W(pStationInfo->pRimStationTemp->sName.c_str()));
				CStringW strStationNO;	strStationNO.Format(L"  վ����:\t%s", CA2W(pStationInfo->pRimStationTemp->sStationNO.c_str()));
				CStringW strRegionno;	strRegionno	.Format(L"  վ������:\t%s", CA2W(pStationInfo->pRegionNoTemp->sRegionName.c_str()));
				CStringW strAddress;	strAddress	.Format(L"  վ���ַ:\t%s", CA2W(pStationInfo->pRimStationTemp->sAddress.c_str()));
				CStringW strType;		strType		.Format(L"  վ������:\t%s", CT2W(pStationInfo->sStationType.c_str()));

				float lineSpace = (rcClient.Height() - textHeight - 10) / 4;

				RectF rectF = rcClientF;

				rectF.Offset(0, 5);
				graphics.DrawString(strNameW, strNameW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strStationNO, strStationNO.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strRegionno, strRegionno.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strAddress, strAddress.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strType, strType.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));
			}
			break;
		case INFOTYPE::INFOTYPE_DEVICE:
			{
				LPDEVICEINFO pDeviceInfo = (LPDEVICEINFO)m_pInfo;

				CStringW strNameW;			strNameW			.Format(L"  �豸����:\t%s", CA2W(pDeviceInfo->pRimDeviceTemp->sName.c_str()));
				CStringW strDeviceNO;		strDeviceNO			.Format(L"  �豸���:\t%s", CA2W(pDeviceInfo->pRimDeviceTemp->sDeviceNO.c_str()));
				CStringW strIP;				strIP				.Format(L"  ����IP:\t%s",	CA2W(pDeviceInfo->pRimDeviceTemp->sDriverIP.c_str()));
				CStringW strPort;			strPort				.Format(L"  ����˿�:\t%d", pDeviceInfo->pRimDeviceTemp->nDriverPort);
				CStringW strDeviceType;		strDeviceType		.Format(L"  �豸����:\t%s", CT2W(pDeviceInfo->sDeviceType.c_str()));
				CStringW strDeviceProtocol;	strDeviceProtocol	.Format(L"  �豸Э��:\t%s", CT2W(pDeviceInfo->sProtocol.c_str()));
				CStringW strDeviceStatus;	strDeviceStatus		.Format(L"  �豸״̬:\t");
				switch(pDeviceInfo->deviceState)
				{
				case 0:
					strDeviceStatus += _T("�豸����");
					break;
				case 1:
					strDeviceStatus += _T("�豸æ");
					break;
				case 2:
					strDeviceStatus += _T("�豸����");
					break;
				case 3:
					strDeviceStatus += _T("�豸δ֪");
					break;
				case 4:
					strDeviceStatus += _T("�豸����������");
					break;
				}

				float lineSpace = (rcClient.Height() - textHeight - 10) / 6;

				RectF rectF = rcClientF;

				rectF.Offset(0, 5);
				graphics.DrawString(strNameW, strNameW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strDeviceNO, strDeviceNO.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strIP, strIP.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strPort, strPort.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strDeviceType, strDeviceType.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strDeviceProtocol, strDeviceProtocol.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strDeviceStatus, strDeviceStatus.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));
			}
			break;
		case INFOTYPE::INFOTYPE_MEASURE:
			{
				/*LPMEASUREINFO pMeasureInfo = (LPMEASUREINFO)m_pInfo;

				CStringW strNameW;		strNameW	.Format(L"  ��������:\t%s", CT2W(pMeasureInfo->measueName.c_str()));
				CStringW strDevice;		strDevice	.Format(L"  �����豸:\t%s", CT2W(pMeasureInfo->pRtmDeviceFunTemp->sDeviceNO.c_str()));
				CStringW strStation;	strStation	.Format(L"  ����վ��:\t%s", CT2W(pMeasureInfo->pRtmDeviceFunTemp->sStationNO.c_str()));

				///����������׶�
				graphics.MeasureString(strStation, strStation.GetLength(), &Gdiplus::Font(memDC.GetSafeHdc()), 
					RectF(float(rcClient.left), float(rcClient.top), float(rcClient.Width()), float(rcClient.Height())), &boundRect);
				///�����߶� - �������������߶� - ���һ�е��ַ����߶�  �ٳ����пհ׼����
				float lineSpace = (rcClient.Height() - boundRect.GetBottom() - 10) / 2;

				RectF rectF = rcClientF;

				rectF.Offset(0, 5);
				graphics.DrawString(strNameW, strNameW.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strDevice, strDevice.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));

				rectF.Offset(0, lineSpace);
				graphics.DrawString(strStation, strStation.GetLength(),
					&Gdiplus::Font(memDC.GetSafeHdc()),
					rectF,
					&stringformat, &SolidBrush(Color(254, 0, 0, 0)));*/
			}
			break;
		default:
			break;
		}

		graphics.DrawRectangle(&Pen(Color(255, 0, 0, 0), 2), rcClientF);
	}

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	BLENDFUNCTION stBlend ={ AC_SRC_OVER, 0, Alpha, AC_SRC_ALPHA };
	::UpdateLayeredWindow(m_hWnd, pDC->GetSafeHdc(), &CPoint(rcWindow.left, rcWindow.top), &CSize(rcWindow.Width(), rcWindow.Height()),
						  memDC.GetSafeHdc(), &CPoint(0, 0), 0, &stBlend, ULW_ALPHA);


	graphics.ReleaseHDC(memDC.GetSafeHdc());
	memDC.DeleteDC();
	memBmp.DeleteObject();

	ReleaseDC(pDC);
}

void CInfoTipDlg::UpdateWindowShow(DWORD infoType, LPVOID pInfo, BOOL bShow)
{
	m_infoType		= infoType;
	m_pInfo			= pInfo;

	if (bShow)
		UpdateView(255);
	else
		UpdateView(0);
}

BOOL CInfoTipDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(255,0,0));
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}


void CInfoTipDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CInfoTipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	ModifyStyle(WS_CAPTION | WS_CHILD, WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX);
	ModifyStyleEx(0, WS_EX_LAYERED | WS_OVERLAPPED);

	UpdateView(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CInfoTipDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	
	// TODO:  �ڴ˴������Ϣ����������
	//if (GetStyle() & WS_VISIBLE)
	//{
		//CDialog::OnShowWindow(bShow, nStatus);
	//}
	//else
	//{
	//	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetWindowLong(GetSafeHwnd(), GWL_STYLE) | WS_VISIBLE);
	//	CDialog::OnShowWindow(FALSE, nStatus);
	//}
}


void CInfoTipDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

void CInfoTipDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnCancel();
}

void CInfoTipDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}
