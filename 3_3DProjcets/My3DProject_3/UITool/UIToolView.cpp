
// UIToolView.cpp : CUIToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "UITool.h"
#endif

#include "UIToolDoc.h"
#include "UIToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIToolView

IMPLEMENT_DYNCREATE(CUIToolView, CView)

BEGIN_MESSAGE_MAP(CUIToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CUIToolView ����/�Ҹ�

CUIToolView::CUIToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CUIToolView::~CUIToolView()
{
}

BOOL CUIToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CUIToolView �׸���

void CUIToolView::OnDraw(CDC* /*pDC*/)
{
	CUIToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CUIToolView �μ�

BOOL CUIToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CUIToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CUIToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CUIToolView ����

#ifdef _DEBUG
void CUIToolView::AssertValid() const
{
	CView::AssertValid();
}

void CUIToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUIToolDoc* CUIToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIToolDoc)));
	return (CUIToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CUIToolView �޽��� ó����
