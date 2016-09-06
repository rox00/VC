//********************************************************************
//	PNGMENU.H 文件注释
//	文件名 : PNGMENU.H
//	文件路径: E:\MY PROJECT\VISUAL STUDIO 2008\MYSKINDLG\MYSKINDLG\CONTROLS/
//	作者 : 张永
//	创建时间: 2014/12/3 11:14
//	文件描述: 
//*********************************************************************	

#pragma once


// CPngMenu

class CPngMenu : public CMenu
{
public:
	CPngMenu();
	virtual ~CPngMenu();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


