
// CntrItem.cpp : CchattestCntrItem ���ʵ��
//

#include "stdafx.h"

#include "ChatDoc.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchattestCntrItem ��ʵ��

IMPLEMENT_SERIAL(CChatCntrItem, CRichEditCntrItem, 0)

CChatCntrItem::CChatCntrItem(REOBJECT* preo, CChatDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: �ڴ����һ���Թ������
}

CChatCntrItem::~CChatCntrItem()
{
	// TODO: �ڴ˴�����������
}


// CchattestCntrItem ���

#ifdef _DEBUG
void CChatCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CChatCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

