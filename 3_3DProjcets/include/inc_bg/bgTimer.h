#pragma once
#include "bgStd.h"

class bgTimer : public bgSingleton<bgTimer>
{
public:
	LARGE_INTEGER	m_Frequency;	// ���� �ý����� �ʴ� ���ļ�
	LARGE_INTEGER	m_AppStartTime;	// ���α׷� ���� �ð�
	LARGE_INTEGER	m_Current;		// ���� �ð�
	LARGE_INTEGER	m_PrevFrame;	// 1������ ���� �ð�
	LARGE_INTEGER	m_PrevSecond;	// 1�� ���� �ð�
	float			m_fSPF;			// 1������ ��� �ð� (���������� �ð������� ����)
	int				m_iFPS;			// 1�ʴ� �����Ӽ� (FPS)
	int				m_iCountFrame;	// ������ ī��Ʈ (1�ʵ���)

	LARGE_INTEGER	m_EventStart;	// ���� �ð�
	LARGE_INTEGER	m_EventElapse;	// ��� �ð�
	float			m_fEventLength;	// ���� �ð� (���۽ð������� ����)
	bool			m_bStarted;		// Ȱ�� ����

public:
	bgTimer();
	virtual ~bgTimer();

public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

public:
	void	Reset();
	void	Start();
	void	Stop();
	float	GetElapsedTime();

	bool	IsStarted() { return m_bStarted; }
	int		GetFPS() { return m_iFPS; }
	float	GetSPF() { return m_fSPF; }
};
