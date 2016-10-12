#pragma once
#include "afxwin.h"


// GCreateMapDlg 대화 상자입니다.

class GCreateMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GCreateMapDlg)

public:
	GCreateMapDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~GCreateMapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iCellWidth;
	int m_iCellHeight;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_CBCellCount;

};
