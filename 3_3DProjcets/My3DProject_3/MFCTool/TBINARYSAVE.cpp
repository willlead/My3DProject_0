// TBINARYSAVE.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "TBINARYSAVE.h"
#include "afxdialogex.h"


// TBINARYSAVE 대화 상자입니다.

IMPLEMENT_DYNAMIC(TBINARYSAVE, CDialogEx)

TBINARYSAVE::TBINARYSAVE(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BINARYSAVE, pParent)
{

}

TBINARYSAVE::~TBINARYSAVE()
{
}

void TBINARYSAVE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TBINARYSAVE, CDialogEx)
END_MESSAGE_MAP()


// TBINARYSAVE 메시지 처리기입니다.
