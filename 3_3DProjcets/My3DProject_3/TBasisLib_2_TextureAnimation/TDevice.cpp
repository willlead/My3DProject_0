#include "TDevice.h"

ID3D11Device*           g_pd3dDevice;		// ����̽� ��ü
IDXGISwapChain*         g_pSwapChain;		// ����ü�� ��ü
ID3D11RenderTargetView* g_pRenderTargetView;// ���� ����Ÿ�� ��	
IDXGIFactory*			g_pGIFactory;		// DXGI ��ü
ID3D11DeviceContext*    g_pImmediateContext;
D3D_FEATURE_LEVEL       g_FeatureLevel;

HRESULT TDevice::InitDevice( HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen )
{	
	HRESULT hr = S_OK;	
	if( FAILED( hr = CreateDevice() ))
	{
		MessageBox( 0, _T("CreateDevice  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}	
	if( FAILED( hr = CreateSwapChain(hWnd,iWidth, iHeight, IsFullScreen) ) )
	{
		MessageBox( 0, _T("CreateSwapChain  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}	
	if( FAILED( hr = SetRenderTargetView() ) )
	{
		MessageBox( 0, _T("SetRenderTargetView  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}
	if( FAILED( hr = SetViewPort() ) )
	{
		MessageBox( 0, _T("SetViewPort  ����"), _T("Fatal error"), MB_OK );
		return hr;
	}	
	// ���� �� ���ҽ� ����
	if( FAILED( hr = CreateDxResource() ))
	{
		return hr;
	}
	// Alt + Enter Ű�� ���´�.
	if( FAILED( hr = GetGIFactory()->MakeWindowAssociation( hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER  )))
	{
		return hr;
	}

	g_pd3dDevice	= m_pd3dDevice;		// ����̽� ��ü
	g_pSwapChain	= m_pSwapChain;			// ����ü�� ��ü
	g_pRenderTargetView= m_pRenderTargetView;	// ���� ����Ÿ�� ��	
	g_pGIFactory	= m_pGIFactory;			// DXGI ��ü
	g_pImmediateContext= m_pImmediateContext;	
	
	return hr;
}

BOOL TDevice::GetFullScreenFlag()
{
	return m_IsFullScreenMode;
}
void TDevice::SetFullScreenFlag( BOOL bFlag )
{
	m_IsFullScreenMode = bFlag;
}
// New
HRESULT TDevice::ResizeDevice( UINT iWidth, UINT iHeight)
{
	if( m_pd3dDevice == NULL ) return true;
	HRESULT hr;
	// ���ε��Ǿ� �ִ� ���� �丮�ҽ��� �����Ѵ�.
	if( FAILED( hr= DeleteDxResource()))
	{
		return hr;
	}
	//--------------------------------------------------------------------------------------
	// ����Ÿ�ϰ� ���̽��ٽ� ���۸� �����Ѵ�.
	//--------------------------------------------------------------------------------------
	m_pImmediateContext->OMSetRenderTargets( 0, NULL, NULL );
	if( m_pRenderTargetView ) m_pRenderTargetView->Release();

	//--------------------------------------------------------------------------------------
	// ������� ũ�⸦ �����Ѵ�.
	//--------------------------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC CurrentSD;
	m_pSwapChain->GetDesc( &CurrentSD );
	 UINT Flags = 0;
    if( !m_IsFullScreenMode )
        Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	V( m_pSwapChain->ResizeBuffers( CurrentSD.BufferCount,
		iWidth, iHeight, CurrentSD.BufferDesc.Format, 
		Flags ));


	// ����Ÿ�Ϻ� ���� �� �����Ѵ�.
    if( FAILED( hr = SetRenderTargetView() ) ) 
	{
		return hr;
	}
	// ����Ʈ�� �����ϰ� �����Ѵ�.
	if( FAILED( hr = SetViewPort() ) )
	{
		return hr;
	}
	if( FAILED( hr = CreateDxResource() ) )
	{
		return hr;
	}
	return S_OK;
}
ID3D11Device* TDevice::GetDevice()
{
	assert( m_pd3dDevice );
	return m_pd3dDevice; 
}
ID3D11DeviceContext* TDevice::GetContext()
{
	assert( m_pImmediateContext );
	return m_pImmediateContext; 
}
IDXGISwapChain* TDevice::GetSwapChain()
{ 
	assert( m_pSwapChain );
	return m_pSwapChain; 
}
ID3D11RenderTargetView* TDevice::GetRenderTargetView()	
{ 
	assert( m_pRenderTargetView );
	return m_pRenderTargetView; 
}
IDXGIFactory* TDevice::GetGIFactory()
{
	assert(m_pGIFactory);
	return m_pGIFactory;
}

//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��κ��� IDXGISwapChain �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateSwapChain( HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{	
	HRESULT hr = S_OK;
	SetFullScreenFlag( IsFullScreen );
	if( m_pGIFactory == NULL ) return S_FALSE;
	DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width		= iWidth;
    sd.BufferDesc.Height	= iHeight;
    sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
     sd.Windowed = !m_IsFullScreenMode;
	// �߰�
	sd.Flags	= 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if( FAILED( hr = m_pGIFactory->CreateSwapChain( m_pd3dDevice, &sd, &m_pSwapChain ) ))
	{
		return hr;
	}	
	return hr;
}
//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��κ��� IDXGISwapChain �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateDeviceAndSwapChain(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{	
	HRESULT hr = S_OK;

    UINT createDeviceFlags =  D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount				= 1;
    sd.BufferDesc.Width			= iWidth;
    sd.BufferDesc.Height		= iHeight;
    sd.BufferDesc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage				= DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow				= hWnd;
    sd.SampleDesc.Count			= 1;
    sd.SampleDesc.Quality		= 0;
    sd.Windowed = !m_IsFullScreenMode;
	// �߰�
	sd.Flags	= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags,featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext );
        if( SUCCEEDED( hr ) )
		{
            break;
		}
    }
    if( FAILED( hr ) )
	{
        return hr;
	}
	g_FeatureLevel = m_FeatureLevel;
	return hr;
}

//--------------------------------------------------------------------------------------
// ID3D11Device �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateDevice()
{
	HRESULT hr;
	  UINT createDeviceFlags =  D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	IDXGIAdapter* pAdapter = NULL;
	IDXGIFactory* pGIFactory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pGIFactory) );
	hr = pGIFactory->EnumAdapters( 0, &pAdapter );
    if ( FAILED( hr) ) 
    {
         return false;
    }

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
		m_driverType = D3D_DRIVER_TYPE_UNKNOWN;    
		if(driverTypeIndex != 0 ) 
		{
			m_driverType = driverTypes[driverTypeIndex];
			pAdapter = NULL;
		}
        
        hr = D3D11CreateDevice(	pAdapter, m_driverType, NULL, createDeviceFlags,featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext );
		// D3D_FEATURE_LEVEL_11_0 �� ������ ���̴�.
		if( FAILED( hr ) || m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
        {
            SAFE_RELEASE( m_pd3dDevice );
            SAFE_RELEASE( m_pImmediateContext );
            continue;
        }
        if( SUCCEEDED( hr ) ) break;
    }
    if( FAILED( hr ) )       return hr;
	g_FeatureLevel = m_FeatureLevel;

	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pGIFactory);
	//D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS ho;
 //   pd3dDevice->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &ho, sizeof(ho));
 //   pDeviceInfo->ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x = ho.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x; 
 //   SAFE_RELEASE( pd3dDeviceContext );             
 //   SAFE_RELEASE( pd3dDevice );

	return CreateGIFactory();
}
//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------
HRESULT TDevice::CreateGIFactory()
{	
	if(m_pd3dDevice==NULL) return E_FAIL;
	HRESULT hr;// = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory) );
	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
      
	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	IDXGIFactory * pIDXGIFactory;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);
	
	pDXGIDevice->Release();
	pDXGIAdapter->Release();

	return S_OK;
}
//--------------------------------------------------------------------------------------
// DXGIFactory �������̽��κ��� IDXGISwapChain �������̽��� �����Ѵ�.
//--------------------------------------------------------------------------------------

HRESULT TDevice::SetRenderTargetView()
{
	HRESULT hr = S_OK;
	 // Create a render target view
    ID3D11Texture2D* pBackBuffer;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;
	
	hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;    

	if( FAILED( hr = GetSwapChain()->GetDesc(&m_SwapChainDesc) ))
	{
		return hr;
	}

	if( FAILED( hr = CreateDepthStencilView( m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height ) ))
	{
		return hr;
	}

	if( FAILED( hr = CreateDepthStencilState() ))
	{
		return hr;
	}	

	m_StateFrontCullSolid.SetRasterizerState( GetDevice(), D3D11_CULL_FRONT, D3D11_FILL_SOLID );
	m_StateBackCullSolid.SetRasterizerState( GetDevice(), D3D11_CULL_BACK, D3D11_FILL_SOLID );
	m_StateNoneCullSolid.SetRasterizerState( GetDevice(), D3D11_CULL_NONE, D3D11_FILL_SOLID );

	return S_OK;
}
HRESULT TDevice::CreateDepthStencilView( UINT iWidth, UINT iHeight )
{	
	if( !GetDevice()) return S_OK;
	//--------------------------------------------------------------------------------------
	// ���� �� ���ٽ� �� ���ҽ� ���� 
	//--------------------------------------------------------------------------------------
	HRESULT hr;
	// Create Texture2D
	ID3D11Texture2D* pDepthStencilTexture = NULL;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width			= iWidth;
	DescDepth.Height		= iHeight;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	GetDevice()->CreateTexture2D( &DescDepth, NULL, &pDepthStencilTexture );

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format = DescDepth.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	if( FAILED( hr = GetDevice()->CreateDepthStencilView( pDepthStencilTexture, &dsvd, &m_pDepthStencilView )))
	{
		return hr;
	}
	SAFE_RELEASE( pDepthStencilTexture);

	return hr;
}

HRESULT TDevice::CreateDepthStencilState()
{
	HRESULT hr;
	if( FAILED( hr = m_StateDepthStencil.CreateDepthStencilState( GetDevice(), TRUE, D3D11_COMPARISON_LESS ) ))
	{
		return hr;
	}
	return hr;
}
HRESULT TDevice::SetViewPort()
{
	HRESULT hr = S_OK;
	// Setup the viewport    
    m_MainVP.Width	= m_SwapChainDesc.BufferDesc.Width;
    m_MainVP.Height	= m_SwapChainDesc.BufferDesc.Height;
    m_MainVP.MinDepth = 0.0f;
    m_MainVP.MaxDepth = 1.0f;
    m_MainVP.TopLeftX = 0;
    m_MainVP.TopLeftY = 0;	
	m_pImmediateContext->RSSetViewports( 1, &m_MainVP );
	return S_OK;
}
void TDevice::ClearD3D11DeviceContext( ID3D11DeviceContext* pd3dDeviceContext )
{
    // Unbind all objects from the immediate context
    if (pd3dDeviceContext == NULL) return;

    ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    ID3D11DepthStencilView* pDSV = NULL;
    ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    // Shaders
    pd3dDeviceContext->VSSetShader( NULL, NULL, 0 );
    pd3dDeviceContext->HSSetShader( NULL, NULL, 0 );
    pd3dDeviceContext->DSSetShader( NULL, NULL, 0 );
    pd3dDeviceContext->GSSetShader( NULL, NULL, 0 );
    pd3dDeviceContext->PSSetShader( NULL, NULL, 0 );

    // IA clear
    pd3dDeviceContext->IASetVertexBuffers( 0, 16, pBuffers, StrideOffset, StrideOffset );
    pd3dDeviceContext->IASetIndexBuffer( NULL, DXGI_FORMAT_R16_UINT, 0 );
    pd3dDeviceContext->IASetInputLayout( NULL );

    // Constant buffers
    pd3dDeviceContext->VSSetConstantBuffers( 0, 14, pBuffers );
    pd3dDeviceContext->HSSetConstantBuffers( 0, 14, pBuffers );
    pd3dDeviceContext->DSSetConstantBuffers( 0, 14, pBuffers );
    pd3dDeviceContext->GSSetConstantBuffers( 0, 14, pBuffers );
    pd3dDeviceContext->PSSetConstantBuffers( 0, 14, pBuffers );

    // Resources
    pd3dDeviceContext->VSSetShaderResources( 0, 16, pSRVs );
    pd3dDeviceContext->HSSetShaderResources( 0, 16, pSRVs );
    pd3dDeviceContext->DSSetShaderResources( 0, 16, pSRVs );
    pd3dDeviceContext->GSSetShaderResources( 0, 16, pSRVs );
    pd3dDeviceContext->PSSetShaderResources( 0, 16, pSRVs );

    // Samplers
    pd3dDeviceContext->VSSetSamplers( 0, 16, pSamplers );
    pd3dDeviceContext->HSSetSamplers( 0, 16, pSamplers );
    pd3dDeviceContext->DSSetSamplers( 0, 16, pSamplers );
    pd3dDeviceContext->GSSetSamplers( 0, 16, pSamplers );
    pd3dDeviceContext->PSSetSamplers( 0, 16, pSamplers );

    // Render targets
    pd3dDeviceContext->OMSetRenderTargets( 8, pRTVs, pDSV );

    // States
    FLOAT blendFactor[4] = { 0,0,0,0 };
    pd3dDeviceContext->OMSetBlendState( NULL, blendFactor, 0xFFFFFFFF );
    pd3dDeviceContext->OMSetDepthStencilState( NULL, 0 );
    pd3dDeviceContext->RSSetState( NULL );
}

bool TDevice::CleanupDevice()
{
	ClearD3D11DeviceContext(m_pImmediateContext);
	// ���ε��Ǿ� �ִ� ���� �丮�ҽ��� �����Ѵ�.
	if( FAILED( DeleteDxResource()))
	{
		return false;
	}
    // Clear state and flush
    if( m_pImmediateContext )    m_pImmediateContext->ClearState();
    if( m_pImmediateContext )    m_pImmediateContext->Flush();
	
	m_StateDepthStencil.Release();
	m_StateBackCullSolid.Release();
	m_StateNoneCullSolid.Release();

	if( m_pRenderTargetView ) m_pDepthStencilView->Release();
    if( m_pRenderTargetView ) m_pRenderTargetView->Release();
    if( m_pSwapChain ) m_pSwapChain->Release();
	if( m_pImmediateContext ) m_pImmediateContext->Release();
    if( m_pd3dDevice ) m_pd3dDevice->Release();
	if( m_pGIFactory ) m_pGIFactory->Release();
	m_pd3dDevice		= NULL;
	m_pSwapChain		= NULL;
	m_pRenderTargetView = NULL;
	m_pImmediateContext = NULL;
	m_pGIFactory		= NULL;
	return true;
}
HRESULT TDevice::CreateDxResource()
{
	return S_OK;
}
HRESULT TDevice::DeleteDxResource()
{
	return S_OK;
}
TDevice::TDevice(void)
{
	m_driverType		= D3D_DRIVER_TYPE_NULL;
	m_FeatureLevel		= D3D_FEATURE_LEVEL_11_0;
	m_pd3dDevice		= NULL;
	m_pSwapChain		= NULL;
	m_pRenderTargetView = NULL;
	m_pImmediateContext = NULL;
	m_pGIFactory		= NULL;
}

TDevice::~TDevice(void)
{
}
