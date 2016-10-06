#ifndef TBASIS_EXTERN_H
#define TBASIS_EXTERN_H
#include <d3dx11.h> 
	//--------------------------------------------------------------------------------------
	// Global Variables
	//--------------------------------------------------------------------------------------	
	extern ID3D11Device*           g_pd3dDevice;		// ����̽� ��ü
	extern IDXGISwapChain*         g_pSwapChain;		// ����ü�� ��ü
	extern ID3D11RenderTargetView* g_pRenderTargetView;// ���� ����Ÿ�� ��	
	extern IDXGIFactory*		   g_pGIFactory;		// DXGI ��ü
	extern ID3D11DeviceContext*    g_pImmediateContext;
	extern D3D_FEATURE_LEVEL       g_FeatureLevel;
#endif