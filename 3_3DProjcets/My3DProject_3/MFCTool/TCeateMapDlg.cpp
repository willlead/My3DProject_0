// TCeateMapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "TCeateMapDlg.h"
#include "afxdialogex.h"


// TCeateMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(TCeateMapDlg, CDialogEx)

TCeateMapDlg::TCeateMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CREATEMAP, pParent)
{

}

TCeateMapDlg::~TCeateMapDlg()
{
}

void TCeateMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TCeateMapDlg, CDialogEx)
END_MESSAGE_MAP()


// TCeateMapDlg 메시지 처리기입니다.
