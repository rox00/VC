// E:\My Project\visual studio 2008\RIM 3.0\聊天\ChatRichView.cpp : implementation file
//

#include "stdafx.h"
#include "ChatRichView.h"


// CChatRichView

IMPLEMENT_DYNCREATE(CChatRichView, CRichEditView)

CChatRichView::CChatRichView()
{
	//m_strClass = TEXT("RichEdit50W");//添加这一句

}

CChatRichView::~CChatRichView()
{
}

BEGIN_MESSAGE_MAP(CChatRichView, CRichEditView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(EN_LINK, &CChatRichView::OnEnLink)
	ON_NOTIFY_REFLECT(EN_PROTECTED, &CChatRichView::OnEnProtected)
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CChatRichView diagnostics

#ifdef _DEBUG
void CChatRichView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CChatRichView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CChatRichView message handlers

BOOL CChatRichView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(	pMsg->message == WM_KEYDOWN						&& 
		pMsg->wParam == VK_RETURN						&&
		!(GetAsyncKeyState(VK_CONTROL) && 0x8000)		&&
		!(GetAsyncKeyState(VK_SHIFT) && 0x8000)			&&
		!(GetAsyncKeyState(VK_MENU) && 0x8000)		
		)
	{
		AfxGetMainWnd()->PostMessage(WM_PRESEND, 0, 0);
		return TRUE;
	}

	return CRichEditView::PreTranslateMessage(pMsg);
}

void CChatRichView::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	AfxGetMainWnd()->SendMessage(WM_DROPFILES, WPARAM(hDropInfo), 0);
	//CRichEditView::OnDropFiles(hDropInfo);
}

HRESULT CChatRichView::QueryAcceptData(LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD dwReco, BOOL bReally, HGLOBAL hMetaFile)
{
	// TODO: Add your specialized code here and/or call the base class


    if (bReally)  
    {  
		FORMATETC tFormat	= { 0xC006/*FileName*/, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };  
		FORMATETC tFormatW	= { 0xC007/*FileNameW*/, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };  
        STGMEDIUM tSTGMEDIUM;  
        ZeroMemory(&tSTGMEDIUM,  sizeof(STGMEDIUM));  
		if (lpdataobj && (S_OK == lpdataobj->GetData(&tFormat, &tSTGMEDIUM) || S_OK == lpdataobj->GetData(&tFormatW, &tSTGMEDIUM)))
        {  
            return S_FALSE;  
        }  
    }  

	return CRichEditView::QueryAcceptData(lpdataobj, lpcfFormat, dwReco, bReally, hMetaFile);
}

int CChatRichView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////

	unsigned mask = ::SendMessage(m_hWnd, EM_GETEVENTMASK, 0, 0);
	::SendMessage(m_hWnd, EM_SETEVENTMASK, 0, mask | ENM_LINK  | ENM_MOUSEEVENTS | ENM_SCROLLEVENTS | ENM_KEYEVENTS);
	::SendMessage(m_hWnd, EM_AUTOURLDETECT, true, 0);

	m_HScroll.m_pImgScrollBk					= ImageConfig::GetInstance()->m_pImgScrollBk;
	m_HScroll.m_pImgHScrollThumb				= ImageConfig::GetInstance()->m_pImgHScrollThumb;
	m_VScroll.m_pImgScrollBk					= ImageConfig::GetInstance()->m_pImgScrollBk;
	m_VScroll.m_pImgVScrollThumb				= ImageConfig::GetInstance()->m_pImgVScrollThumb;


	if (AfxGetThreadState()->m_pWndInit == NULL)
	{
		m_HScroll.InitScrollBar(this, SB_HORZ);
		m_VScroll.InitScrollBar(this, SB_VERT);
	}

	{	//设置字体行间距

		//PARAFORMAT2 pf2;
		//memset(&pf2, 0, sizeof(PARAFORMAT2));
		////int lineSpacing = 30;   //行距
		//pf2.cbSize = sizeof(PARAFORMAT2);
		//pf2.dwMask = PFM_LINESPACING;
		//pf2.bLineSpacingRule = 5;   //2倍行距，取值0－5，具体含义看帮助
		////pf2.dyLineSpacing = lineSpacing;
		//VERIFY(SetParaFormat(pf2)); 

		//PARAFORMAT2 pf;
		//pf.cbSize 		= sizeof(PARAFORMAT2);
		//pf.dwMask 		= PFM_NUMBERING | PFM_OFFSET;
		//pf.wNumbering	= PFN_BULLET;//注意PFM_NUMBERING 
		//pf.dxOffset		= 10;
		//VERIFY(SetParaFormat(pf)); 
	}

	return 0;
}

