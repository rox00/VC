#pragma once

// SVCBoxPropPage.h : Declaration of the CSVCBoxPropPage property page class.


// CSVCBoxPropPage : See SVCBoxPropPage.cpp for implementation.

class CSVCBoxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSVCBoxPropPage)
	DECLARE_OLECREATE_EX(CSVCBoxPropPage)

// Constructor
public:
	CSVCBoxPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SVCBOX };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

