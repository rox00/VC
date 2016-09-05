#pragma once

// SVCBoxMonitorCtrl.h : Declaration of the CSVCBoxMonitorCtrl ActiveX Control class.


// CSVCBoxMonitorCtrl : See SVCBoxMonitorCtrl.cpp for implementation.
#include "CVedioDlg.h"
#include "Input/InputManager.h"
#include "Input/KeyboardMapper.h"
#include <map>
using namespace std;

class CSVCBoxMonitorCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSVCBoxMonitorCtrl)
	DECLARE_INTERFACE_MAP()
	DECLARE_OLECREATE_EX(CSVCBoxMonitorCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSVCBoxMonitorCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSVCBoxMonitorCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSVCBoxMonitorCtrl)		// Type name and misc status

	// Message maps
	DECLARE_MESSAGE_MAP()
	// Dispatch maps
	DECLARE_DISPATCH_MAP()
	// Event maps
	DECLARE_EVENT_MAP()
// Constructor
public:
	CSVCBoxMonitorCtrl();
	~CSVCBoxMonitorCtrl();
// Implementation
	afx_msg void AboutBox();
// Dispatch and event IDs
public:
	enum 
	{									
		dispidUpdateCollsepRc		= 14L,
		dispidUpdateDeviceSize		= 13L,
		dispidSetFlag				= 12L,
		dispidUpdateVedioRect		= 11L,
		dispidClearTouchedKey		= 10L,
		dispidExit					= 9L,
		dispidCallAndroidHomeKey	= 8L,
		dispidCallAndroidBack		= 7L,
		dispidOpenTaskManager		= 6L,
		dispidSetRunningApp			= 5L,
		dispidSaveKeyMappingConfig	= 4L,
		dispidSetKeyMappingOpen		= 3L,
		dispidLoadKeyMapping		= 2L,
		dispidStart					= 1L,

		eventidNotifyConnectStateChanged = 1L
	};

protected:
	/*�ӿ�*/
	ULONG						Start(ULONG bAutoReconnect, ULONG hwndVedio, LPCTSTR strIP);
	ULONG						LoadKeyMapping(LPCTSTR packageName);
	ULONG						SetKeyMappingOpen(SHORT open);
	ULONG						SaveKeyMappingConfig(LPCTSTR configFile, LPCTSTR content);
	ULONG						SetRunningApp(LPCTSTR packageName);
	ULONG						OpenTaskManager();
	ULONG						CallAndroidBackKey();
	ULONG						CallAndroidHomeKey();
	ULONG						Exit();
	void						ClearTouchedKey();
	void						SetFlag(LONG nFlag, LONG value);
	void						UpdateVedioRect(LONG l, LONG t, LONG r, LONG b);
	void						UpdateDeviceSize(LONG deviceWidth, LONG deviceHeight);
	void						UpdateCollsepRc(LONG screenLeft, LONG screenTop, LONG screenRight, LONG screenBottom);
	/*�¼�*/
	void NotifyConnectStateChanged(ULONG ulCurrentState)
	{
		FireEvent(eventidNotifyConnectStateChanged, EVENT_PARAM(VTS_UI4), ulCurrentState);
	}

	/*��Ϣ*/
	afx_msg int					OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void				OnSize(UINT nType, int cx, int cy);
	afx_msg void				OnDestroy();
	afx_msg LRESULT				OnConnectStateChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT				OnNotifyReStart(WPARAM wParam, LPARAM lParam);
	afx_msg void				OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void				OnClose();
	afx_msg BOOL				OnEraseBkgnd(CDC* pDC);
	afx_msg void				OnPaint();
	afx_msg void				OnNcPaint();
	/*����*/
	void						BindMappingFunc();
	void						UnBindMappingFunc();
	void						PushFileToBox(LPCTSTR downladFile);
	String						UpdatePCFile(LPSTR file);
	void						ReStart();
private:
	BOOL						m_bAutoReconnect;			//��������
	boost::thread_group			m_WorkingThreadGroup;									////�߳���
	boost::thread				*m_pThreadConnectProc;
	list<COPYDATASTRUCT>		m_copyDataVec;				//����copyData�����ݶ���
	ConnectStatus				m_Status;
	BOOL						m_bExit;					//�ж��Ƿ��Ѿ��˳�
	HWND						m_hwndVedio;				//��ʾ��Ƶ�Ĵ���

	CRect						m_CollapseScreenRect;		//��ť����Ļ������
	CRect						m_CollapseVedioClientRect;	//��ť����������ڵĿͻ���������
	CRect						m_vedioScreenRect;
	double						m_fCoordinateXMappingRatio;	//����ں��ӵı���
	double						m_fCoordinateYMappingRatio;	//����ں��ӵı���

	CVedioDlg					m_vedioDlg;
	//CBoxView					m_Vedio;
	KeyboardMapper				m_KeyboardMapper;

	std::wstring				m_className;				//������־��ӡ������
	CString						m_strSDCardPath;			//SD����·����
	std::string					m_strCurrentRunningApp;		//��ǰ�������е�APP������
	ULONG						ulAvailable;				//ʣ��ռ�
	ULONG						ulTotal;					//�ܿռ�
	USHORT						m_inputValue;				//��ǰ�Ƿ������������ֵ�ģʽ

	CString						m_strBoxPackageFile;		//�������������ں����еĴ��·��
	CString						m_ipStr;

public:

	COLORREF					GetWindowPixelColor(HWND hWnd, int x, int y);
	UINT						GetWindowColorValue(HWND hWnd);
	void						StartConnectProc();

	void						ClientToDeviceScreen(int &x, int &y);
	void						DeviceScreenToScreen(int &x, int &y);
protected:

};
