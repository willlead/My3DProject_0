#include "TMiniMap.h"

bool TMiniMap::Create(ID3D11Device* pd3dDevice,ID3D11DeviceContext*    pImmediateContext )
{
	SAFE_NEW( m_pPlaneShape, TPlaneShape );
	if( FAILED( m_pPlaneShape->Create(pd3dDevice,pImmediateContext) ) )
	{
		MessageBox( 0, _T("m_LineShape 실패"), _T("Fatal error"), MB_OK );
		return 0;
	}

	D3DXVECTOR3 vUpVector( 0.0f, 1.0f, 0.0f );	
	SetViewMatrix( D3DXVECTOR3( 30.0f, 70.0f, -10.0f ), D3DXVECTOR3( 0.0f, 0.0f, 1.0f ), vUpVector );
	SetProjMatrix(	D3DX_PI * 0.25f,
		(float)m_TextureVP.m_vp.Width  / (float)m_TextureVP.m_vp.Height, 
								0.1f, 
								5000.0f);	
	return true;
}
bool TMiniMap::BeginRender(ID3D11DeviceContext*    pImmediateContext)
{
	pImmediateContext->IASetPrimitiveTopology( (D3D11_PRIMITIVE_TOPOLOGY)m_PrimitiveType ); 

	//-----------------------------------------------------------------------
	// 현재 세팅된 뷰포트 정보 저장
	//-----------------------------------------------------------------------
    pImmediateContext->OMGetRenderTargets( 1, &m_pOldRT, &m_pOldDS );
	pImmediateContext->RSGetViewports( &m_nViewPorts, m_vpOld );	

	pImmediateContext->OMSetRenderTargets( 1,&m_pMiniMapRT->m_pRTV, m_pMiniMapDS->m_pDSV );	
	///-----------------------------------------------------------------------
	// 미니맵  뷰포트
	//-----------------------------------------------------------------------	
	m_TextureVP.Apply( pImmediateContext , m_pMiniMapDS->m_pDSV );

	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	pImmediateContext->ClearRenderTargetView( m_pMiniMapRT->m_pRTV, ClearColor );
	return true;
}
bool TMiniMap::EndRender(ID3D11DeviceContext*    pImmediateContext)
{
	//-----------------------------------------------------------------------
	// 기본 뷰포트 정보로 복원
	//-----------------------------------------------------------------------	    
	pImmediateContext->OMSetRenderTargets( 1, &m_pOldRT, m_pOldDS );
	pImmediateContext->RSSetViewports(m_nViewPorts, m_vpOld);
	SAFE_RELEASE( m_pOldDS );
    SAFE_RELEASE( m_pOldRT );  
	return true;
}
bool TMiniMap::Frame()
{
	return true;
}
bool TMiniMap::Render(ID3D11DeviceContext*    pImmediateContext)
{
	pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 0x00 );
	pImmediateContext->OMSetBlendState( m_pAlphaBlend, 0, 0xffffffff );	

	m_pPlaneShape->m_Texture.m_pTextureRV = m_pMiniMapRT->m_pSRV;
	m_pPlaneShape->m_pTextureVariable->SetResource( m_pMiniMapRT->m_pSRV ); 
	m_pPlaneShape->SetMatrix( NULL,  NULL, NULL );
	m_pPlaneShape->Render();
	return true;
}
HRESULT TMiniMap::CreateResource(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pImmediateContext, UINT iWidth, UINT iHeight)
{	
	HRESULT hr = S_OK;	
	UINT iRectWidth  = iWidth/3;
	UINT iRectHeight = iHeight/3;
	m_TextureVP.Set(	0, 0,	iRectWidth, iRectHeight, 0.0f, 1.0f);	

	D3D11_DEPTH_STENCIL_DESC dsd; 
	memset(&dsd, 0, sizeof(dsd)); 
	dsd.DepthEnable = FALSE; 	
	dsd.StencilEnable = FALSE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	pd3dDevice->CreateDepthStencilState( &dsd, &m_pDepthStencilState); 

	D3D11_BLEND_DESC BlendState;
   ZeroMemory( &BlendState, sizeof( D3D11_BLEND_DESC ) );
    BlendState.RenderTarget[0].BlendEnable = FALSE;
	BlendState.RenderTarget[0].BlendOp		= D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
    BlendState.RenderTarget[0].DestBlend	= D3D11_BLEND_INV_SRC_ALPHA;

	BlendState.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND_ZERO;

    BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    pd3dDevice->CreateBlendState( &BlendState, &m_pAlphaBlend );
	

	SAFE_DEL( m_pMiniMapRT );
	SAFE_DEL( m_pMiniMapDS );

	D3D11_TEXTURE2D_DESC dstex;
    dstex.Width			= iRectWidth;
    dstex.Height		= iRectHeight;
    dstex.MipLevels		= 1;
    dstex.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
    dstex.SampleDesc.Count		= 1;
    dstex.SampleDesc.Quality	= 0;
    dstex.Usage					= D3D11_USAGE_DEFAULT;
    dstex.BindFlags				= D3D10_BIND_RENDER_TARGET|D3D10_BIND_SHADER_RESOURCE;
    dstex.CPUAccessFlags		= 0;
    dstex.MiscFlags				= 0;
    dstex.ArraySize				= 1;
	m_pMiniMapRT = new TViewRT( pd3dDevice, pImmediateContext,&dstex);


	D3D11_TEXTURE2D_DESC texDesc;
    texDesc.ArraySize          = 1;
    texDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;;
    texDesc.CPUAccessFlags     = NULL;
    texDesc.Format             = DXGI_FORMAT_R32_TYPELESS;
    texDesc.Width              = iRectWidth;
    texDesc.Height             = iRectHeight;
    texDesc.MipLevels          = 1;
    texDesc.MiscFlags          = NULL;
    texDesc.SampleDesc.Count   = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage              = D3D11_USAGE_DEFAULT;

	m_pMiniMapDS = new TViewDS( pd3dDevice, &texDesc);

	return hr;
}
HRESULT TMiniMap::DeleteResource()
{
	HRESULT hr= S_OK;	
	m_TextureVP.Release();
	return S_OK;
}

bool TMiniMap::Release()
{
	SAFE_RELEASE( m_pAlphaBlend );
	SAFE_RELEASE( m_pDepthStencilState );
	SAFE_DEL( m_pPlaneShape  );
	SAFE_DEL( m_pMiniMapRT );
	SAFE_DEL( m_pMiniMapDS );;
	return true;
}
TMiniMap::TMiniMap(void)
{
	m_pOldRT = NULL;
    m_pOldDS = NULL;
	m_nViewPorts = 1;
	m_pAlphaBlend = 0;
	m_pDepthStencilState = 0;
	m_pPlaneShape  =0;
	m_pMiniMapRT =0;
	m_pMiniMapDS =0;
	m_PrimitiveType	=	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

TMiniMap::~TMiniMap(void)
{
}
