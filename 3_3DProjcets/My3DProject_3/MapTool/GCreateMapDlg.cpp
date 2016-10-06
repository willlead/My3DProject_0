// GCreateMapDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "GCreateMapDlg.h"
#include "afxdialogex.h"


// GCreateMapDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(GCreateMapDlg, CDialogEx)

GCreateMapDlg::GCreateMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CREATEMAP, pParent)
	, m_iCellWidth(0)
	, m_iCellHeight(0)

{

}

GCreateMapDlg::~GCreateMapDlg()
{
}

void GCreateMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iCellWidth);
	DDX_Text(pDX, IDC_EDIT2, m_iCellHeight);
	DDX_Control(pDX, IDC_COMBO1, m_CBCellCount);
}


BEGIN_MESSAGE_MAP(GCreateMapDlg, CDialogEx)
	
END_MESSAGE_MAP()


// GCreateMapDlg �޽��� ó�����Դϴ�.


BOOL GCreateMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_CBCellCount.InsertString(0, L"1");
	m_CBCellCount.InsertString(1, L"2");
	m_CBCellCount.InsertString(2, L"4");
	m_CBCellCount.InsertString(3, L"8");
	m_CBCellCount.InsertString(4, L"16");
	m_CBCellCount.InsertString(5, L"32");


	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_iCellWidth = 16;
	m_iCellHeight = 16;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void GCreateMapDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	UpdateData(TRUE);


	CDialogEx::OnOK();
}
