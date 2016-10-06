#pragma once
#include "TDefine.h"
class TDevice
{
public:
	DWORD		m_dwWidth;
	DWORD		m_dwHeight;

	DXGI_SWAP_CHAIN_DESC	m_sd;
	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	IDXGIFactory*			g_pGIFactory;
	ID3D11Device*           g_pd3dDevice = NULL;
	ID3D11DeviceContext*    g_pImmediateContext = NULL;
	IDXGISwapChain*         g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_pRenderTargetView = NULL;

	ID3D11VertexShader*     g_pVertexShader = NULL;
	ID3D11PixelShader*      g_pPixelShader = NULL;
	ID3D11InputLayout*      g_pVertexLayout = NULL;

	D3DXVECTOR3				g_Eye;



public:
	HRESULT CreateGIFactory();
	HRESULT InitDevice();
	HRESULT InitSwapChain();
	HRESULT SetRenderTarget();
	HRESULT SetViewPort();
	void	CleanupDevice();
	HRESULT  CreateDXResouce();
	HRESULT  DeleteDXResouce();
	bool   ResizeDevice(UINT iWidth, UINT iHeight);

public:
	TDevice();
	virtual ~TDevice();
};
