#pragma once
#include "TDefine.h"
#include "TDxState.h"

class TDevice 
{
public:
	//--------------------------------------------------------------------------------------
	// Global Variables
	//--------------------------------------------------------------------------------------	
	ID3D11Device*           m_pd3dDevice;		// ����̽� ��ü
	IDXGISwapChain*         m_pSwapChain;		// ����ü�� ��ü
	ID3D11RenderTargetView* m_pRenderTargetView;// ���� ����Ÿ�� ��	
	IDXGIFactory*			m_pGIFactory;		// DXGI ��ü
	ID3D11DeviceContext*    m_pImmediateContext;
	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*    GetContext();
	IDXGISwapChain*			GetSwapChain();
	ID3D11RenderTargetView*	GetRenderTargetView();
	IDXGIFactory*			GetGIFactory();

public:
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;
	D3D11_VIEWPORT			m_MainVP;			// ����Ʈ 
	D3D_DRIVER_TYPE         m_driverType;		// ����̽� Ÿ��( ����Ʈ:�ϵ���� ���� )	
	D3D_FEATURE_LEVEL       m_FeatureLevel;
	BOOL					m_IsFullScreenMode;	// Ǯ��ũ�� ����̽� �����Ǵ�
	BOOL					GetFullScreenFlag();
	void					SetFullScreenFlag( BOOL bFlag );
	//--------------------------------------------------------------------------------------
	// ���� �� ���ٽ� ������ �� ���ҽ� �� ��������
	//--------------------------------------------------------------------------------------
	ID3D11DepthStencilView*		m_pDepthStencilView;
	TDxState					m_StateDepthStencil;
	TDxState					m_StateBackCullSolid;
	TDxState					m_StateNoneCullSolid;	
	TDxState					m_StateFrontCullSolid;	
	HRESULT						CreateDepthStencilView( UINT iWidth, UINT iHeight );
	HRESULT						CreateDepthStencilState();
	ID3D11DepthStencilView*		GetDepthStencilView() { return m_pDepthStencilView;};
public:
	//--------------------------------------------------------------------------------------
	// ����̽� �� ����ü�� ���� : InitDevice()
	//--------------------------------------------------------------------------------------
	HRESULT		InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen= FALSE);
	// 0, InitDevice()
	// 1, CreateGIFactory()
	// 2, CreateDevice()
	// 3, CreateSwapChain()
	// 4, SetRenderTargetView()
	// 5, SetViewPort()
	//--------------------------------------------------------------------------------------
	HRESULT		CreateDevice( );
	HRESULT		CreateGIFactory();
	HRESULT		CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeigh, BOOL IsFullScreen = FALSE);
	HRESULT		CreateDeviceAndSwapChain(HWND hWnd, UINT iWidth, UINT iHeigh, BOOL IsFullScreen = FALSE);
	//--------------------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------------------	
	bool		CleanupDevice();
	void		ClearD3D11DeviceContext( ID3D11DeviceContext* pd3dDeviceContext );


	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���Ѵ�.
	//--------------------------------------------------------------------------------------
	HRESULT		ResizeDevice( UINT iWidth, UINT iHeight);
	virtual		HRESULT		SetRenderTargetView();
	virtual		HRESULT		SetViewPort();
	virtual		HRESULT		DeleteDxResource();
	virtual		HRESULT		CreateDxResource();

public:
	TDevice(void);
	virtual ~TDevice(void);
};
