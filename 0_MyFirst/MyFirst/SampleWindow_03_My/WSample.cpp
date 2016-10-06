#include "WSample.h"
bool Sample::Init()
{
	m_Timer.Init();
	return true;
}

bool Sample::Frame()
{
	DWORD dwCurrentTick = timeGetTime();
	return true;

}

bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{
	return true;
}

int WINAPI wWinMain(
	HINSTANCE hInstatnce,
	HINSTANCE hPrevInstatnce,
	LPWSTR   lpCmdLine,
	int      nCmdShow)
{
	Sample win;
	//win.SetRect(1024, 768);
	if (win.SetWindow(hInstatnce) == true)
	{
		win.Run();
	}
	return 0;
}

Sample::Sample()
{
}


Sample::~Sample()
{
}
