#pragma once
#include "afxwin.h"


// GCreateMapDlg ��ȭ �����Դϴ�.

class GCreateMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GCreateMapDlg)

public:
	GCreateMapDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~GCreateMapDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_iCellWidth;
	int m_iCellHeight;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_CBCellCount;

};
