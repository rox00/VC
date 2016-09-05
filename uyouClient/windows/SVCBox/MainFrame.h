#pragma once

#include "BoxView.h"
#include <GdiPlus.h>

using namespace Gdiplus;

class CMainFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CMainFrame)

private:
	class WindowToTouchScreenMapping
	{	//TODO: ���ﻹ��Ҫ���ǵ���Ļ��ת�Ŀ�����
	public:
		WindowToTouchScreenMapping() { m_fCoordinateXMappingRatio = m_fCoordinateYMappingRatio = 1; }

		void SetWindowSize(UINT x, UINT y, UINT realX, UINT realY, float scale = 1.0)
		{			
			boost::mutex::scoped_lock Lock(m_Mutex);
			m_fCoordinateXMappingRatio = (double)realX / x;
			m_fCoordinateYMappingRatio = (double)realY / y;
		}

		void DoMapping(int &x, int &y)
		{	//ע�⣬��������������������߳����汻���õ�
			boost::mutex::scoped_lock Lock(m_Mutex);
			x = (int)(x * m_fCoordinateXMappingRatio);
			y = (int)(y * m_fCoordinateYMappingRatio);
		}
	private:
		double								m_fCoordinateXMappingRatio;
		double								m_fCoordinateYMappingRatio;
		boost::mutex						m_Mutex;
	} m_WindowToTouchScreenMapping;

public:
	CMainFrame(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainFrame();

	BOOL									InitVideo(LPCTSTR lpszCableName, DWORD nCableId);
	void									Stop();
	static CSize							GetDeviceScreenSize() { return CSize(1920, 1008); }		//Android�豸��ʵ�ʷֱ��ʡ�Ϊ�˷����޸ģ�������һ�ݣ������ط�����Ӳ����˷ֱ���

protected:
	virtual void							DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL							OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT							OnFgNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void							OnPaint();
	afx_msg int								OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void							OnDestroy();
	afx_msg void							OnSize(UINT nType, int cx, int cy);	
	afx_msg void							OnDropFiles(HDROP hDropInfo);

private:

	CBrush									m_BrushBackground;
	CBoxView								m_Video;
	ULONG_PTR								m_gdiplusToken;
	GdiplusStartupInput						m_gdiplusStartupInput;	
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
