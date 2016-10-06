#pragma once
#include "WBase.h"
class WTimer : public WBase
{
public:
	TCHAR		m_csBuffer[MAX_PATH];
	int m_iFPS; // �ʴ� ������ ����
	float m_fSecondPerFrame;
	float m_fAccumulation;
	float m_fFrameTime;
	DWORD dwBeforeCurrentTIck;
	int m_dwFramecouter;

public:
	bool     Init(); // �ʱ�ȭ
	bool     Frame();// ���
	bool     Render();// ��ο�
	bool     Release();// ����, �Ҹ�
public:
	WTimer();
	virtual ~WTimer();
};

