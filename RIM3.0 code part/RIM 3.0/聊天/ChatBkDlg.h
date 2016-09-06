#pragma once

#include "../resource.h"
#include "afxcmn.h"
// CChatBkDlg 对话框

#define  FILETREEWIDTH			200
class CChatBkDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatBkDlg)

public:
	CChatBkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatBkDlg();

// 对话框数据
	enum { IDD = IDD_CHATBKDLG };
	enum { BUFLEN = 0x1000000 };

	CString				m_oldDic;
	static	int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		switch (uMsg)
		{
			case BFFM_INITIALIZED: //初始化消息
				::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPTSTR)(LPCTSTR)lpData); // m_filePath 为类的静态变量
				break;
			case BFFM_SELCHANGED: //选择路径变化，
			{
				TCHAR curr[MAX_PATH];
				SHGetPathFromIDList((LPCITEMIDLIST)lParam, curr);
				::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)curr);
			}
				break;
			default:
				break;
		}
		return 0;
	}


	static DWORD CALLBACK 						RichEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK 						RichEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL 								OnInitDialog();
	virtual void 								OnOK();
	virtual void 								OnCancel();
	afx_msg HBRUSH								OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 								OnEraseBkgnd(CDC* pDC);
	afx_msg void 								OnPaint();
	afx_msg void 								OnSize(UINT nType, int cx, int cy);
	afx_msg void 								OnMove(int x, int y);
	afx_msg void 								OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void 								OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void 								OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT								OnNcHitTest(CPoint point);
	afx_msg void								OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void 								OnMouseLeave();
	afx_msg void 								OnClose();
	afx_msg void 								OnDestroy();
	afx_msg void 								OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void								OnTimer(UINT_PTR nIDEvent);
	afx_msg void 								OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void 								OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL 								OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void 								OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL 								PreTranslateMessage(MSG* pMsg);
	afx_msg void 								OnDropFiles(HDROP hDropInfo);
	afx_msg void 								OnNcPaint();
	afx_msg BOOL 								OnNcActivate(BOOL bActive);
	virtual LRESULT								WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT								OnPreSendMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void 								OnTvnSelchangedSendfilltree(NMHDR *pNMHDR, LRESULT *pResult);

	/////自定义方法
	void 										RepositionSubDlg();
	void 										RepositionSelfDlg(BOOL bFile);
	void										SendFileOrDic(CString fileOrDic);
	BOOL										CanClose();
	void										DoLogon(BOOL bAreadyLogon);
	void										DrawBackground(CDC &dc);

	CChatDoc *m_pDoc1;
	CChatDoc *m_pDoc2;
	LPUSERINFO									m_pCurrentUser;				////当前聊天对话框的用户信息
	tstring										m_curUserIP;				////当前用户的IP;

	CToolTipCtrl								m_pngToolTipCtrl;			////工具提示控件			
	LPBYTE										m_pBuf;						////用于存储用户发送消息的临时缓冲区
	CFont										m_textFont;					////消息文字字体

	BOOL										m_bInitDlg;					////是否初始化对话框
	BOOL										m_bMouseMove;
	CRect										*m_pOldHoverSelRc;			////当前鼠标停留所在的按钮的Pt位置
	BOOL										m_bAlreadFileMode;			////是否是发送文件状态
	
	Image*										m_ImageBkTemp;				////背景图片			
	Image*										m_ImageUserSelTemp;			////用户选择图像
	Image*										m_ImageMinNormalTemp;		////最小化正常图像
	Image*										m_ImageMinPushTemp;			////最小化鼠标经过或按下时的图像
	Image*										m_ImageRestoreNormalTemp;	////还原按钮正常图像
	Image*										m_ImageRestorePushTemp;		////还原按钮鼠标经过或按下时的图像
	Image*										m_ImageMaxNormalTemp;		////最大化正常图像
	Image*										m_ImageMaxPushTemp;			////最大化鼠标经过或按下时的图像
	Image*										m_ImageCloseNormalTemp;		////关闭按钮正常图像
	Image*										m_ImageClosePushTemp;		////关闭按钮鼠标经过或按下时的图像
	Image*										m_ImageSendNormalTemp;		////发送按钮正常图像
	Image*										m_ImageSendPushTemp;		////发送按钮鼠标经过或按下时的图像
	Image*										m_ImageClose1NormalTemp;	////关闭按钮正常图像
	Image*										m_ImageClose1PushTemp;		////关闭按钮鼠标经过或按下时的图像

	Image*										m_RichImageBkTemp;			////RichEdit背景图片	目前没有使用但保留备用
	Image*										m_FileNormalTemp;			////文件图标的正常图像
	Image*										m_FilePushTemp;				////文件图标按下时的图像
	Image*										m_DicNormalTemp;			////文件夹图标的正常图像
	Image*										m_DicPushTemp;				////文件夹图标按下时的图像
	Image*										m_PictureNormalTemp;		////发送图片按钮正常图像
	Image*										m_PicturePushTemp;			////发送图片按钮按下时的图像
														
	CRect										m_selBtnRect;				////用户选择按钮区域
	CRect										m_minBtnRect;				////最小化按钮区域
	CRect										m_maxBtnRect;				////最大化按钮区域
	CRect										m_closeBtnRect;				////关闭按钮区域
	CRect										m_sendBtnRect;				////发送按钮区域
	CRect										m_close1BtnRect;			////关闭按钮按钮区域

	CRect										m_msgRc;					///下部消息输入框的区域
	CRect										m_toolRc;					///工具条区域·
	CRect										m_recordRc;					///上部消息记录框的区域

	CRect										m_sendFileRc;				////发送文件按钮区域
	CRect										m_sendDicRc;				////发送文件夹按钮区域
	CRect										m_sendPictureRc;			////发送图片按钮区域


	CHARFORMAT									m_defaultCharFormat;		////默认的文字格式
	CHARFORMAT									m_sendUserCharFormat;		////发送方用户名的文本格式
	CHARFORMAT									m_recvUserCharFormat;		////接收方用户名的文本格式


	CChatRichView								*m_pRecordView;				////消息记录显示控件
	CChatRichView								*m_pMsgEdit;				////发送消息框控件
	CSendFileListTree							m_filleTree;				////用于显示用户发送或接收文件列表的树控件

	static const long							m_toolHeight	= 50;		///工具条的高度，采用固定高度
	static const long							m_msgMinHeight	= 100;		///消息输入框的最小高度	
	long										m_msgHeight;				///消息输入框的实时高度	

public:
	void										InitUserInfo(LPUSERINFO pCurrentUser);
};
