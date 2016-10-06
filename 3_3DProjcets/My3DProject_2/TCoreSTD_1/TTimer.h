#pragma once
#include <windows.h>

class TTimer
{
private:
	LARGE_INTEGER	m_Frequency;	// �ʴ� ���ļ� 
	LARGE_INTEGER	m_Current;		// ���� �ð� 
	LARGE_INTEGER	m_Frame;		// ���� �������� �ð� üũ 
	float			m_fSecPerFrame;	// ������ ��� �ð� 

	LARGE_INTEGER	m_FPS;				// FPS üũ Ÿ�̸�		
	int				m_iFramePerSecond;	// �ʴ� ������	
	int				m_iFPSElapse;		// �ʴ� ����� ������ 


	LARGE_INTEGER	m_Start;		// �̺�Ʈ Ÿ�̸� ���� �ð�
	LARGE_INTEGER	m_Elapse;		// �̺�Ʈ Ÿ�̸� ��� �ð�
	bool			m_bStarted;			// �̺�Ʈ Ÿ�̸� �۵�����
	float			m_fEventTime;


public:
	// �����ھ�� ����ϴ� �����Լ�
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

	// ����� �ð��� ���� ���� �Լ���
	void	Reset();
	void	Start();
	void	Stop();
	bool	IsStarted() const;
	float	GetElapsedTime();

	// FPS
	int				GetFPS();
	float			GetSPF();

public:
	TTimer();
	~TTimer();
};
