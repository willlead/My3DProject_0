#include "Sample.h"



Sample::Sample()
{
}


Sample::~Sample()
{
}

#ifndef _DEBUG
#define TCORE_RUN(s,x,y) TCORE_START;TCORE_WIN(s,x,y);
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int       nCmdShow)
{
	Sample sample;
	if (sample.SetWindow(hInstance, _T("Sample Timer"), 800, 600))
	{
		sample.Run();
	}
}
#endif