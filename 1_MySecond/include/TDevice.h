#pragma once
#include "TStd.h"

class TDevice
{
public:
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	//D3D_DRIVER_TYPE         g_driverType;
	//D3D_FEATURE_LEVEL       g_featureLevel;
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

