#include "TViewPort.h"

HRESULT TViewPort::Set( /*ID3D11Device* pDevice,
						ID3D11RenderTargetView* pRTV,*/
						INT TopLeftX,   
						INT TopLeftY,   
						UINT Width,   
						UINT Height,   
						FLOAT MinDepth,  
						FLOAT MaxDepth	)
{	
	HRESULT hr = S_OK;	
	
	m_vp.TopLeftX = TopLeftX; m_vp.TopLeftY = TopLeftY;
	m_vp.Width    = Width;	m_vp.Height   = Height;
	m_vp.MinDepth = MinDepth;	m_vp.MaxDepth = MaxDepth;
	
	D3DXMatrixPerspectiveFovLH( &m_matProj, 
								( float )D3DX_PI * 0.25f, 
								Width  / (float)Height, 								
								0.1f, 1000.0f );

	//// 백버퍼의 크기가 변경되면 반드시 깊이스텐실 뷰어도 다시 작성되어야 한다.		
	//if( FAILED( hr = CreateDepthStencilView( Width, Height, pDevice, pRTV) ) )
	//{
	//	MessageBox( 0, _T("LoadEffectFile 실패"), _T("Fatal error"), MB_OK );
	//	return hr;
	//}
	return hr;
}
HRESULT TViewPort::CreateDepthStencilView(	UINT iWidth, UINT iHeight, 
											ID3D11Device* pDevice,
											ID3D11DeviceContext*    pImmediateContext,
											ID3D11RenderTargetView* pRTV )
{
	if( m_pDepthStencilView != NULL )
	{
		SAFE_RELEASE(m_pDepthStencilView);
	}
	ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
    pImmediateContext->PSSetShaderResources( 0, 1, ppSRVNULL );
	pImmediateContext->RSSetState( NULL );

	ID3D11RenderTargetView* pNullRTV = NULL;
	pImmediateContext->OMSetRenderTargets( 1, &pNullRTV, NULL );
	//--------------------------------------------------------------------------------------
	// 깊이 및 스텐실 뷰 리소스 생성 
	//--------------------------------------------------------------------------------------
	HRESULT hr;
	// Create Texture2D
	ID3D11Texture2D* pDepthStencilTexture = NULL;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width				= iWidth;
	DescDepth.Height			= iHeight;
	DescDepth.MipLevels			= 1;
	DescDepth.ArraySize			= 1;
	DescDepth.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count	= 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage				= D3D11_USAGE_DEFAULT;
	DescDepth.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	DescDepth.CPUAccessFlags	= 0;
	DescDepth.MiscFlags			= 0;
	pDevice->CreateTexture2D( &DescDepth, NULL, &pDepthStencilTexture );

	//
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format					= DescDepth.Format;
	dsvd.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice		= 0;

	hr = pDevice->CreateDepthStencilView( pDepthStencilTexture, &dsvd, &m_pDepthStencilView );

	
	return hr;
}
void TViewPort::Apply(ID3D11DeviceContext*    pImmediateContext, ID3D11RenderTargetView* pRTV, bool bDepthClear, bool bStencilClear )
{
	assert(pImmediateContext);		
	if( pRTV != NULL )
	{
		pImmediateContext->OMSetRenderTargets( 1, &pRTV, m_pDepthStencilView );	
	}
	if( bDepthClear )
	{
		pImmediateContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0 );	
	}
	if( bStencilClear )
	{
		pImmediateContext->ClearDepthStencilView( m_pDepthStencilView, D3D10_CLEAR_STENCIL, 1.0, 0 );	
	}
	pImmediateContext->RSSetViewports(1, &m_vp);	
}
void TViewPort::Apply(ID3D11DeviceContext*    pImmediateContext, ID3D11DepthStencilView* pDepthStencilView, bool bDepthClear, bool bStencilClear )
{
	assert(pImmediateContext);		
	if( bDepthClear )
	{
		pImmediateContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0 );	
	}
	if( bStencilClear )
	{
		pImmediateContext->ClearDepthStencilView( pDepthStencilView, D3D10_CLEAR_STENCIL, 1.0, 0 );	
	}
	pImmediateContext->RSSetViewports(1, &m_vp);	
}
void TViewPort::Bind(ID3D11DeviceContext*    pImmediateContext, ID3D11RenderTargetView* pRTV)
{
	assert(pRTV);	
	ID3D11RenderTargetView* pNullRTV = NULL;
	pImmediateContext->OMSetRenderTargets( 1, &pNullRTV, NULL );
	pImmediateContext->OMSetRenderTargets( 1, &pRTV, m_pDepthStencilView );
}
bool TViewPort::Release()
{
	SAFE_RELEASE( m_pDepthStencilView );
	return true;
}
TViewPort::TViewPort(void)
{
	m_pDepthStencilView	=	NULL;
}

TViewPort::~TViewPort(void)
{
	Release();
}
