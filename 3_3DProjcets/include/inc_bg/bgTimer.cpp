#include "bgTimer.h"
#include "bgSys.h"

// Ÿ�̸� ���� ��������
int		g_iFPS;
float	g_fSPF;
float	g_fCurrent;



void bgTimer::Reset()
{
	m_bStarted = false;
	m_fEventLength = 0.0f;
	memset(&m_EventStart, 0, sizeof(LARGE_INTEGER));
	memset(&m_EventElapse, 0, sizeof(LARGE_INTEGER));
}

void bgTimer::Start()
{
	m_bStarted = true;
	m_fEventLength = 0.0f;
	QueryPerformanceCounter(&m_EventStart);
}

void bgTimer::Stop()
{
	GetElapsedTime();
	m_bStarted = false;
}

float bgTimer::GetElapsedTime()
{
	if (m_bStarted)
	{
		QueryPerformanceCounter(&m_EventElapse);
		m_fEventLength = static_cast<float>(m_EventElapse.QuadPart - m_EventStart.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
	}
	return m_fEventLength;
}

bool bgTimer::Init()
{
	QueryPerformanceCounter(&m_Current);

	// ���ػ� Ÿ�̸� �������� false ����
	if (m_Current.QuadPart == 0)
		return false;

	g_iFPS = m_iFPS = 0;
	g_fSPF = m_fSPF = 0.0f;
	m_iCountFrame = 0;
	m_PrevSecond = m_Current;
	m_PrevFrame = m_Current;
	m_AppStartTime = m_Current;

	Start();
	return true;
}

bool bgTimer::Frame()
{
	// ���� �ð� ���
	QueryPerformanceCounter(&m_Current);
	g_fCurrent = static_cast<float>(m_Current.QuadPart - m_AppStartTime.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	// 1�������� ����� �ð� ���
	g_fSPF = m_fSPF = static_cast<float>(m_Current.QuadPart - m_PrevFrame.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	// 1�ʰ� �����ٸ�...
	if (((m_Current.QuadPart - m_PrevSecond.QuadPart) / m_Frequency.QuadPart) >= 1)
	{
		g_iFPS = m_iFPS = m_iCountFrame;	// ���� FPS �� ����
		m_PrevSecond = m_Current;			// ���ο� 1���� ���۰��� ����ð����� ����
		m_iCountFrame = 0;					// ������ ī��Ʈ �ʱ�ȭ
	}
	m_iCountFrame++;
	m_PrevFrame = m_Current;
	return true;
}

bool bgTimer::Render()
{
	return true;
}

bool bgTimer::Release()
{
	return true;
}

bgTimer::bgTimer()
{
	m_bStarted = false;
	m_iCountFrame = 0;
	m_iFPS = 0;
	m_fSPF = 0.0f;
	m_fEventLength = 0.0f;

	QueryPerformanceFrequency((LARGE_INTEGER *)&m_Frequency);
}

bgTimer::~bgTimer()
{
}
