
// chattestDoc.cpp : CchattestDoc ���ʵ��
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
	// ����Ĭ�ϵ� OLE ����ʵ��
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// CchattestDoc ����/����

CChatDoc::CChatDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CChatDoc::~CChatDoc()
{
}

BOOL CChatDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

CRichEditCntrItem* CChatDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CChatCntrItem(preo, const_cast<CChatDoc*>(this));
}




// CchattestDoc ���л�

void CChatDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}

	// ���û��� CRichEditDoc ������
	//  �����ĵ��� COleClientItem ��������л���
	// TODO: �����Ϊ�ı��������л��������� CRichEditDoc::m_bRTF = FALSE
	CRichEditDoc::Serialize(ar);
}


// CchattestDoc ���

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


// CchattestDoc ����

BOOL CChatDoc::SaveModified()
{
	// TODO: Add your specialized code here and/or call the base class

	return FALSE;
	return CRichEditDoc::SaveModified();
}
