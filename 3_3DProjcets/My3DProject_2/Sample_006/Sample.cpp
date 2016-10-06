#include "Sample.h"

HRESULT Sample::InitDevice()
{
	HRESULT hr;
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	if (FAILED(hr = CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(hr = CreateDevice()))
	{
		return false;
	}
	if (FAILED(hr = CreateSwapChain(m_hWnd, width, height)))
	{
		return false;
	}
	if (FAILED(hr = SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(hr = SetViewPort()))
	{
		return false;
	}
	return hr;
}
bool Sample::Init()
{
	HRESULT hr;

	if (FAILED(hr = InitDevice()))
	{
		CleanupDevice();
		return 0;
	}	
	
	return true;
}
bool Sample::Frame()
{


	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	return true;
}
bool Sample::Render()
{
	m_pSwapChain->Present(0, 0);
	return true;
}
bool Sample::Release() 
{
	CleanupDevice();
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}

#ifndef _DEBUG
TCORE_RUN(_T"Sample 006", 800, 600)
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int       nCmdShow)
{
	Sample sample;
	if (sample.SetWindow(hInstance, L"Sample 006", 800, 600))
	{
		sample.Run();
	}
}
#endif