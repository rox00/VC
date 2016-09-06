
// CntrItem.cpp : CchattestCntrItem 类的实现
//

#include "stdafx.h"

#include "ChatDoc.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchattestCntrItem 的实现

IMPLEMENT_SERIAL(CChatCntrItem, CRichEditCntrItem, 0)

CChatCntrItem::CChatCntrItem(REOBJECT* preo, CChatDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: 在此添加一次性构造代码
}

CChatCntrItem::~CChatCntrItem()
{
	// TODO: 在此处添加清理代码
}


// CchattestCntrItem 诊断

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

