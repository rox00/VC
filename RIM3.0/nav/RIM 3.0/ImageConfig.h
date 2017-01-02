#pragma once


// CImageConfig

class ImageConfig
{
public:
	virtual ~ImageConfig();

protected:
	ImageConfig();
	static ImageConfig*						m_pImageConfig;				//内部实例指针

public:
	static ImageConfig* 					GetInstance();
	static void 							ReleaseInstance();

	////【zy 2015-05-29 ↓】	主界面共用
	Image*									m_pImgSizeMid;				//用于拉伸时绘制中间部分的PNG图片
	Image*									m_pImgSizebottom;			//用于拉伸时绘制底部的PNG图片

	////【zy 2015-05-29 ↓】 CRIM30Dlg
	Image*									m_pImgChkSel;				//用于绘制下部按钮选中状态
	Image*									m_pImgChkUnSel;				//用于绘制下部按钮未选中状态

	////【zy 2015-05-29 ↓】 CLogonWndDlg
	Image*									m_pImgLogon;				//登录界面图片绘制在主界面矩形的上部

	////【zy 2015-05-29 ↓】 CCircleDlg
	Image*									m_pImgLogo;					//用于绘制圆环中心区域的Logo图片
	Image*									m_pImgCircle;				//用于绘制圆环区域的图片

	////【zy 2015-05-29 ↓】 CMainListDlg
	Image*									m_pImgMainList;				//用于绘制登录后主界面矩形上部的PNG图片

	////【zy 2015-05-29 ↓】 CDialogSet
	Image*									m_pImageDlgSetBk;		


	////【zy 2015-05-29 ↓】 CChatBkDlg
	Image*									m_ImageChatBk;				////用户选择图像
	Image*									m_ImageUserSel;				////用户选择图像
	Image*									m_ImageMinNormal;			////最小化正常图像
	Image*									m_ImageMinPush;				////最小化鼠标经过或按下时的图像
	Image*									m_ImageRestoreNormal;		////还原按钮正常图像
	Image*									m_ImageRestorePush;			////还原按钮鼠标经过或按下时的图像
	Image*									m_ImageMaxNormal;			////最大化正常图像
	Image*									m_ImageMaxPush;				////最大化鼠标经过或按下时的图像
	Image*									m_ImageCloseNormal;			////关闭按钮正常图像
	Image*									m_ImageClosePush;			////关闭按钮鼠标经过或按下时的图像
	Image*									m_ImageSendNormal;			////发送按钮正常图像
	Image*									m_ImageSendPush;			////发送按钮鼠标经过或按下时的图像
	Image*									m_ImageClose1Normal;		////关闭按钮正常图像
	Image*									m_ImageClose1Push;			////关闭按钮鼠标经过或按下时的图像

	Image*									m_RichImageBk;				////RichEdit背景图片	目前没有使用但
	Image*									m_FileNormal;				////文件图标的正常图像
	Image*									m_FilePush;					////文件图标按下时的图像
	Image*									m_DicNormal;				////文件夹图标的正常图像
	Image*									m_DicPush;					////文件夹图标按下时的图像
	Image*									m_PictureNormal;			////发送图片按钮正常图像
	Image*									m_PicturePush;				////发送图片按钮按下时的图像

	////【zy 2015-05-29 ↓】 CChatRichView
	Image*									m_pImgScrollBk;	
	Image*									m_pImgHScrollThumb;
	Image*									m_pImgVScrollThumb;


};


