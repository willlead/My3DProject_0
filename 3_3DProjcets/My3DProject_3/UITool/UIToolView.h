
// UIToolView.h : CUIToolView Ŭ������ �������̽�
//

#pragma once


class CUIToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CUIToolView();
	DECLARE_DYNCREATE(CUIToolView)

// Ư���Դϴ�.
public:
	CUIToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CUIToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // UIToolView.cpp�� ����� ����
inline CUIToolDoc* CUIToolView::GetDocument() const
   { return reinterpret_cast<CUIToolDoc*>(m_pDocument); }
#endif

