#include "Sample.h"
#include "WSys.h"
#include "WBitmapMgr.h"

bool	Sample::Init() {

	return true;}
bool	Sample::Frame() {
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].Frame();
	}

	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{
		m_Hero.Up();
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{
		m_Hero.Down();
	}
	if (m_Input.KeyCheck('A') == KEY_HOLD)
	{
		m_Hero.Left();
	}
	if (m_Input.KeyCheck('D') == KEY_HOLD)
	{
		m_Hero.Right();
	}
	/*if (m_Input.KeyCheck(VK_SPACE) == KEY_HOLD)
	{
	m_Hero.SetSpeed(30.0f);
	}
	else
	{
	m_Hero.SetSpeed(-100.0f);
	}*/
	return true;}

bool	Sample::Render() {

	return true;}
bool	Sample::Release() {

}

Sample::Sample()
{
	m_iX = 0;
	m_iY = 0;
}


Sample::~Sample()
{
}

int WINAPI wWinMain(
	HINSTANCE hInstatnce,
	HINSTANCE hPrevInstatnce,
	LPWSTR   lpCmdLine,
	int      nCmdShow)
{
	Sample win;
	if (win.SetWindow(hInstatnce) == true)
	{
		win.Run();
	}
	return 0;
}