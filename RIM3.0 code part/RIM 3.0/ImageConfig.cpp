// ImageConfig.cpp : implementation file
//

#include "stdafx.h"
#include "ImageConfig.h"


// CImageConfig

ImageConfig* ImageConfig::m_pImageConfig			= NULL;	//当前配置实例
ImageConfig* ImageConfig::GetInstance()
{
	if(m_pImageConfig == NULL)
	{
		m_pImageConfig = new ImageConfig();
	}

	return m_pImageConfig;
}

void ImageConfig::ReleaseInstance()
{
	if(m_pImageConfig)
	{
		delete m_pImageConfig;
		m_pImageConfig = NULL;
	}
}

ImageConfig::ImageConfig()
{
	////【zy 2015-05-29 ↓】	主界面共用
	m_pImgSizeMid			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\sizemiddle.png")).c_str()));
	m_pImgSizebottom		= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\sizebottom.png")).c_str()));

	////【zy 2015-05-29 ↓】 CRIM30Dlg
	m_pImgChkSel			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\mainlist\\sel.png")).c_str()));
	m_pImgChkUnSel			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\mainlist\\unsel.png")).c_str()));

	////【zy 2015-05-29 ↓】 CLogonWndDlg
	m_pImgLogon				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\logon.png")).c_str()));


	////【zy 2015-05-29 ↓】 CCircleDlg
	m_pImgLogo				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\logo.png")).c_str()));
	m_pImgCircle			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\circle.png")).c_str()));

	////【zy 2015-05-29 ↓】 CMainListDlg
	m_pImgMainList			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\mainlist.png")).c_str()));

	////【zy 2015-05-29 ↓】 CDialogSet
	m_pImageDlgSetBk		= Bitmap::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\systemset\\systemset.bmp")).c_str()));



	////【zy 2015-05-29 ↓】 CChatBkDlg
	m_ImageChatBk				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\imgbk.png")).c_str()));
	m_ImageUserSel				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\usersel.png")).c_str()));
	m_ImageMinNormal			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\minnormal.png")).c_str()));
	m_ImageMinPush				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\minpush.png")).c_str()));
	m_ImageRestoreNormal		= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\restorenormal.png")).c_str()));
	m_ImageRestorePush			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\restorepush.png")).c_str()));
	m_ImageMaxNormal			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\maxnormal.png")).c_str()));
	m_ImageMaxPush				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\maxpush.png")).c_str()));
	m_ImageCloseNormal			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\closenormal.png")).c_str()));
	m_ImageClosePush			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\closepush.png")).c_str()));
	m_ImageSendNormal			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\sendnormal.png")).c_str()));
	m_ImageSendPush				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\sendpush.png")).c_str()));
	m_ImageClose1Normal			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\close1normal.png")).c_str()));
	m_ImageClose1Push			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\close1push.png")).c_str()));

	m_RichImageBk				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\richBk.png")).c_str()));
	m_FileNormal				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\filenormal.png")).c_str()));
	m_FilePush					= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\filepush.png")).c_str()));
	m_DicNormal					= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\dicnormal.png")).c_str()));
	m_DicPush					= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\dicpush.png")).c_str()));
	m_PictureNormal				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\imgnormal.png")).c_str()));
	m_PicturePush				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\imgpush.png")).c_str()));

	////【zy 2015-05-29 ↓】 CChatRichView
	m_pImgScrollBk				= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\chatdlg\\richBk.png")).c_str()));
	m_pImgHScrollThumb			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\scroll\\hscrollThumb.png")).c_str()));
	m_pImgVScrollThumb			= Image::FromFile(CStringW(RelativeToFullPath(_T("res\\PNG\\scroll\\vscrollThumb.png")).c_str()));
}	

ImageConfig::~ImageConfig()
{
}

// CImageConfig message handlers


