#pragma once


// TCeateMapDlg ��ȭ �����Դϴ�.

class TCeateMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TCeateMapDlg)

public:
	TCeateMapDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TCeateMapDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
