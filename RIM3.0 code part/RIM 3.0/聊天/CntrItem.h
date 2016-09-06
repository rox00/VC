
// CntrItem.h : CchattestCntrItem ��Ľӿ�
//

#pragma once

class CChatDoc;
class CchattestView;

class CChatCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CChatCntrItem)

// ���캯��
public:
	CChatCntrItem(REOBJECT* preo = NULL, CChatDoc* pContainer = NULL);
		// ע��: ���� pContainer Ϊ NULL ������ IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE Ҫ������д���
		//  �����Ĺ��캯����OLE ��ͨ������
		//  �� NULL �ĵ�ָ�빹���

// ����
public:
	CChatDoc* GetDocument()
		{ return reinterpret_cast<CChatDoc*>(CRichEditCntrItem::GetDocument()); }
	CchattestView* GetActiveView()
		{ return reinterpret_cast<CchattestView*>(CRichEditCntrItem::GetActiveView()); }

// ʵ��
public:
	~CChatCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

