// E:\Share\VS\RIM 3.0\聊天\ChatBkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatBkDlg.h"
#include "afxdialogex.h"


// CChatBkDlg 对话框

IMPLEMENT_DYNAMIC(CChatBkDlg, CDialog)

CChatBkDlg::CChatBkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatBkDlg::IDD, pParent)
{
	m_textFont.CreatePointFont(100, _T("微软雅黑"));

	m_bInitDlg				= FALSE;
	m_bMouseMove			= FALSE;
	m_bAlreadFileMode		= TRUE;
	m_pCurrentUser			= NULL; 
	m_curUserIP				= _T("");
	m_pOldHoverSelRc		= NULL;

	m_oldDic				= _T("");
	m_pBuf					= new BYTE[BUFLEN];

	m_minBtnRect			= CRect(0,0,0,0);				////最小化按钮区域
	m_maxBtnRect			= CRect(0,0,0,0);				////最大化按钮区域
	m_closeBtnRect			= CRect(0,0,0,0);				////关闭按钮区域
	m_sendBtnRect			= CRect(0,0,0,0);				////发送按钮区域
	m_close1BtnRect			= CRect(0,0,0,0);				////关闭按钮按钮区域

	m_msgRc					= CRect(0,0,0,0);				///下部消息输入框的区域
	m_toolRc				= CRect(0,0,0,0);				///工具条区域·
	m_recordRc				= CRect(0,0,0,0);				///上部消息记录框的区域

	m_sendFileRc			= CRect(0,0,0,0);				////发送文件按钮区域
	m_sendDicRc				= CRect(0,0,0,0);				////发送文件夹按钮区域
	m_sendPictureRc			= CRect(0,0,0,0);				////发送图片按钮区域

	m_pRecordView			= NULL;							////消息记录显示控件
	m_pMsgEdit				= NULL;							////发送消息框控件

	m_msgHeight				= 120;
}

CChatBkDlg::~CChatBkDlg()
{
	m_textFont.DeleteObject();

	if (m_pBuf)
		delete[] m_pBuf;
}

void CChatBkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SENDFILLTREE, m_filleTree);
}


BEGIN_MESSAGE_MAP(CChatBkDlg, CDialog)
	ON_MESSAGE(WM_PRESEND, OnPreSendMsg)

	ON_WM_DROPFILES()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_CLOSE()
	ON_WM_NCCALCSIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETCURSOR()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_SENDFILLTREE, &CChatBkDlg::OnTvnSelchangedSendfilltree)
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
END_MESSAGE_MAP()


// CChatBkDlg 消息处理程序

DWORD CALLBACK CChatBkDlg::RichEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	//CFile* pFile = (CFile*)dwCookie;
	//pFile->Write(pbBuff, cb);
	//*pcb = cb;

	((CFile *)(LONGLONG)(dwCookie))->Write(pbBuff, cb);
	return 0;
}

DWORD CALLBACK CChatBkDlg::RichEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	//CFile* pFile = (CFile*)dwCookie;
	//*pcb = pFile->Read(pbBuff, cb);

	*pcb=((CFile *)(LONGLONG)(dwCookie))->Read(pbBuff, cb);
	return 0;
}

