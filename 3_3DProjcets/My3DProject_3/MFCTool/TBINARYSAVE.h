#pragma once


// TBINARYSAVE ��ȭ �����Դϴ�.

class TBINARYSAVE : public CDialogEx
{
	DECLARE_DYNAMIC(TBINARYSAVE)

public:
	TBINARYSAVE(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TBINARYSAVE();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARYSAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
