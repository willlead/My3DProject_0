#include "TRenderTexture.h"
#ifndef SAFE_DEL
	#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

bool TRenderTexture::Begin(ID3D11DeviceContext*    pImmediateContext,
						   D3DXVECTOR4 vClearColor,
						   bool bColorWrite)
{
	if( pImmediateContext == NULL ) return false;
	//Store the old viewport
    UINT cRT = 1;
    pImmediateContext->RSGetViewports( &cRT, &m_OldVP );
    pImmediateContext->OMGetRenderTargets( 1, &m_pOldRT, &m_pOldDS );    
    pImmediateContext->RSSetViewports( 1, &m_ApplyVP );

	ID3D11RenderTargetView* 	pRTV[1] =  { 0 };
	if( m_pViewRT && bColorWrite )
	{
		pImmediateContext->OMSetRenderTargets( 1, &m_pViewRT->m_pRTV, m_pViewDS->m_pDSV );
		pImmediateContext->ClearRenderTargetView( m_pViewRT->m_pRTV, (float*)vClearColor );
	}
	else
	{
		pImmediateContext->OMSetRenderTargets( 1, pRTV, m_pViewDS->m_pDSV );
	}	
	pImmediateContext->ClearDepthStencilView( m_pViewDS->m_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );	
	return true;
}
void TRenderTexture::End(ID3D11DeviceContext*    pImmediateContext)
{
	ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
    pImmediateContext->PSSetShaderResources( 0, 1, ppSRVNULL );
	pImmediateContext->RSSetState( NULL );

	ID3D11RenderTargetView* pNullRTV = NULL;
	pImmediateContext->OMSetRenderTargets( 1, &pNullRTV, NULL );

	// Restore the original viewport
    pImmediateContext->RSSetViewports( 1, &m_OldVP );
    // Restore rendertarget
    pImmediateContext->OMSetRenderTargets( 1, &m_pOldRT, m_pOldDS );	

	SAFE_RELEASE( m_pOldRT );
    SAFE_RELEASE( m_pOldDS );
}
bool TRenderTexture::Create(ID3D11Device* pd3dDevice, 
							ID3D11DeviceContext*    pImmediateContext, 
							UINT iSize,							
							bool bColorWrite,
							DXGI_FORMAT format)
{	
	m_ApplyVP.Width	= iSize;
	m_ApplyVP.Height	= iSize;
    m_ApplyVP.MinDepth = 0;
    m_ApplyVP.MaxDepth = 1;
    m_ApplyVP.TopLeftX = 0;
    m_ApplyVP.TopLeftY = 0;

	if( bColorWrite )
	{
		// Create Source and Dest textures
		m_rtTexDesc.Width		= iSize;
		m_rtTexDesc.Height		= iSize;
		m_rtTexDesc.MipLevels	= 1;
		m_rtTexDesc.Format		= format;
		m_rtTexDesc.SampleDesc.Count	= 1;
		m_rtTexDesc.SampleDesc.Quality	= 0;
		m_rtTexDesc.Usage				= D3D11_USAGE_DEFAULT;
		m_rtTexDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		m_rtTexDesc.CPUAccessFlags		= 0;
		m_rtTexDesc.MiscFlags			= 0;
		m_rtTexDesc.ArraySize			= 1;
		m_pViewRT = new TViewRT( pd3dDevice,pImmediateContext, &m_rtTexDesc);
	}

    m_dsTexDesc.ArraySize          = 1;
    m_dsTexDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    m_dsTexDesc.CPUAccessFlags     = NULL;
    m_dsTexDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS;
    m_dsTexDesc.Width              = iSize;
    m_dsTexDesc.Height             = iSize;
    m_dsTexDesc.MipLevels          = 1;
    m_dsTexDesc.MiscFlags          = NULL;
    m_dsTexDesc.SampleDesc.Count   = 1;
    m_dsTexDesc.SampleDesc.Quality = 0;
    m_dsTexDesc.Usage              = D3D11_USAGE_DEFAULT;

	m_pViewDS = new TViewDS( pd3dDevice, &m_dsTexDesc);
	return true;
}
bool TRenderTexture::Release()
{
	SAFE_DEL( m_pViewRT );
	SAFE_DEL( m_pViewDS );
	return true;
}
TRenderTexture::TRenderTexture(void)
{
	m_pViewRT = NULL;
	m_pViewDS = NULL;
	m_pOldRT  = NULL;
    m_pOldDS  = NULL;
}


TRenderTexture::~TRenderTexture(void)
{
}
