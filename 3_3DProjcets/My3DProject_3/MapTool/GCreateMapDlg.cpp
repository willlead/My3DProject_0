// GCreateMapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "GCreateMapDlg.h"
#include "afxdialogex.h"


// GCreateMapDlg 대화 상자입니다.

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


// GCreateMapDlg 메시지 처리기입니다.


BOOL GCreateMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_CBCellCount.InsertString(0, L"1");
	m_CBCellCount.InsertString(1, L"2");
	m_CBCellCount.InsertString(2, L"4");
	m_CBCellCount.InsertString(3, L"8");
	m_CBCellCount.InsertString(4, L"16");
	m_CBCellCount.InsertString(5, L"32");


	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_iCellWidth = 16;
	m_iCellHeight = 16;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void GCreateMapDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	UpdateData(TRUE);


	CDialogEx::OnOK();
}
