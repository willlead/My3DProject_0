#include "TCore.h"

bool TCore::GameInit()
{
	//if (FAILED(CreateGIFactory())) { return 0; }

	if (FAILED(InitDevice())) { return 0; }
	if (FAILED(InitSwapChain())) { return 0; }
	if (FAILED(SetRenderTarget())) { return 0; }
	if (FAILED(SetViewPort())) { return 0; }

	/*if (FAILED(CreateVertexShader())) { return 0; }
	if (FAILED(DefineInputLayout())) { return 0; }
	if (FAILED(CreatePixelShader())) { return 0; }

	if (FAILED(CreateDXResouce())) { return 0; }
	if (FAILED(DeleteDXResouce())) { return 0; }*/

	m_Timer.Init();
	m_Input.Init();

	COLORREF bkColor = RGB(255, 255, 255);

	// ÆùÆ®
	m_fontDefault = CreateFont(20, 0, 0, FW_BOLD,
		0, 0, 0, 0,
		HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN,
		_T("°íµñ"));
	
	Init();
	return true;
}
bool TCore::GameRelease()
{
	Release();
	DeleteObject(m_fontDefault);
		
	m_Timer.Release();
	m_Input.Release();

	//CleanupDevice();
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

bool TCore::GameRender()
{
	
	Render();
	m_Timer.Render();
	m_Input.Render();
	
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
