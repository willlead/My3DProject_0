#pragma once


// TTEXTSAVE ��ȭ �����Դϴ�.

class TTEXTSAVE : public CDialogEx
{
	DECLARE_DYNAMIC(TTEXTSAVE)

public:
	TTEXTSAVE(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TTEXTSAVE();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTSAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
