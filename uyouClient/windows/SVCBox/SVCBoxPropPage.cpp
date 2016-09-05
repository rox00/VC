// SVCBoxPropPage.cpp : Implementation of the CSVCBoxPropPage property page class.

#include "stdafx.h"
#include "SVCBox.h"
#include "SVCBoxPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSVCBoxPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CSVCBoxPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSVCBoxPropPage, "SVCBOX.SVCBoxPropPage.1",
	0x48e4a1da, 0xc8c, 0x4bc7, 0x9b, 0x9b, 0x9e, 0xd8, 0xe9, 0x33, 0xbe, 0x21)



// CSVCBoxPropPage::CSVCBoxPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSVCBoxPropPage

BOOL CSVCBoxPropPage::CSVCBoxPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SVCBOX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CSVCBoxPropPage::CSVCBoxPropPage - Constructor

CSVCBoxPropPage::CSVCBoxPropPage() :
	COlePropertyPage(IDD, IDS_SVCBOX_PPG_CAPTION)
{
}



// CSVCBoxPropPage::DoDataExchange - Moves data between page and properties

void CSVCBoxPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CSVCBoxPropPage message handlers
