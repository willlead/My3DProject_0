#pragma once


// TMAPLOAD ��ȭ �����Դϴ�.

class TMAPLOAD : public CDialogEx
{
	DECLARE_DYNAMIC(TMAPLOAD)

public:
	TMAPLOAD(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TMAPLOAD();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
