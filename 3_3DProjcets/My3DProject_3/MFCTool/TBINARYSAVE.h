#pragma once


// TBINARYSAVE 대화 상자입니다.

class TBINARYSAVE : public CDialogEx
{
	DECLARE_DYNAMIC(TBINARYSAVE)

public:
	TBINARYSAVE(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TBINARYSAVE();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARYSAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
