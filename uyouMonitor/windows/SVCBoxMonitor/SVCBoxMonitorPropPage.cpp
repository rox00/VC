// SVCBoxMonitorPropPage.cpp : Implementation of the CSVCBoxMonitorPropPage property page class.

#include "stdafx.h"
#include "SVCBoxMonitor.h"
#include "SVCBoxMonitorPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSVCBoxMonitorPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CSVCBoxMonitorPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSVCBoxMonitorPropPage, "SVCBoxMonitor.SVCBoxMonitorPropPage.1",
	0x795cad23, 0x92a6, 0x45d1, 0xba, 0x8, 0x79, 0xf9, 0xae, 0x27, 0x5d, 0x49 )



// CSVCBoxMonitorPropPage::CSVCBoxMonitorPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSVCBoxMonitorPropPage

BOOL CSVCBoxMonitorPropPage::CSVCBoxMonitorPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SVCBoxMonitor_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CSVCBoxMonitorPropPage::CSVCBoxMonitorPropPage - Constructor

CSVCBoxMonitorPropPage::CSVCBoxMonitorPropPage() :
	COlePropertyPage(IDD, IDS_SVCBoxMonitor_PPG_CAPTION)
{
}



// CSVCBoxMonitorPropPage::DoDataExchange - Moves data between page and properties

void CSVCBoxMonitorPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CSVCBoxMonitorPropPage message handlers
