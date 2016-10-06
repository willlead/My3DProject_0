// TMAPLOAD.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "TMAPLOAD.h"
#include "afxdialogex.h"


// TMAPLOAD 대화 상자입니다.

IMPLEMENT_DYNAMIC(TMAPLOAD, CDialogEx)

TMAPLOAD::TMAPLOAD(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPLOAD, pParent)
{

}

TMAPLOAD::~TMAPLOAD()
{
}

void TMAPLOAD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TMAPLOAD, CDialogEx)
END_MESSAGE_MAP()


// TMAPLOAD 메시지 처리기입니다.