BOOL CChatBkDlg::CanClose()
{
	if (m_pCurrentUser && m_filleTree.GetSafeHwnd() && m_filleTree.GetCount())
	{
		if (IDNO == MessageBox(_T("如果关闭窗口，将会终止文件传送。是否关闭？"), _T("RIM3.0 提示"), MB_YESNO))
		{
			return FALSE;
		}
	}

	DoLogon(FALSE);
	return TRUE;
}
void CChatBkDlg::DrawBackground(CDC &dc)
{
	if(m_bInitDlg == FALSE)	return;

	CRect rcClient;
	GetClientRect(&rcClient);

	CPoint ptCur;
	GetCursorPos(&ptCur);
	ScreenToClient(&ptCur);

	CDC			memDC;
	CBitmap		memBmp;
	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	memDC.SelectObject(&memBmp);
	memDC.SelectObject(m_textFont);

	memDC.SetBkMode(TRANSPARENT);

	if(m_pCurrentUser->state == 0)
		memDC.SetTextColor(RGB(128, 128, 128));
	else
		memDC.SetTextColor(RGB(255, 255, 255));

	/////画背景

	Graphics graphics(memDC.GetSafeHdc());

	RectF rcClientF = RectF(float(rcClient.left), float(rcClient.top), float(rcClient.Width()), float(rcClient.Height()));
	LinearGradientBrush lineargradientbrush(rcClientF, Color(255, 49,110,140), Color(255,84,186,178), LinearGradientModeVertical);

	graphics.FillRectangle(&lineargradientbrush, rcClientF);
	//m_ImageBk.Draw(memDC.GetSafeHdc(), rcClient);
	//MemPaintPng(memDC, CRect(rcClient.Width() - m_ImageBk->GetWidth(), 0, rcClient.right, m_ImageBk->GetHeight()), m_ImageBk);

	//memDC.FillSolidRect(rcClient, RGB(75, 75, 75));
	//HBITMAP hBitmap = NULL;
	//((Bitmap*)m_ImageBkTemp)->GetHBITMAP(RGB(0, 0, 0), &hBitmap);
	//memDC.DrawState(CPoint(rcClient.Width() - m_ImageBkTemp->GetWidth(),0), CSize(m_ImageBkTemp->GetWidth(), m_ImageBkTemp->GetHeight()),
	//				hBitmap, DST_BITMAP);

	MemPaintPng(memDC, CRect(rcClient.Width() - m_ImageBkTemp->GetWidth(), 0, rcClient.right, m_ImageBkTemp->GetHeight()), m_ImageBkTemp);


	/////画标题
	if (m_pCurrentUser)
	{
		TCHAR ch[MAX_PATH] ={ 0 };
		_stprintf_s(ch, MAX_PATH, _T(": %d (ID: %d)"), m_pCurrentUser->userUdpPortTemp, m_pCurrentUser->userTypeID);
		tstring str = m_pCurrentUser->UserID + _T(" ") + m_curUserIP + ch;

		CString strWndText;
		GetWindowText(strWndText);
		if(strWndText.GetLength() == 0)
			SetWindowText(str.c_str());

		memDC.TextOut(m_selBtnRect.right, 5, str.c_str());
	}

	MemPaintPng(memDC, m_selBtnRect, m_ImageUserSelTemp);
	//m_ImageUserSel.Draw(memDC.GetSafeHdc(), m_selBtnRect);

	if (m_minBtnRect.PtInRect(ptCur))	//最小化
		MemPaintPng(memDC, m_minBtnRect, m_ImageMinPushTemp);
	else
		MemPaintPng(memDC, m_minBtnRect, m_ImageMinNormalTemp);

	if (m_maxBtnRect.PtInRect(ptCur))	//最大化
	{
		if (IsZoomed())
			MemPaintPng(memDC, m_maxBtnRect, m_ImageRestorePushTemp);
		else
			MemPaintPng(memDC, m_maxBtnRect, m_ImageMaxPushTemp);
	}
	else
	{
		if (IsZoomed())
			MemPaintPng(memDC, m_maxBtnRect, m_ImageRestoreNormalTemp);
		else
			MemPaintPng(memDC, m_maxBtnRect, m_ImageMaxNormalTemp);
	}

	if (m_closeBtnRect.PtInRect(ptCur))	//关闭按钮
		MemPaintPng(memDC, m_closeBtnRect, m_ImageClosePushTemp);
	else
		MemPaintPng(memDC, m_closeBtnRect, m_ImageCloseNormalTemp);

	if (m_sendBtnRect.PtInRect(ptCur))	//发送
		MemPaintPng(memDC, m_sendBtnRect, m_ImageSendPushTemp);
	else
		MemPaintPng(memDC, m_sendBtnRect, m_ImageSendNormalTemp);
	memDC.DrawText(_T("发送"), m_sendBtnRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	if (m_close1BtnRect.PtInRect(ptCur))	//关闭
		MemPaintPng(memDC, m_close1BtnRect, m_ImageClose1PushTemp);
	else
		MemPaintPng(memDC, m_close1BtnRect, m_ImageClose1NormalTemp);
	memDC.DrawText(_T("关闭"), m_close1BtnRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


	if (m_sendFileRc.PtInRect(ptCur))	///发送文件
		MemPaintPng(memDC, m_sendFileRc, m_FilePushTemp);
	else
		MemPaintPng(memDC, m_sendFileRc, m_FileNormalTemp);

	if (m_sendDicRc.PtInRect(ptCur))	//发送文件夹
		MemPaintPng(memDC, m_sendDicRc, m_DicPushTemp);
	else
		MemPaintPng(memDC, m_sendDicRc, m_DicNormalTemp);

	if (m_sendPictureRc.PtInRect(ptCur))	//发送图片
		MemPaintPng(memDC, m_sendPictureRc, m_PicturePushTemp);
	else
		MemPaintPng(memDC, m_sendPictureRc, m_PictureNormalTemp);

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	graphics.ReleaseHDC(memDC.GetSafeHdc());
	memDC.DeleteDC();
	memBmp.DeleteObject();
}

void CChatBkDlg::DoLogon(BOOL bAreadyLogon)
{

	if (bAreadyLogon)
	{
		SetTimer(1000, 200, NULL);
	}
	else
	{
		KillTimer(1000);


		{	//清除当前正在传送的文件及文件夹相关数据及树控件

			LPTRANSFILEINFO		pTransfileInfo		= NULL;
			while (m_filleTree.GetCount())
			{
				HTREEITEM		hItem				= m_filleTree.GetRootItem();
				if (pTransfileInfo = (LPTRANSFILEINFO)m_filleTree.GetItemData(hItem))
				{
					pTransfileInfo->SafeModifyTransFlag(tagTRANSFILEINFO::TRANS_FLAG_TRANSCANCEL);
					while (pTransfileInfo->GetTransFlag() != tagTRANSFILEINFO::TRANS_FLAG_TRANSEND) Sleep(1); //等待数组最后置空
					m_filleTree.DeleteItem(hItem);
				}
			}
		}
	}
}

void CChatBkDlg::SendFileOrDic(CString fileOrDic)
{
	LPTRANSFILEINFO pTransfileInfo = new TRANSFILEINFO();
	if (IsFileExist(fileOrDic))
	{
		tstring filePath = tstring(fileOrDic);
		TRANSFILEINFO::InitTransFileInfoByPath(pTransfileInfo, filePath, filePath);
	}
	else if (IsDicExist(fileOrDic))
	{
		tstring Dic = tstring(fileOrDic);
		TRANSFILEINFO::InitTransFileInfoByPath(pTransfileInfo, Dic, Dic);
	}
	if (pTransfileInfo->GetTotalSize() == 0)
		delete pTransfileInfo;
	else if (pTransfileInfo->GetTotalSize() >= 0)
	{
		//如果 用户在线，则构造缓冲区
		pTransfileInfo->SafeModifyTransFlag(TRANSFILEINFO::TRANS_FLAG_PRETRANS);
		m_pCurrentUser->sendFileQueue.Push(pTransfileInfo);
	}
}

void CChatBkDlg::RepositionSelfDlg(BOOL bFile)
{
	if (m_bAlreadFileMode == !bFile)
	{
		CRect rcWindow; GetWindowRect(rcWindow);

		if (bFile)
		{
			rcWindow.right += (FILETREEWIDTH + 10);
		}
		else
		{
			rcWindow.right -= (FILETREEWIDTH + 10);
		}

		m_bAlreadFileMode = bFile;
		MoveWindow(rcWindow, TRUE);
	}
}

void CChatBkDlg::RepositionSubDlg()
{
	if (GetDlgItem(IDC_PICTURE))
	{
		BOOL	bRedraw		= FALSE;
		DWORD	redrawFlag	= bRedraw ? 0 : SWP_NOREDRAW;
		int 	btncx 		= 20;
		int 	btncy 		= 20;

		CRect rcClient; GetClientRect(rcClient);
		{
			if (m_msgHeight <= m_msgMinHeight)
				m_msgHeight = m_msgMinHeight;
			else if (m_msgHeight >= rcClient.Height() - 2*m_msgMinHeight)
				m_msgHeight = rcClient.Height() - 2*m_msgMinHeight;
		}

		m_selBtnRect		= CRect(rcClient.left + 2, rcClient.top + 2 , rcClient.left + 2 + m_ImageUserSelTemp->GetWidth(), rcClient.top + 2 + m_ImageUserSelTemp->GetHeight());

		m_minBtnRect		= CRect(rcClient.right - 3 * btncx - 15, rcClient.top + 5, rcClient.right - 2 * btncx - 15, rcClient.top + btncy + 5);
		m_maxBtnRect		= CRect(rcClient.right - 2 * btncx - 10, rcClient.top + 5, rcClient.right - btncx - 10, rcClient.top + btncy + 5);
		m_closeBtnRect		= CRect(rcClient.right - btncx - 5, rcClient.top + 5, rcClient.right - 5, rcClient.top + btncy + 5);


		CRect dlgRect		= CRect(rcClient.left, rcClient.top + 45, rcClient.right, rcClient.bottom - 40);

		CRect chatRect		= dlgRect;

		if (m_bAlreadFileMode)
			chatRect		= CRect(dlgRect.left + 10, dlgRect.top, dlgRect.right - FILETREEWIDTH - 20, dlgRect.bottom);
		else
			chatRect		= CRect(dlgRect.left + 10, dlgRect.top, dlgRect.right - 10, dlgRect.bottom);


		m_msgRc				= CRect(chatRect.left, chatRect.bottom - m_msgHeight, chatRect.right, chatRect.bottom);
		m_toolRc			= CRect(chatRect.left, chatRect.bottom - m_msgHeight - m_toolHeight, chatRect.right, chatRect.bottom - m_msgHeight);
		m_recordRc			= CRect(chatRect.left, chatRect.top, chatRect.right, chatRect.bottom - m_msgHeight - m_toolHeight);

		{	/////计算工具区域的图像
			int btnHeight	= m_toolHeight - 20;
			m_sendFileRc	=	CRect(m_toolRc.left + 10, m_toolRc.top + 10, m_toolRc.left + 10 + btnHeight, m_toolRc.bottom - 10);
			m_sendDicRc		=	CRect(m_sendFileRc.right + 10, m_toolRc.top + 10, m_sendFileRc.right + 10 + btnHeight, m_toolRc.bottom - 10);
			m_sendPictureRc	=	CRect(m_sendDicRc.right + 10, m_toolRc.top + 10, m_sendDicRc.right + 10 + btnHeight, m_toolRc.bottom - 10);
		}

		CRect treeRect		= CRect(chatRect.right + 10, chatRect.top, chatRect.right + FILETREEWIDTH + 10, chatRect.bottom);

		m_sendBtnRect		= CRect(chatRect.right - 150, chatRect.bottom + 5, chatRect.right - 80, rcClient.bottom - 5);
		m_close1BtnRect		= CRect(chatRect.right - 75, chatRect.bottom + 5, chatRect.right - 5, rcClient.bottom - 5);

		HDWP hdwp = BeginDeferWindowPos(3);

		DeferWindowPos(hdwp, m_pRecordView->GetRichEditCtrl().GetSafeHwnd(), wndTop, m_recordRc.left, m_recordRc.top, m_recordRc.Width(), m_recordRc.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | redrawFlag);
		DeferWindowPos(hdwp, m_pMsgEdit->GetRichEditCtrl().GetSafeHwnd(), wndTop, m_msgRc.left, m_msgRc.top, m_msgRc.Width(), m_msgRc.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | redrawFlag);
		DeferWindowPos(hdwp, m_filleTree.GetSafeHwnd(), wndTop, treeRect.left, treeRect.top, treeRect.Width(), treeRect.Height(), SWP_NOZORDER | SWP_NOOWNERZORDER | redrawFlag);

		SCROLLBARINFO hscr;
		memset(&hscr, 0, sizeof(SCROLLBARINFO));
		hscr.cbSize = sizeof(SCROLLBARINFO);

		EndDeferWindowPos(hdwp);

		Invalidate(FALSE);
		m_filleTree.Invalidate(FALSE);
	}
}

void CChatBkDlg::InitUserInfo(LPUSERINFO pCurrentUser)
{
	m_pCurrentUser	= pCurrentUser;

	USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_READ);
	if(m_pCurrentUser->ipList.size())
		m_curUserIP		= *(m_pCurrentUser->ipList.begin());
	else
		m_curUserIP		= _T("127.0.0.1");
	USERINFO::IPListLock.unlock();
}

BOOL CChatBkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_pngToolTipCtrl.Create(this, TTS_ALWAYSTIP);
	m_pngToolTipCtrl.AddTool(this, _T(""));	//添加一个工具窗口，每增加一个就会生成 一个窗口

	m_ImageBkTemp					= ImageConfig::GetInstance()->m_ImageChatBk;
	m_ImageUserSelTemp				= ImageConfig::GetInstance()->m_ImageUserSel;
	m_ImageMinNormalTemp			= ImageConfig::GetInstance()->m_ImageMinNormal;
	m_ImageMinPushTemp				= ImageConfig::GetInstance()->m_ImageMinPush;
	m_ImageRestoreNormalTemp		= ImageConfig::GetInstance()->m_ImageRestoreNormal;
	m_ImageRestorePushTemp			= ImageConfig::GetInstance()->m_ImageRestorePush;
	m_ImageMaxNormalTemp			= ImageConfig::GetInstance()->m_ImageMaxNormal;
	m_ImageMaxPushTemp				= ImageConfig::GetInstance()->m_ImageMaxPush;
	m_ImageCloseNormalTemp			= ImageConfig::GetInstance()->m_ImageCloseNormal;
	m_ImageClosePushTemp			= ImageConfig::GetInstance()->m_ImageClosePush;
	m_ImageSendNormalTemp			= ImageConfig::GetInstance()->m_ImageSendNormal;
	m_ImageSendPushTemp				= ImageConfig::GetInstance()->m_ImageSendPush;
	m_ImageClose1NormalTemp			= ImageConfig::GetInstance()->m_ImageClose1Normal;
	m_ImageClose1PushTemp			= ImageConfig::GetInstance()->m_ImageClose1Push;

	m_RichImageBkTemp				= ImageConfig::GetInstance()->m_RichImageBk;
	m_FileNormalTemp				= ImageConfig::GetInstance()->m_FileNormal;
	m_FilePushTemp					= ImageConfig::GetInstance()->m_FilePush;
	m_DicNormalTemp					= ImageConfig::GetInstance()->m_DicNormal;
	m_DicPushTemp					= ImageConfig::GetInstance()->m_DicPush;
	m_PictureNormalTemp				= ImageConfig::GetInstance()->m_PictureNormal;
	m_PicturePushTemp				= ImageConfig::GetInstance()->m_PicturePush;


	{
		m_pDoc1 = (CChatDoc*)RUNTIME_CLASS(CChatDoc)->CreateObject();
		m_pDoc1->m_bAutoDelete = TRUE;

		CCreateContext cc;
		cc.m_pCurrentDoc	= m_pDoc1;

		m_pRecordView = (CChatRichView*)RUNTIME_CLASS(CChatRichView)->CreateObject();
		m_pRecordView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_READONLY, CRect(10, 10, 500, 300), this, 1001, &cc);
		m_pRecordView->DragAcceptFiles(FALSE);
	}

	{
		m_pDoc2 = (CChatDoc*)RUNTIME_CLASS(CChatDoc)->CreateObject();
		m_pDoc2->m_bAutoDelete = TRUE;

		CCreateContext cc;
		cc.m_pCurrentDoc	= m_pDoc2;

		m_pMsgEdit = (CChatRichView*)RUNTIME_CLASS(CChatRichView)->CreateObject();
		m_pMsgEdit->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL, CRect(10, 10, 500, 300), this, 1002, &cc);
		m_pMsgEdit->DragAcceptFiles(TRUE);
	}

	m_pRecordView->GetRichEditCtrl().SetBackgroundColor(FALSE, RGB(249, 245, 237));
	m_pMsgEdit->GetRichEditCtrl().SetBackgroundColor(FALSE, RGB(249, 245, 237));
	m_pMsgEdit->GetRichEditCtrl().DragAcceptFiles(TRUE);
	m_filleTree.SetItemHeight(40);



	if (m_pMsgEdit->GetRichEditCtrl().GetSafeHwnd())
	{	//设置默认字体
		CHARFORMAT charFormatTemp;

		m_pMsgEdit->GetRichEditCtrl().GetSelectionCharFormat(charFormatTemp);

		charFormatTemp.dwMask|=CFM_BOLD;
		charFormatTemp.dwEffects|=CFE_BOLD;		//设置粗体，取消用charFormatTemp.dwEffects&=~CFE_BOLD;
		charFormatTemp.dwMask|=CFM_ITALIC;
		//charFormatTemp.dwEffects|=CFE_ITALIC;	//设置斜体，取消用charFormatTemp.dwEffects&=~CFE_ITALIC;

		charFormatTemp.dwMask|=CFM_UNDERLINE;
		//charFormatTemp.dwEffects|=CFE_UNDERLINE;//设置斜体，取消用charFormatTemp.dwEffects&=~CFE_UNDERLINE;

		charFormatTemp.dwMask|=CFM_COLOR;
		charFormatTemp.crTextColor = RGB(0, 0, 0);//设置颜色

		charFormatTemp.dwMask|=CFM_SIZE;
		charFormatTemp.yHeight =200;//设置高度

		charFormatTemp.dwMask|=CFM_FACE;
		_tcscpy_s(charFormatTemp.szFaceName, LF_FACESIZE, _T("微软雅黑"));//设置字体

		m_pRecordView->GetRichEditCtrl().SetDefaultCharFormat(charFormatTemp);
		m_pMsgEdit->GetRichEditCtrl().SetDefaultCharFormat(charFormatTemp);

		m_defaultCharFormat			= charFormatTemp;
		charFormatTemp.crTextColor	= RGB(0, 128, 0);	charFormatTemp.dwEffects = 0; charFormatTemp.dwEffects&=~CFE_BOLD;
		m_sendUserCharFormat		= charFormatTemp;
		charFormatTemp.crTextColor	= RGB(0, 0, 143);	charFormatTemp.dwEffects = 0; charFormatTemp.dwEffects&=~CFE_BOLD;
		m_recvUserCharFormat		= charFormatTemp;

		//PARAFORMAT2 pf;
		//pf.cbSize = sizeof(PARAFORMAT2);
		//pf.dwMask = PFM_NUMBERING | PFM_OFFSET;
		//pf.wNumbering = PFN_BULLET; //注意PFM_NUMBERING 
		//pf.dxOffset = 10;
		//m_pChatFrame->m_pMsgView->GetRichEditCtrl().getmargi;
	}

	CenterWindow();
	RepositionSelfDlg(FALSE);
	RepositionSubDlg();

	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	//ModifyStyle(WS_CAPTION | WS_CHILD, WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX |WS_MAXIMIZEBOX|WS_THICKFRAME);
	//ModifyStyleEx(0, WS_EX_LAYERED | WS_OVERLAPPED);

	//SetWindowLong(GetSafeHwnd(), GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX/* | WS_THICKFRAME */| WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	//SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, /*WS_EX_LAYERED |*/ WS_EX_LEFT | WS_EX_OVERLAPPEDWINDOW | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR);

	//SetLayeredWindowAttributes(g_transAlphaClr, 255, LWA_COLORKEY | LWA_ALPHA);
	//UpdateView();
	//ShowWindow(SW_SHOW);

	DoLogon(TRUE);

	m_bInitDlg = TRUE;
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CChatBkDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//PostMessage(WM_PRESEND, 0, 0);
	CDialog::OnOK();
}

void CChatBkDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	if (CanClose())
		CDialog::OnCancel();
}

void CChatBkDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (CanClose())
		CDialog::OnClose();
}

HBRUSH CChatBkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CChatBkDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


void CChatBkDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	DrawBackground(dc);
	return;

}


void CChatBkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);


	// TODO:  在此处添加消息处理程序代码
	if (GetDlgItem(IDC_PICTURE))
	{
		RepositionSubDlg();
	}
}


void CChatBkDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO:  在此处添加消息处理程序代码
	if (GetDlgItem(IDC_PICTURE))
	{
		RepositionSubDlg();
	}
}


void CChatBkDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (point.y > m_toolRc.top && point.y < (m_toolRc.top + 5) || point.y >(m_toolRc.bottom - 5) && point.y < m_toolRc.bottom)
	{
		CPoint cursorPos;
		GetCursorPos(&cursorPos);

		CRect rcClient;
		GetClientRect(rcClient);

		SetCapture();
		MSG msg;
		while (::GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.hwnd == m_hWnd)
			{
				if (msg.message == WM_MOUSEMOVE)
				{
					CPoint ptCur = CPoint(0, 0);
					GetCursorPos(&ptCur);

					int offsetY = ptCur.y - cursorPos.y;
					m_msgHeight -= offsetY;

					if (m_msgHeight >= rcClient.Height() - 2*m_msgMinHeight)
					{
						/////将高度设为起始点击处
						ptCur.y = cursorPos.y - (rcClient.Height() - 2*m_msgMinHeight - (m_msgHeight + offsetY));
						m_msgHeight = rcClient.Height() - 2*m_msgMinHeight;
					}
					else if (m_msgHeight <= m_msgMinHeight)
					{
						/////将高度设为起始点击处
						ptCur.y = cursorPos.y - (m_msgMinHeight - (m_msgHeight + offsetY));
						m_msgHeight = m_msgMinHeight;
					}

					RepositionSubDlg();
					///////////优化拖动时的效果

					RedrawWindow();
					cursorPos = ptCur;
				}
				else if (msg.message == WM_LBUTTONUP)
				{
					break;
				}
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if (!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	//鼠标左键不按下时，及时跳出循环
				break;
		}

		ReleaseCapture();
		return;
	}

	CDialog::OnLButtonDown(nFlags, point);

	if(	m_selBtnRect.PtInRect(point)	||
		m_minBtnRect.PtInRect(point)	||
		m_maxBtnRect.PtInRect(point)	||
		m_closeBtnRect.PtInRect(point)	||
		m_close1BtnRect.PtInRect(point)	||
		m_sendBtnRect.PtInRect(point)	||
		m_sendFileRc.PtInRect(point)	||
		m_sendDicRc.PtInRect(point)		||
		m_sendPictureRc.PtInRect(point)	
		)
		return;
	else
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}


void CChatBkDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if(m_selBtnRect.PtInRect(point))
	{
		CMenu poupoMenu;
		poupoMenu.CreatePopupMenu();

		USERINFO::IPListLock.lock(CReadWriteLock::LOCK_LEVEL_READ);
		int index = 0;
		for (set<tstring>::iterator it = m_pCurrentUser->ipList.begin(); it != m_pCurrentUser->ipList.end(); ++it, ++index)
		{
			if(_tcscmp(m_curUserIP.c_str(), (*it).c_str())		== 0)
				poupoMenu.AppendMenu(MF_STRING | MF_CHECKED, WM_INDEXBASE + index,	(*it).c_str());
			else
				poupoMenu.AppendMenu(MF_STRING, WM_INDEXBASE + index,	(*it).c_str());
		}
		USERINFO::IPListLock.unlock();

		int menuSel = 0;
		CPoint pt = point;
		ClientToScreen(&pt);
		if(menuSel = poupoMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this))
		{
			CString strText = _T("");
			poupoMenu.GetMenuString(menuSel, strText, MF_BYCOMMAND);

			m_curUserIP = strText;
			Invalidate(FALSE);
		}

		poupoMenu.DestroyMenu();
	}
	else if (m_minBtnRect.PtInRect(point))
	{
		ShowWindow(SW_MINIMIZE);
		return;
	}
	else if (m_maxBtnRect.PtInRect(point))
	{
		if (IsZoomed())
			ShowWindow(SW_NORMAL);
		else
			ShowWindow(SW_MAXIMIZE);
		return;
	}
	else if (m_closeBtnRect.PtInRect(point) || m_close1BtnRect.PtInRect(point))
	{
		OnCancel();
		return;
	}
	else if (m_sendBtnRect.PtInRect(point))
	{
		PostMessage(WM_PRESEND);
		return;
	}
	else if (m_sendFileRc.PtInRect(point))
	{
		AfxGetMainWnd()->SendMessage(WM_PRESEND, 1, 0);
		return;
	}
	else if (m_sendDicRc.PtInRect(point))
	{
		AfxGetMainWnd()->SendMessage(WM_PRESEND, 2, 0);
		return;
	}
	else if (m_sendPictureRc.PtInRect(point))
	{
		AfxGetMainWnd()->SendMessage(WM_PRESEND, 3, 0);
		return;
	}


	CDialog::OnLButtonUp(nFlags, point);
}

void CChatBkDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CRect captionRect;
	GetClientRect(captionRect);
	captionRect.bottom = captionRect.top + 40;

	if (captionRect.PtInRect(point))
	{
		if (IsZoomed())
			ShowWindow(SW_NORMAL);
		else
			ShowWindow(SW_MAXIMIZE);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CChatBkDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

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

	if (!(m_pOldHoverSelRc && m_pOldHoverSelRc->PtInRect(point)))
	{//不在原来的区域了。或者压根没选中
		CRect *m_pHoverSelRcTemp = m_pOldHoverSelRc;

		if (m_minBtnRect.PtInRect(point) && m_pOldHoverSelRc != &m_minBtnRect)
		{
			InvalidateRect(m_minBtnRect, FALSE);
			m_pOldHoverSelRc = &m_minBtnRect;
			m_pngToolTipCtrl.UpdateTipText(_T("最小化"), this);
		}
		else if (m_maxBtnRect.PtInRect(point) && m_pOldHoverSelRc != &m_maxBtnRect)
		{
			InvalidateRect(m_maxBtnRect, FALSE);
			m_pOldHoverSelRc = &m_maxBtnRect;
			m_pngToolTipCtrl.UpdateTipText(IsZoomed() ? _T("还原") : _T("最大化"), this);
		}
		else if (m_closeBtnRect.PtInRect(point) && m_pOldHoverSelRc != &m_closeBtnRect)
		{
			InvalidateRect(m_closeBtnRect, FALSE);
			m_pOldHoverSelRc = &m_closeBtnRect;
			m_pngToolTipCtrl.UpdateTipText(_T("关闭"), this);
		}
		else if (m_sendBtnRect.PtInRect(point) && m_pOldHoverSelRc != &m_sendBtnRect)
		{
			InvalidateRect(m_sendBtnRect, FALSE);
			m_pOldHoverSelRc = &m_sendBtnRect;
			m_pngToolTipCtrl.UpdateTipText(_T("发送"), this);
		}
		else if (m_close1BtnRect.PtInRect(point) && m_pOldHoverSelRc != &m_close1BtnRect)
		{
			InvalidateRect(m_close1BtnRect, FALSE);
			m_pOldHoverSelRc = &m_close1BtnRect;
			m_pngToolTipCtrl.UpdateTipText(_T("关闭"), this);
		}
		else if (m_sendFileRc.PtInRect(point) && m_pOldHoverSelRc != &m_sendFileRc)
		{
			InvalidateRect(m_sendFileRc, FALSE);
			m_pOldHoverSelRc = &m_sendFileRc;
			m_pngToolTipCtrl.UpdateTipText(_T("发送文件"), this);
		}
		else if (m_sendDicRc.PtInRect(point) && m_pOldHoverSelRc != &m_sendDicRc)
		{
			InvalidateRect(m_sendDicRc, FALSE);
			m_pOldHoverSelRc = &m_sendDicRc;
			m_pngToolTipCtrl.UpdateTipText(_T("发送文件夹"), this);
		}
		else if (m_sendPictureRc.PtInRect(point) && m_pOldHoverSelRc != &m_sendPictureRc)
		{
			InvalidateRect(m_sendPictureRc, FALSE);
			m_pOldHoverSelRc = &m_sendPictureRc;
			m_pngToolTipCtrl.UpdateTipText(_T("发送图片"), this);
		}

		if (m_pHoverSelRcTemp != m_pOldHoverSelRc && m_pHoverSelRcTemp)	//从已选中的区域转移到了其它的区域
		{
			InvalidateRect(*m_pHoverSelRcTemp, FALSE);					//把之前的状态更新过来
		}
		else if (m_pHoverSelRcTemp == m_pOldHoverSelRc)					//之前判断前提是选择改炉台，此处又与之前的选区相比未改变因为必为NULL
		{
			m_pOldHoverSelRc = NULL;
			if (m_pHoverSelRcTemp)
				InvalidateRect(*m_pHoverSelRcTemp, FALSE);				//从选中项变为未选中
		}
	}

	////【zy 2015-01-06 ↓】		工具提示是否显示
	if (m_pOldHoverSelRc)
		m_pngToolTipCtrl.Activate(TRUE);
	else
		m_pngToolTipCtrl.Activate(FALSE);
	////【zy 2015-01-06 ↑】

	CDialog::OnMouseMove(nFlags, point);
}


LRESULT CChatBkDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CRect wndRect, NormalRect;
	GetWindowRect(&wndRect);

	NormalRect = wndRect; NormalRect.DeflateRect(5, 5, 5, 5);

	if (point.x > wndRect.left && point.x < NormalRect.left				&& point.y > wndRect.top		&& point.y < NormalRect.top)
		return HTTOPLEFT;
	else if (point.x > NormalRect.left	&& point.x < NormalRect.right	&& point.y > wndRect.top		&& point.y < NormalRect.top)
		return HTTOP;	
	else if (point.x > NormalRect.right && point.x < wndRect.right		&& point.y > wndRect.top		&& point.y < NormalRect.top)
		return HTTOPRIGHT;
	else if (point.x > NormalRect.right && point.x < wndRect.right		&& point.y > NormalRect.top 	&& point.y < NormalRect.bottom)
		return HTRIGHT;
	else if (point.x > NormalRect.right && point.x < wndRect.right		&& point.y > NormalRect.bottom	&& point.y <= wndRect.bottom)
		return HTBOTTOMRIGHT;
	else if (point.x > NormalRect.left	&& point.x < NormalRect.right	&& point.y > NormalRect.bottom	&& point.y <= wndRect.bottom)
		return HTBOTTOM;
	else if (point.x > wndRect.left		&& point.x < NormalRect.left	&& point.y > NormalRect.bottom	&& point.y <= wndRect.bottom)
		return HTBOTTOMLEFT;
	else if (point.x > wndRect.left		&& point.x < NormalRect.left	&& point.y > NormalRect.top		&& point.y < NormalRect.bottom)
		return HTLEFT;

	return HTCLIENT;	////为了避免不同的系统版本之下鼠标移动时还会出现NCMOUSEMOVE的消息及非客户区鼠标按下的消息
	return CDialog::OnNcHitTest(point);
}


void CChatBkDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (nHitTest == HTTOP)				{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));			return; }
	else if (nHitTest == HTBOTTOM)		{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));		return; }
	else if (nHitTest == HTLEFT)		{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));		return; }
	else if (nHitTest == HTRIGHT)		{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));		return; }
	else if (nHitTest == HTTOPLEFT)		{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));		return; }
	else if (nHitTest == HTTOPRIGHT)	{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));	return; }
	else if (nHitTest == HTBOTTOMLEFT)	{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));	return; }
	else if (nHitTest == HTBOTTOMRIGHT)	{ SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));	return; }
	
	CDialog::OnNcLButtonDown(nHitTest, point);
}


void CChatBkDlg::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bMouseMove = false;
	CDialog::OnMouseLeave();
	Invalidate(FALSE);
}


void CChatBkDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	lpncsp->rgrc[0].top		+= 0-GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYSIZEFRAME);;
	lpncsp->rgrc[0].left	+= 0-GetSystemMetrics(SM_CXFRAME);
	lpncsp->rgrc[0].right	-= 0-GetSystemMetrics(SM_CXSIZEFRAME);
	lpncsp->rgrc[0].bottom	-= 0-GetSystemMetrics(SM_CYSIZEFRAME);

	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
	//CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}

LRESULT CChatBkDlg::OnPreSendMsg(WPARAM wParam, LPARAM lParam)
{

	switch (wParam)
	{
		case 0:
			if (m_pMsgEdit->GetRichEditCtrl().GetSafeHwnd() && m_pMsgEdit->GetRichEditCtrl().GetTextLength())
			{
				clock_t clr0 = clock();


				//将字符串插入到整个内容的最前面

				m_pMsgEdit->GetRichEditCtrl().HideSelection(TRUE, FALSE);
				m_pMsgEdit->GetRichEditCtrl().SetSel(0, -1);
				m_pMsgEdit->GetRichEditCtrl().SetSelectionCharFormat(m_defaultCharFormat);
				

				CMemFile memFile(m_pBuf, BUFLEN);
				memFile.SeekToBegin();
				///////////////////////将整个 RichView中的内容写到内存文件中
				EDITSTREAM es;
				es.dwCookie = (DWORD_PTR)&memFile;
				es.pfnCallback = RichEditStreamOutCallback;
				m_pMsgEdit->GetRichEditCtrl().SetSel(0, -1);
				m_pMsgEdit->GetRichEditCtrl().StreamOut(SF_RTF | SFF_SELECTION, es);
				m_pMsgEdit->GetRichEditCtrl().Clear();
				m_pMsgEdit->GetRichEditCtrl().HideSelection(FALSE, FALSE);


				TRACE(_T("写入内存 用时：%d ms 时间：%d\n"), clock() - clr0, clock()); clr0 = clock();


				//IRichEditOle* pRichEditOleMsg = pChatFrame->m_pMsgView->GetRichEditCtrl().GetIRichEditOle();
				//int count = pRichEditOleMsg->GetObjectCount();     
				//for(int i = 0 ; i < count ; i++) //遍历位图
				//{
				//	REOBJECT object;  //位图信息存在这里
				//	memset(&object,0,sizeof(REOBJECT));
				//	object.cbStruct = sizeof(REOBJECT);
				//	pRichEditOleMsg->GetObject(i, &object, REO_GETOBJ_ALL_INTERFACES);

				//	int pos = object.cp ; //位图的位置信息
				//	DWORD dwUSer  =object.dwUser; //位图的信息，之前应用程序设置的，应有程序当然知道什么意思了
				//}

				//IRichEditOle* pRichEditOleRecord = pChatFrame->m_pRecordView->GetRichEditCtrl().GetIRichEditOle();

				if (m_pCurrentUser->state == USERINFO::USER_STATE_ONLINE)
				{
					CNetData *pChatNetData = new CNetData();
					pChatNetData->MakeNETBuffer(CNetData::TCP_CHATMSG, RIMConfig::GetInstance()->m_pLogonUser, m_pCurrentUser, m_curUserIP, m_pBuf, UINT(memFile.Seek(0, CFile::current)));
					if (pChatNetData->GetBufferLen())
						RIMConfig::GetInstance()->m_TCPSendQueue.Push(pChatNetData);
					else
						delete pChatNetData;


					//int memFileLen = UINT(memFile.Seek(0, CFile::current));
					//LPBYTE pBufferTemp = new BYTE[memFileLen + 5];

					//////////////////////////////对数据进行压缩
					//if (HZIP hz = CreateZip(pBufferTemp, memFileLen + 5, ""))
					//{
					//	tstring newGuidStr = GetNewGuid() + _T(".txt");
					//	if (ZR_OK == ZipAdd(hz, newGuidStr.c_str(), m_pBuf, UINT(memFile.Seek(0, CFile::current))))
					//	{
					//		LPBYTE	pZipBuffer	= NULL;
					//		DWORD	destlen		= 0;
					//		ZipGetMemory(hz, (void**)&pZipBuffer, &destlen);
					//		if (pZipBuffer && destlen)
					//		{
					//			CNetData *pChatNetData = new CNetData();
					//			pChatNetData->MakeNETBuffer(CNetData::TCP_CHATMSG, RIMConfig::GetInstance()->m_pLogonUser, m_pCurrentUser, pZipBuffer, destlen);
					//			RIMConfig::GetInstance()->m_TCPSendQueue.Push(pChatNetData);
					//		}
					//	}

					//	if (hz != NULL)	{ CloseZip(hz);	hz = NULL; }
					//}
					//if (pBufferTemp)
					//	delete[] pBufferTemp;
				}
				TRACE(_T("写入数据队列 用时：%d ms 时间：%d\n"), clock() - clr0, clock()); clr0 = clock();


				{//////////////// 将内存文件中的内容写到通话记录视图中

					///////////////构造字符串，包括发送的用户，及发送时间
					SYSTEMTIME systemtime;
					GetLocalTime(&systemtime);
					TCHAR ch[MAX_PATH] ={ 0 };
					_stprintf_s(ch, MAX_PATH, _T("%s (%d/%02d/%02d %02d:%02d:%02d):\n"), 
						RIMConfig::GetInstance()->m_pLogonUser->UserName.c_str(), 
						systemtime.wYear, 
						systemtime.wMonth, 
						systemtime.wDay, 
						systemtime.wHour, 
						systemtime.wMinute, 
						systemtime.wSecond);

					memFile.SeekToBegin();
					m_pRecordView->GetRichEditCtrl().SetReadOnly(FALSE);

					m_pRecordView->GetRichEditCtrl().SetSel(-2, -1);
					m_pRecordView->GetRichEditCtrl().SetWordCharFormat(m_sendUserCharFormat);
					m_pRecordView->GetRichEditCtrl().ReplaceSel(ch, FALSE);					//输入用户发送时间

					EDITSTREAM es;
					es.dwCookie = (DWORD_PTR)&memFile;
					es.pfnCallback = RichEditStreamInCallback;
					m_pRecordView->GetRichEditCtrl().SetSel(-2, -1);
					m_pRecordView->GetRichEditCtrl().StreamIn(SF_RTF | SFF_SELECTION, es);
					//为了解决添加SFF_SELECTION后，自动滚动不显示的问题

					if(!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	////鼠标没有按下
						m_pRecordView->GetRichEditCtrl().PostMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0);;

					m_pRecordView->GetRichEditCtrl().SetReadOnly(TRUE);
				}

				TRACE(_T("写入上面View 用时：%d ms 时间：%d\n"), clock() - clr0, clock()); clr0 = clock();

			}
			break;
		case 1:
		{
			CFileDialog filedlg(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_READONLY, _T("All Files (*.*)|*.*||"), this);

			if (IDOK == filedlg.DoModal())
			{
				CString filePath = filedlg.GetPathName();
				SendFileOrDic(filePath);
			}

			InvalidateRect(m_sendFileRc, FALSE);
		}
			break;
		case 2:
		{
			TCHAR szPath[MAX_PATH] ={ 0 }; //存放选择的目录路径

			BROWSEINFO bi;
			bi.hwndOwner		= m_hWnd;
			bi.pidlRoot			= NULL;
			bi.pszDisplayName	= szPath;
			bi.lpszTitle		= _T("请选择需要导出的目录：");
			bi.ulFlags			= BIF_RETURNONLYFSDIRS;
			bi.lpfn				= BrowseCallbackProc; //设置CALLBACK函数
			bi.iImage			= 0;
			bi.lParam			= long(m_oldDic.GetBuffer());

			//弹出选择目录对话框
			LPITEMIDLIST lp = SHBrowseForFolder(&bi);
			if (lp && SHGetPathFromIDList(lp, szPath))
			{
				m_oldDic = szPath;
				SendFileOrDic(szPath);
			}

			InvalidateRect(m_sendDicRc, FALSE);
		}
			break;
		case 3:
		{
			CString sFilter = _T("All image file|*.bmp;*.jpg;|Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|");
			//CString sFilter = _T("All image file|*.bmp;*.jpg;*.gif|Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|GIF Files (*.gif)|*.gif|");

			CFileDialog filedlg(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_READONLY, sFilter, this);

			if (IDOK == filedlg.DoModal())
			{
				CTapBitmap bmp;
				if (bmp.Load(filedlg.GetPathName()) == FALSE)
				{
					AfxMessageBox(_T("Could not load image."));
					return TRUE;
				}
				CEnBitmap enBitmap;
				CBitmap Bitmap;
				if (enBitmap.Attach(bmp.GetBMP(), 0))
				{
					Bitmap.DeleteObject();
					Bitmap.Attach(enBitmap.Detach());

					IRichEditOle	*pRichEditOle;
					pRichEditOle	= m_pMsgEdit->GetRichEditCtrl().GetIRichEditOle();
					HBITMAP hBitmap = (HBITMAP)Bitmap;
					if (hBitmap)
					{
						ImageDataObject::InsertBitmap(pRichEditOle, hBitmap);
					}
					pRichEditOle->Release();
				}
			}

			InvalidateRect(m_sendPictureRc, FALSE);
		}
			break;
	}


	return TRUE;
}

void CChatBkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (m_pCurrentUser)
	{
		while (m_pCurrentUser->receiveMsgQueue.GetQueueCount())
		{
			CNetData *pQueueData = NULL;
			m_pCurrentUser->receiveMsgQueue.Pop((CQueueData**)&pQueueData);
			if (pQueueData)
			{//////////////// 将内存文件中的内容写到通话记录视图中
				CMemFile memFile(pQueueData->GetBuffer(), pQueueData->GetBufferLen());
				memFile.SeekToBegin();


				{//////////////// 将内存文件中的内容写到通话记录视图中

					///////////////接收时间
					int iplistCnt = m_pCurrentUser->ipList.size();
					TCHAR ch[MAX_PATH] ={ 0 };
					_stprintf_s(ch, MAX_PATH, _T("%s (%d/%02d/%02d %02d:%02d:%02d):\n"), 
						iplistCnt > 1 ?pQueueData->GetFromOrToIP().c_str():pQueueData->GetSourceID().c_str(), 
								pQueueData->GetDataCreateTime().wYear, 
								pQueueData->GetDataCreateTime().wMonth,
								pQueueData->GetDataCreateTime().wDay,
								pQueueData->GetDataCreateTime().wHour,
								pQueueData->GetDataCreateTime().wMinute,
								pQueueData->GetDataCreateTime().wSecond);

					memFile.SeekToBegin();
					m_pRecordView->GetRichEditCtrl().SetReadOnly(FALSE);

					m_pRecordView->GetRichEditCtrl().SetSel(-2, -1);
					m_pRecordView->GetRichEditCtrl().SetWordCharFormat(m_recvUserCharFormat);
					m_pRecordView->GetRichEditCtrl().ReplaceSel(ch, FALSE);					//输入用户发送时间

					EDITSTREAM es;
					es.dwCookie = (DWORD_PTR)&memFile;
					es.pfnCallback = RichEditStreamInCallback;
					m_pRecordView->GetRichEditCtrl().SetSel(-2, -1);
					m_pRecordView->GetRichEditCtrl().StreamIn(SF_RTF | SFF_SELECTION, es);
					//为了解决添加SFF_SELECTION后，自动滚动不显示的问题

					if(!(GetAsyncKeyState(VK_LBUTTON) && 0x8000))	////鼠标没有按下
						m_pRecordView->GetRichEditCtrl().PostMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0);
					m_pRecordView->GetRichEditCtrl().SetReadOnly(TRUE);
				}

				memFile.Close();
				delete pQueueData;
			}
		}

		/////将接收队列插入树控件
		while (m_pCurrentUser->receiveFileQueue.GetQueueCount())
		{
			LPTRANSFILEINFO pTransfileInfo = NULL;
			m_pCurrentUser->receiveFileQueue.Pop((CQueueData**)&pTransfileInfo);

			HTREEITEM hItem = m_filleTree.InsertItem(SENDFILELISTTREE_RECVSTR, TVI_ROOT, TVI_FIRST);
			m_filleTree.SetItemData(hItem, (DWORD_PTR)pTransfileInfo);
			pTransfileInfo->SafeModifyTransFlag(TRANSFILEINFO::TRANS_FLAG_TRANSBEGIN);

			BringWindowToTop();
			SetForegroundWindow();
		}
		/////将发送队列插入树控件
		while (m_pCurrentUser->sendFileQueue.GetQueueCount())
		{
			LPTRANSFILEINFO pTransfileInfo = NULL;
			m_pCurrentUser->sendFileQueue.Pop((CQueueData**)&pTransfileInfo);

			HTREEITEM hItem = m_filleTree.InsertItem(SENDFILELISTTREE_SENDSTR, TVI_ROOT, TVI_LAST);
			m_filleTree.SetItemData(hItem, (DWORD_PTR)pTransfileInfo);

			///////构造文件网络传输数据块
			string xmlStr = TRANSFILEINFO::MakeXmlStr(pTransfileInfo);
			if (xmlStr.size())
			{
				CFileNetData *pFileNetData = new CFileNetData(pTransfileInfo);
				pFileNetData->MakeNETBuffer(CNetData::TCP_FILEASKSEND, RIMConfig::GetInstance()->m_pLogonUser, m_pCurrentUser, m_curUserIP, (LPBYTE)xmlStr.c_str(), xmlStr.size());
				RIMConfig::GetInstance()->m_TCPSendQueue.Push(pFileNetData);
			}

			pTransfileInfo->SafeModifyTransFlag(TRANSFILEINFO::TRANS_FLAG_TRANSBEGIN);

			BringWindowToTop();
			SetForegroundWindow();
		}

	}

	if (m_filleTree.GetSafeHwnd())
	{
		LPTRANSFILEINFO pTransfileInfo	= NULL;
		HTREEITEM	hItem				= m_filleTree.GetRootItem();
		while (hItem != NULL)
		{
			if (pTransfileInfo = (LPTRANSFILEINFO)m_filleTree.GetItemData(hItem))
			{
				switch (pTransfileInfo->GetTransFlag())
				{
					case TRANSFILEINFO::TRANS_FLAG_PRETRANS:
					case TRANSFILEINFO::TRANS_FLAG_TRANSBEGIN:
						////该操作由树控件处理是开始接收还是发送
					case TRANSFILEINFO::TRANS_FLAG_TRANSING:
						/////由下面统一刷新
						break;
					case TRANSFILEINFO::TRANS_FLAG_TRANSCANCEL:
					case TRANSFILEINFO::TRANS_FLAG_TRANSPREEND:
						//取消操作不处理等待发送或接收线程处理
						break;
					case TRANSFILEINFO::TRANS_FLAG_TRANSEND:
					{
						CString strTip = _T("");
						strTip.Format(_T("(%s)%s\n"), PathFindFileName(pTransfileInfo->GetApplicationName().c_str()), pTransfileInfo->transError.c_str());
						m_pRecordView->GetRichEditCtrl().SetReadOnly(FALSE);
						m_pRecordView->GetRichEditCtrl().SetSel(-2, -1);
						m_pRecordView->GetRichEditCtrl().ReplaceSel(strTip, FALSE);	//替换首部字符串
						m_pRecordView->GetRichEditCtrl().SetReadOnly(TRUE);

						m_filleTree.DeleteItem(hItem);	hItem = m_filleTree.GetRootItem();	////删除并重新开始遍历
						delete pTransfileInfo;
						TRACE(_T("删除结构时间：%d ms\n"), clock());

					}
						continue;	//receiveFileVec结构发生变化需要重新来
				}
			}
			hItem	= m_filleTree.GetNextItem(hItem, TVGN_NEXT);
		}

		if (m_filleTree.GetCount())
		{
			RepositionSelfDlg(TRUE);
			m_filleTree.Invalidate(FALSE);
		}
		else
			RepositionSelfDlg(FALSE);
	}


	CDialog::OnTimer(nIDEvent);
}

void CChatBkDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	if (m_pCurrentUser->state == USERINFO::USER_STATE_ONLINE)
	{	///用户必须在线
		TCHAR szFileName[MAX_PATH] ={ 0 };
		UINT nFileCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		ASSERT(nFileCount != 0);
		for (UINT i=0; i < nFileCount; i++)
		{
			::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
			SendFileOrDic(szFileName);
		}

		::DragFinish(hDropInfo);
	}
	else
	{
		m_pRecordView->GetRichEditCtrl().SetReadOnly(FALSE);
		m_pRecordView->GetRichEditCtrl().SetSel(-2, -1);
		m_pRecordView->GetRichEditCtrl().ReplaceSel(_T("	文件传输:用户当前不在线.\n"), FALSE);	//替换首部字符串
		m_pRecordView->GetRichEditCtrl().SetReadOnly(TRUE);
	}
}



void CChatBkDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (lpMMI->ptMinTrackSize.y < 550)
	{
		lpMMI->ptMinTrackSize.y = 550;
	}

	////文件发送模式和非文件发送模式 的窗口大小限制是不同的
	if(m_bAlreadFileMode)
	{
		if (lpMMI->ptMinTrackSize.x < (600 + (FILETREEWIDTH + 10)))
		{
			lpMMI->ptMinTrackSize.x = 600 + (FILETREEWIDTH + 10);
		}
	}
	else
	{
		if (lpMMI->ptMinTrackSize.x < 600)
		{
			lpMMI->ptMinTrackSize.x = 600;
		}
	}


	CDialog::OnGetMinMaxInfo(lpMMI);
}

BOOL CChatBkDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if (point.y > m_toolRc.top && point.y < (m_toolRc.top + 5) || point.y >(m_toolRc.bottom - 5) && point.y < m_toolRc.bottom)
	{
		HCURSOR   hCursor   =   AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
		SetCursor(hCursor);
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CChatBkDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO:  在此处添加消息处理程序代码
	if (m_pMsgEdit->GetRichEditCtrl().GetSafeHwnd())
	{
		m_pMsgEdit->GetRichEditCtrl().SetFocus();
	}
}


BOOL CChatBkDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_LBUTTONUP ||
		pMsg->message == WM_MOUSEMOVE
		)
	{
		m_pngToolTipCtrl.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CChatBkDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO:  在此处添加消息处理程序代码

	if (m_pngToolTipCtrl.GetSafeHwnd())
		m_pngToolTipCtrl.DestroyWindow();
}


void CChatBkDlg::OnTvnSelchangedSendfilltree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

void CChatBkDlg::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CDialog::OnNcPaint() for painting messages
	CWindowDC dc(this);
	dc.ExcludeClipRect(m_recordRc);
	dc.ExcludeClipRect(m_msgRc);

	CRect treeRect;
	m_filleTree.GetWindowRect(treeRect);
	ScreenToClient(treeRect);
	dc.ExcludeClipRect(treeRect);
	//dc.ExcludeClipRect(m_toolRc);
	//dc.ExcludeClipRect(m_minBtnRect);
	//dc.ExcludeClipRect(m_maxBtnRect);
	//dc.ExcludeClipRect(m_closeBtnRect);
	//dc.ExcludeClipRect(m_sendBtnRect);
	//dc.ExcludeClipRect(m_close1BtnRect);
 
	DrawBackground(dc);
}

BOOL CChatBkDlg::OnNcActivate(BOOL bActive)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

LRESULT CChatBkDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == 0x125)
	{
		OnNcPaint();
	}

	if (message ==	0x00AE	||	//WM_NCUAHDRAWCAPTION
		message	==	0x00AF)		//WM_NCUAHDRAWFRAME)
	{
		OnNcPaint();
		return 0;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
