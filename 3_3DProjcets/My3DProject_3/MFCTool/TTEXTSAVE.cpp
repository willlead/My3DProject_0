// TTEXTSAVE.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "TTEXTSAVE.h"
#include "afxdialogex.h"


// TTEXTSAVE 대화 상자입니다.

IMPLEMENT_DYNAMIC(TTEXTSAVE, CDialogEx)

TTEXTSAVE::TTEXTSAVE(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXTSAVE, pParent)
{

}

TTEXTSAVE::~TTEXTSAVE()
{
}

void TTEXTSAVE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TTEXTSAVE, CDialogEx)
END_MESSAGE_MAP()


// TTEXTSAVE 메시지 처리기입니다.
