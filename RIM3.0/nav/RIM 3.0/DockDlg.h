//********************************************************************
//	DOCKDLG.H �ļ�ע��
//	�ļ���		: DOCKDLG.H
//	�ļ�·��	: E:\MY PROJECT\VISUAL STUDIO 2008\RIM 3.0/
//	����		: ����
//	����ʱ��	: 2014/12/5 9:05
//	�ļ�����	: ������ʾͣ���Ĵ���
//*********************************************************************	
#pragma once


// CDockDlg dialog

class CDockDlg : public CDialog
{
	DECLARE_DYNAMIC(CDockDlg)

public:
	CDockDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDockDlg();

// Dialog Data
	enum { IDD = IDD_DOCKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	Image*									m_pImgLogoTemp;				//���ڻ���Բ�����������LogoͼƬ
	bool									m_bMouseMove;			//�����ж�����Ƿ��Ƴ�������
	BOOL									m_bDisplayChange;


	afx_msg void							OnMove(int x, int y);
	LRESULT									OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);
	virtual BOOL							OnInitDialog();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL 							OnEraseBkgnd(CDC* pDC);
	afx_msg void 							OnPaint();
	afx_msg void							OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void							OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void							OnMouseLeave();
	afx_msg void 							OnBnClickedOk();
	afx_msg void 							OnBnClickedCancel();
public:

	BYTE									m_wndAlphaValue;		//��ǰ���ڵ�͸����
	//************************************
	// Method:    UpdateView
	// FullName:  CDockDlg::UpdateView
	// Access:    public 
	// Returns:   void
	// Qualifier:							����Layerd��������ʾ
	// Parameter: Image * pImageLogo		LogoͼƬ����
	// Parameter: byte Alpha				͸����ֵ Ĭ��255����͸��
	//************************************
	void									UpdateView(Image* pImageLogo, byte Alpha = 255);
};
