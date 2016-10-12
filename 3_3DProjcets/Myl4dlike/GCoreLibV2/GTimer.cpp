#include "GTimer.h"
namespace GBASIS {
float g_fSecPerFrame = 0.0f;
float g_fDurationTime = 0.0f;
}

bool GTimer::Init()
{
	QueryPerformanceCounter( &m_Frame );
	
	// ���ػ� Ÿ�̸� �������� �Ǵ�
	if( m_Frame.QuadPart == 0 )
	{
		return false;
	}

	m_FPS				= m_Frame;
	m_iFramePerSecond	= 0;
	m_iFPSElapse		= 0;
	m_fSecPerFrame		= 0.0f;		

	Start();
	return true;
}
bool GTimer::Frame()
{
	// 1�������� �ð� üũ 
	// ���� �ð��� ã�´�. 
	QueryPerformanceCounter( &m_Current );		
	g_fSecPerFrame = m_fSecPerFrame =  static_cast<float>( m_Current.QuadPart-m_Frame.QuadPart ) / static_cast<float>(m_Frequency.QuadPart);	
	g_fDurationTime = m_fDurationTime += m_fSecPerFrame;
	// 1�ʴ� ������ üũ 	( 0 ���� ũ�ٴ� ���� �����̱� ������ 1 �� �ȴ�. )
	if( ( ( m_Current.QuadPart-m_FPS.QuadPart ) / m_Frequency.QuadPart)  >= 1 )
	{		
		m_iFramePerSecond	= m_iFPSElapse;
		m_iFPSElapse		= 0;
		m_FPS				= m_Current;
	}
	m_iFPSElapse++;

	m_Frame = m_Current;

	return true;
}

int	GTimer::GetFPS() 
{
	return m_iFramePerSecond;
}
float GTimer::GetSPF()
{
	return m_fSecPerFrame;
}

bool GTimer::Render()
{
	return true;
}
bool GTimer::Release()
{	
	return true;
}

void GTimer::Reset()
{
   m_bStarted = false;
   m_fEventTime = 0.0f;
   memset( &m_Start, 0, sizeof( LARGE_INTEGER ) );
   memset( &m_Elapse, 0, sizeof( LARGE_INTEGER ) );
}

void GTimer::Start()
{
   m_bStarted	= true;
   m_fEventTime = 0.0f;
   QueryPerformanceCounter((LARGE_INTEGER *)&m_Start);
}

void GTimer::Stop()
{
   GetElapsedTime();
   m_bStarted = false;
}


bool GTimer::IsStarted() const
{
   return m_bStarted;
}


float GTimer::GetElapsedTime()
{   
   if( m_bStarted )
   {
      QueryPerformanceCounter((LARGE_INTEGER *)&m_Elapse);
      m_fEventTime = static_cast<float>(m_Elapse.QuadPart - m_Start.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
   }
   return m_fEventTime;
}

GTimer::GTimer() 
{   
    m_bStarted			=  false;   	
   	m_iFramePerSecond	= 0;
	m_iFPSElapse		= 0;
	m_fSecPerFrame		= 0.0f;	
	m_fEventTime		= 0.0f;
	m_fDurationTime = 0.0f;
   	// �ý����� ���ļ� �������� ��� �´�. �̴� �ð��� ǥ���� ��� ���ؼ� ����Ѵ�.
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_Frequency);
}

GTimer::~GTimer()
{
}