void CChatRichView::OnEnLink(NMHDR *pNMHDR, LRESULT *pResult)
{
	ENLINK *pEnLink = reinterpret_cast<ENLINK *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString linkStr = _T("");
	if (pEnLink->msg == WM_LBUTTONUP)
	{
		GetRichEditCtrl().SetSel(pEnLink->chrg);//这是链接的文字范围
		linkStr = GetRichEditCtrl().GetSelText();//这是链接文字
		//后面是你的处理过程

		ShellExecute(NULL, _T("open"), linkStr, NULL, NULL, SW_SHOWNORMAL);
	}
}

void CChatRichView::OnEnProtected(NMHDR *pNMHDR, LRESULT *pResult)
{
	ENPROTECTED *pEnProtected = reinterpret_cast<ENPROTECTED *>(pNMHDR);
	// TODO:  The control will not send this notification unless you override the
	// CRichEditView::OnInitDialog() function to send the EM_SETEVENTMASK message
	// to the control with the ENM_PROTECTED flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	switch (pEnProtected->msg) 
	{
   case WM_KEYDOWN://按键，判断pEP->wParam
   case WM_PASTE://粘贴
	   GetRichEditCtrl().Paste();
   case WM_CUT://剪切
	   GetRichEditCtrl().Cut();
   case WM_COPY://剪切
	   GetRichEditCtrl().Copy();
   case EM_SETCHARFORMAT:
   default:
	   break;
	}
	*pResult = 0;
}

BOOL CChatRichView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CRichEditView::OnEraseBkgnd(pDC);
}

void CChatRichView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if (nPos >= 0)
	{
		switch (nSBCode)
		{
			case SB_THUMBPOSITION:
				break;
			case SB_THUMBTRACK:
				SetScrollPos(SB_HORZ, nPos);
				break;
			case SB_ENDSCROLL:
				break;
			default:
				break;
		}
	}

	CRichEditView::OnHScroll(nSBCode, nPos, pScrollBar);
	//Invalidate();
}

void CChatRichView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (nPos >= 0)
	{
		switch (nSBCode)
		{
			case SB_THUMBPOSITION:
				break;
			case SB_THUMBTRACK:
				SetScrollPos(SB_VERT, nPos);
				break;
			case SB_ENDSCROLL:
				break;
			default:
				break;
		}
	}

	CRichEditView::OnVScroll(nSBCode, nPos, pScrollBar);
	//Invalidate();
}

void CChatRichView::OnPaint()
{
	Default();

	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();

	return;
}

HBRUSH CChatRichView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CRichEditView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired

	return hbr;
}

HBRUSH CChatRichView::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}


void CChatRichView::OnSize(UINT nType, int cx, int cy)
{
	CRichEditView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	m_VScroll.UpdateScrollBar();
	m_HScroll.UpdateScrollBar();
	Invalidate();
}


void CChatRichView::OnMove(int x, int y)
{
	CRichEditView::OnMove(x, y);

	// TODO:  在此处添加消息处理程序代码
	m_VScroll.UpdateScrollBar();
	m_HScroll.UpdateScrollBar();
	Invalidate();
}

void CChatRichView::OnDestroy()
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
		pActiveItem->Deactivate();
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}
	CRichEditView::OnDestroy();
}
