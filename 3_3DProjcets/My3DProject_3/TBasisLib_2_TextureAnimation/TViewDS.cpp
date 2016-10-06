#include "TViewDS.h"

TViewDS::TViewDS( ID3D11Device* pd3dDevice, D3D11_TEXTURE2D_DESC* pTexDesc )
{
    m_pTexture = NULL;
    m_pDSV     = NULL;
    m_pSRV     = NULL;

    HRESULT hr;
    pd3dDevice->CreateTexture2D( pTexDesc, NULL, &m_pTexture );
	assert( m_pTexture );
        
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	ZeroMemory( &srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    switch(pTexDesc->Format)
    {
        case DXGI_FORMAT_R32_TYPELESS:
            dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
            srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
            break;        
		case DXGI_FORMAT_R24G8_TYPELESS:
            dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            break;
    }  
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    pd3dDevice->CreateDepthStencilView( m_pTexture, &dsvDesc, &m_pDSV );
	assert( m_pDSV );
    
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
	if( pTexDesc->Format == DXGI_FORMAT_R32_TYPELESS)
    {       
        srvDesc.Texture2D.MipLevels = 1;
    }
	else
	{
        srvDesc.Texture2D.MipLevels = pTexDesc->MipLevels;		
	}
	pd3dDevice->CreateShaderResourceView( m_pTexture, &srvDesc, &m_pSRV );
	assert( m_pDSV );
}

TViewDS::~TViewDS()
{
    m_pTexture->Release();
    m_pDSV->Release();
    m_pSRV->Release();
}