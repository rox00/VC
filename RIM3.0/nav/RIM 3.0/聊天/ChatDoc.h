
#pragma once


class CChatDoc : public CRichEditDoc
{
protected: // �������л�����
	CChatDoc();
	DECLARE_DYNCREATE(CChatDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// ʵ��
public:
	virtual ~CChatDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL SaveModified();
};


