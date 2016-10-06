#include "TCore.h"

bool TCore::GameInit()
{
	//if (FAILED(CreateGIFactory()))
	//{
	//	return 0;
	//}
	//if (FAILED(InitDevice()))
	//{
	//	CleanupDevice();
	//	return 0;
	//}
	//if (FAILED(InitSwapChain()))
	//{
	//	return 0;
	//}
	//if (FAILED(SetRenderTarget()))
	//{
	//	return 0;
	//}
	//if (FAILED(SetViewPort()))
	//{
	//	return 0;
	//}
	//TDxState::SetState(g_pd3dDevice);

	m_Timer.Init();
	I_Input.Init();

	// ÆùÆ®
	m_fontDefault = CreateFont(20, 0, 0, FW_BOLD,
		0, 0, 0, 0,
		HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN,
		_T("°íµñ"));
	//m_fontDefaultOld = (HFONT)SelectObject(m_hOffScreenDC, m_fontDefault);
	Init();
	return true;
}
bool TCore::GameRelease()
{
	Release();
	DeleteObject(m_fontDefault);
	m_Timer.Release();
	m_Input.Release();
	return true;
}
bool TCore::GameFrame()
{
	m_Timer.Frame();
	m_Input.Frame();
	PreFrame();
	Frame();
	PostFrame();
	return true;
}
bool TCore::PreRender()
{
	return true;
}
bool TCore::PostRender()
{
	return true;
}
bool TCore::DrawDebug(TCHAR* pString, int iX, int iY)
{
	return true;
}
bool TCore::DrawDebug()
{

	return true;
}
bool TCore::GameRender()
{
	PreRender();
	Render();
	m_Timer.Render();
	m_Input.Render();
	DrawDebug();
	PostRender();
	return true;
}
bool TCore::GameRun()
{
	GameFrame();
	GameRender();
	return true;
};
void TCore::MsgEvent(MSG msg)
{
	m_Input.MsgEvent(msg);
};
TCore::TCore()
{
}


TCore::~TCore()
{
}
