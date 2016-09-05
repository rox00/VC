#pragma once

// SVCBoxMonitorPropPage.h : Declaration of the CSVCBoxMonitorPropPage property page class.


// CSVCBoxMonitorPropPage : See SVCBoxMonitorPropPage.cpp for implementation.

class CSVCBoxMonitorPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSVCBoxMonitorPropPage)
	DECLARE_OLECREATE_EX(CSVCBoxMonitorPropPage)

// Constructor
public:
	CSVCBoxMonitorPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SVCBoxMonitor };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

