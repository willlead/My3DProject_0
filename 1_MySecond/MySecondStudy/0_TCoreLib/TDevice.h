#pragma once
#include "TStd.h"

class TDevice
{
public:
	DWORD m_dwWidth;
	DWORD m_dwHeight;
//public:
//	D3D_DRIVER_TYPE         g_driverType;
//	D3D_FEATURE_LEVEL       g_featureLevel;
//	ID3D11Device*           g_pd3dDevice;
//	ID3D11DeviceContext*    g_pImmediateContext;
//	IDXGISwapChain*         g_pSwapChain;
//	ID3D11RenderTargetView* g_pRenderTargetView;
//
public:
	HRESULT InitDevice();
	HRESULT InitSwapChain();
	HRESULT SetRenderTarget();
	HRESULT SetViewPort();
	void CleanupDevice();
public:
	TDevice();
	virtual ~TDevice();
};

