#pragma once

#include "../resource.h"
#include "afxcmn.h"
// CChatBkDlg �Ի���

#define  FILETREEWIDTH			200
class CChatBkDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatBkDlg)

public:
	CChatBkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatBkDlg();

// �Ի�������
	enum { IDD = IDD_CHATBKDLG };
	enum { BUFLEN = 0x1000000 };

	CString				m_oldDic;
	static	int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		switch (uMsg)
		{
			case BFFM_INITIALIZED: //��ʼ����Ϣ
				::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPTSTR)(LPCTSTR)lpData); // m_filePath Ϊ��ľ�̬����
				break;
			case BFFM_SELCHANGED: //ѡ��·���仯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	/////�Զ��巽��
	void 										RepositionSubDlg();
	void 										RepositionSelfDlg(BOOL bFile);
	void										SendFileOrDic(CString fileOrDic);
	BOOL										CanClose();
	void										DoLogon(BOOL bAreadyLogon);
	void										DrawBackground(CDC &dc);

	CChatDoc *m_pDoc1;
	CChatDoc *m_pDoc2;
	LPUSERINFO									m_pCurrentUser;				////��ǰ����Ի�����û���Ϣ
	tstring										m_curUserIP;				////��ǰ�û���IP;

	CToolTipCtrl								m_pngToolTipCtrl;			////������ʾ�ؼ�			
	LPBYTE										m_pBuf;						////���ڴ洢�û�������Ϣ����ʱ������
	CFont										m_textFont;					////��Ϣ��������

	BOOL										m_bInitDlg;					////�Ƿ��ʼ���Ի���
	BOOL										m_bMouseMove;
	CRect										*m_pOldHoverSelRc;			////��ǰ���ͣ�����ڵİ�ť��Ptλ��
	BOOL										m_bAlreadFileMode;			////�Ƿ��Ƿ����ļ�״̬
	
	Image*										m_ImageBkTemp;				////����ͼƬ			
	Image*										m_ImageUserSelTemp;			////�û�ѡ��ͼ��
	Image*										m_ImageMinNormalTemp;		////��С������ͼ��
	Image*										m_ImageMinPushTemp;			////��С����꾭������ʱ��ͼ��
	Image*										m_ImageRestoreNormalTemp;	////��ԭ��ť����ͼ��
	Image*										m_ImageRestorePushTemp;		////��ԭ��ť��꾭������ʱ��ͼ��
	Image*										m_ImageMaxNormalTemp;		////�������ͼ��
	Image*										m_ImageMaxPushTemp;			////�����꾭������ʱ��ͼ��
	Image*										m_ImageCloseNormalTemp;		////�رհ�ť����ͼ��
	Image*										m_ImageClosePushTemp;		////�رհ�ť��꾭������ʱ��ͼ��
	Image*										m_ImageSendNormalTemp;		////���Ͱ�ť����ͼ��
	Image*										m_ImageSendPushTemp;		////���Ͱ�ť��꾭������ʱ��ͼ��
	Image*										m_ImageClose1NormalTemp;	////�رհ�ť����ͼ��
	Image*										m_ImageClose1PushTemp;		////�رհ�ť��꾭������ʱ��ͼ��

	Image*										m_RichImageBkTemp;			////RichEdit����ͼƬ	Ŀǰû��ʹ�õ���������
	Image*										m_FileNormalTemp;			////�ļ�ͼ�������ͼ��
	Image*										m_FilePushTemp;				////�ļ�ͼ�갴��ʱ��ͼ��
	Image*										m_DicNormalTemp;			////�ļ���ͼ�������ͼ��
	Image*										m_DicPushTemp;				////�ļ���ͼ�갴��ʱ��ͼ��
	Image*										m_PictureNormalTemp;		////����ͼƬ��ť����ͼ��
	Image*										m_PicturePushTemp;			////����ͼƬ��ť����ʱ��ͼ��
														
	CRect										m_selBtnRect;				////�û�ѡ��ť����
	CRect										m_minBtnRect;				////��С����ť����
	CRect										m_maxBtnRect;				////��󻯰�ť����
	CRect										m_closeBtnRect;				////�رհ�ť����
	CRect										m_sendBtnRect;				////���Ͱ�ť����
	CRect										m_close1BtnRect;			////�رհ�ť��ť����

	CRect										m_msgRc;					///�²���Ϣ����������
	CRect										m_toolRc;					///����������
	CRect										m_recordRc;					///�ϲ���Ϣ��¼�������

	CRect										m_sendFileRc;				////�����ļ���ť����
	CRect										m_sendDicRc;				////�����ļ��а�ť����
	CRect										m_sendPictureRc;			////����ͼƬ��ť����


	CHARFORMAT									m_defaultCharFormat;		////Ĭ�ϵ����ָ�ʽ
	CHARFORMAT									m_sendUserCharFormat;		////���ͷ��û������ı���ʽ
	CHARFORMAT									m_recvUserCharFormat;		////���շ��û������ı���ʽ


	CChatRichView								*m_pRecordView;				////��Ϣ��¼��ʾ�ؼ�
	CChatRichView								*m_pMsgEdit;				////������Ϣ��ؼ�
	CSendFileListTree							m_filleTree;				////������ʾ�û����ͻ�����ļ��б�����ؼ�

	static const long							m_toolHeight	= 50;		///�������ĸ߶ȣ����ù̶��߶�
	static const long							m_msgMinHeight	= 100;		///��Ϣ��������С�߶�	
	long										m_msgHeight;				///��Ϣ������ʵʱ�߶�	

public:
	void										InitUserInfo(LPUSERINFO pCurrentUser);
};
