
// CntrItem.h : CchattestCntrItem 类的接口
//

#pragma once

class CChatDoc;
class CchattestView;

class CChatCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CChatCntrItem)

// 构造函数
public:
	CChatCntrItem(REOBJECT* preo = NULL, CChatDoc* pContainer = NULL);
		// 注意: 允许 pContainer 为 NULL 以启用 IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE 要求类具有带零
		//  参数的构造函数。OLE 项通常是用
		//  非 NULL 文档指针构造的

// 属性
public:
	CChatDoc* GetDocument()
		{ return reinterpret_cast<CChatDoc*>(CRichEditCntrItem::GetDocument()); }
	CchattestView* GetActiveView()
		{ return reinterpret_cast<CchattestView*>(CRichEditCntrItem::GetActiveView()); }

// 实现
public:
	~CChatCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

