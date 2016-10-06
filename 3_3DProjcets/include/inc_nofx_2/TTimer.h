#pragma once
#include "TBasisStd.h"

#define TIME(t, cmd) { t=mx_get_time(); cmd; t=mx_get_time() - t; }
extern double mx_get_time();

class CStopwatch 
{
public:
    CStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
    void Start() { QueryPerformanceCounter(&m_liPerfStart); } 
   
	__int64 Now() const
	{    // Start �� ȣ��� �� ����� �и��ʸ� ����
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		return(((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000)/ m_liPerFreq.QuadPart);
    }
private:
    LARGE_INTEGER m_liPerFreq;    // �ʴ� ���
    LARGE_INTEGER m_liPerfStart;   // ���� ���
};

// �����ġ�� �����Ѵ�(�⺻ ���� ���� �ð�).
//CStopwatch stopwatch;
// ���⼭ �ڵ带 �����Ѵ�.
// ���ݱ��� �󸶳� ���� �ð��� ��� �Ǿ����� ��´�.
//__int64 qwElapsedTime = stopwatch.Now();

class TTimer : public TSingleton < TTimer >
{
private:
	friend class TSingleton<TTimer>;
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

protected: 
	TTimer();  
public:
	virtual ~TTimer();
};
#define I_Timer TTimer::GetInstance()