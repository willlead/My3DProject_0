#pragma once
#include "TStd.h"
class TTimer
{
public:
	TCHAR		m_csBuffer[MAX_PATH];
	int			m_iFPS;// 1�ʴ� ������ ����
	float		m_fSecondPerFrame; // 1�������� ����ð�
	float		m_fAccumulation;// ����� ��� �ð�

	DWORD		m_dwBeforeTick;
	float		m_fFrameTime; // �ӽú���
	DWORD		m_dwFrameCnt;
public:
	// Ŭ���� ��� ���� �ʱ�ȭ
	bool		Init();
	// �� �����ӿ��� ����� ����Ѵ�.
	bool		Frame();
	// �� �����ӿ��� ��ο츦 ����Ѵ�.
	bool		Render();
	// Ŭ���� ��� ���� �Ҹ��� ����Ѵ�.
	bool		Release();
public:
	TTimer();
	virtual ~TTimer();
};

