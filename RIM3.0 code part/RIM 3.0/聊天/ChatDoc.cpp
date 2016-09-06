
// chattestDoc.cpp : CchattestDoc 类的实现
//

#include "stdafx.h"

#include "ChatDoc.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchattestDoc

IMPLEMENT_DYNCREATE(CChatDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CChatDoc, CRichEditDoc)
	// 启用默认的 OLE 容器实现
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// CchattestDoc 构造/析构

CChatDoc::CChatDoc()
{
	// TODO: 在此添加一次性构造代码

}

CChatDoc::~CChatDoc()
{
}

BOOL CChatDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

CRichEditCntrItem* CChatDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CChatCntrItem(preo, const_cast<CChatDoc*>(this));
}




// CchattestDoc 序列化

void CChatDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}

	// 调用基类 CRichEditDoc 将启用
	//  容器文档的 COleClientItem 对象的序列化。
	// TODO: 如果作为文本进行序列化，则设置 CRichEditDoc::m_bRTF = FALSE
	CRichEditDoc::Serialize(ar);
}


// CchattestDoc 诊断

#ifdef _DEBUG
void CChatDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CChatDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CchattestDoc 命令

BOOL CChatDoc::SaveModified()
{
	// TODO: Add your specialized code here and/or call the base class

	return FALSE;
	return CRichEditDoc::SaveModified();
}
