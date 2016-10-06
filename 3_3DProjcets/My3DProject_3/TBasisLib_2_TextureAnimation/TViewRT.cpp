#include "TViewRT.h"

bool TViewRT::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pImmediateContext,DWORD dwWidth, DWORD dwHeight )
{
	D3D11_TEXTURE2D_DESC dstex;
    dstex.Width			= dwWidth;
    dstex.Height		= dwHeight;
    dstex.MipLevels		= 1;
    dstex.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
    dstex.SampleDesc.Count		= 1;
    dstex.SampleDesc.Quality	= 0;
    dstex.Usage					= D3D11_USAGE_DEFAULT;
    dstex.BindFlags				= D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    dstex.CPUAccessFlags		= 0;
    dstex.MiscFlags				= 0;
    dstex.ArraySize				= 1;

	if( FAILED( pd3dDevice->CreateTexture2D( &dstex, NULL, &m_pTexture ) ))
	{
		return false;
	}
	if( FAILED( pd3dDevice->CreateShaderResourceView( m_pTexture, NULL, &m_pSRV ) ))
	{
		return false;
	}
    if( FAILED( pd3dDevice->CreateRenderTargetView( m_pTexture, NULL, &m_pRTV ) ))
	{
		return false;
	}

	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    pImmediateContext->ClearRenderTargetView( m_pRTV, ClearColor );
	return true;
}

TViewRT::TViewRT( ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pImmediateContext,D3D11_TEXTURE2D_DESC* pTexDesc )
{
    pTexDesc->BindFlags		= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    pTexDesc->Usage			= D3D11_USAGE_DEFAULT;
    pTexDesc->CPUAccessFlags = NULL;

    pd3dDevice->CreateTexture2D( pTexDesc, NULL, &m_pTexture );
    pd3dDevice->CreateShaderResourceView( m_pTexture, NULL, &m_pSRV );
    pd3dDevice->CreateRenderTargetView( m_pTexture, NULL, &m_pRTV );

	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    pImmediateContext->ClearRenderTargetView( m_pRTV, ClearColor );
}
TViewRT::TViewRT()
{
	m_pTexture		= NULL;
	m_pSRV			= NULL;
	m_pRTV			= NULL;
}

TViewRT::~TViewRT()
{
    m_pTexture->Release();
    m_pRTV->Release();
    m_pSRV->Release();
}
