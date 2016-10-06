#include "TDxState.h"
HRESULT TDxState::Create(ID3D11Device* pDevice)
{
	HRESULT hr= S_OK;
	memset(&m_RasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC) );
	m_RasterizerDesc.DepthClipEnable = TRUE;
	m_RasterizerDesc.CullMode = D3D11_CULL_BACK;
	m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;    
	V_RETURN( pDevice->CreateRasterizerState( 
		&m_RasterizerDesc, &m_StateBackCullSolid ) );
	
	m_RasterizerDesc.CullMode = D3D11_CULL_NONE;
	m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;     
	V_RETURN( pDevice->CreateRasterizerState( 
		&m_RasterizerDesc, &m_StateNoneCullSolid ) );	
	//--------------------------------------------------------------------------------------
	// ºí·£µå »óÅÂ °´Ã¼ »ý¼º 
	//--------------------------------------------------------------------------------------		
	CreateBlendState(pDevice);
	//--------------------------------------------------------------------------------------
	// ±íÀÌ ¹× ½ºÅÙ½Ç ¹öÆÛ »óÅÂ°´Ã¼ »ý¼º 
	//--------------------------------------------------------------------------------------		
	memset(&m_DepthStencilDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC)); 
	m_DepthStencilDesc.DepthEnable			= TRUE; 
	m_DepthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
	m_DepthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;
	if( FAILED( hr = pDevice->CreateDepthStencilState( 
		&m_DepthStencilDesc, &m_pDepthEnableLessDSV) ))
	{
		return hr;
	}

	return hr;
};
HRESULT TDxState::CreateBlendState(ID3D11Device* pDevice)
{
	HRESULT hr;
	if(m_pAlphaBlend) m_pAlphaBlend->Release();
	D3D11_BLEND_DESC BlendState;
    ZeroMemory( &BlendState, sizeof( D3D11_BLEND_DESC ) );
    BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].BlendOp		= D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend		= D3D11_BLEND_ZERO;
    BlendState.RenderTarget[0].DestBlend	= D3D11_BLEND_ONE;

	BlendState.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND_ZERO;

    BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    pDevice->CreateBlendState( &BlendState, &m_pSrcNoBlend );

	//D3D11_BLEND_DESC BlendState;
    ZeroMemory( &BlendState, sizeof( D3D11_BLEND_DESC ) );
    BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].BlendOp		= D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend		= D3D11_BLEND_SRC_ALPHA;
    BlendState.RenderTarget[0].DestBlend	= D3D11_BLEND_INV_SRC_ALPHA;
		
	BlendState.RenderTarget[0].BlendOpAlpha		= D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha	= D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha	= D3D11_BLEND_ZERO;

    BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    if( FAILED( hr = pDevice->CreateBlendState( &BlendState, &m_pAlphaBlend ) ))
	{
		return hr;
	}
	return hr;
}
void TDxState::ApplyAlphaBlend(ID3D11DeviceContext*   pImmediateContext, const FLOAT fBlendFactor[], UINT iMask )
{
	pImmediateContext->OMSetBlendState( m_pAlphaBlend, fBlendFactor, iMask );
}
void TDxState::ApplySrcNoBlend(ID3D11DeviceContext*   pImmediateContext, const FLOAT fBlendFactor[], UINT iMask )
{
	pImmediateContext->OMSetBlendState( m_pSrcNoBlend, fBlendFactor, iMask );
}
HRESULT TDxState::SetRasterizerState(ID3D11Device* pDevice, D3D11_CULL_MODE CullMode, D3D11_FILL_MODE FillMode )
{
	assert( pDevice );
	HRESULT hr =S_OK;
	if(m_pRasterizerState) m_pRasterizerState->Release();
	m_CullMode = CullMode;
	m_FillMode = FillMode;

	memset(&m_RasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC) );
	m_RasterizerDesc.DepthClipEnable = TRUE;
    m_RasterizerDesc.CullMode = m_CullMode;
	m_RasterizerDesc.FillMode = m_FillMode;    
   /* m_RasterizerDesc.FrontCounterClockwise = FALSE;
    m_RasterizerDesc.DepthBias = 0;
    m_RasterizerDesc.DepthBiasClamp = 0;
    m_RasterizerDesc.SlopeScaledDepthBias = 0;
    m_RasterizerDesc.ScissorEnable = FALSE;
    m_RasterizerDesc.MultisampleEnable = TRUE;
    m_RasterizerDesc.AntialiasedLineEnable = FALSE;*/
    V_RETURN( pDevice->CreateRasterizerState( &m_RasterizerDesc, &m_pRasterizerState ) );
	
	return hr;
}
HRESULT TDxState::SetRasterizerState(ID3D11Device* pDevice, D3D11_RASTERIZER_DESC  RSDesc )
{
	assert( pDevice );
	HRESULT hr =S_OK;
	if(m_pRasterizerState) m_pRasterizerState->Release();
	m_CullMode = RSDesc.CullMode;
	m_FillMode = RSDesc.FillMode;

    V_RETURN( pDevice->CreateRasterizerState( &RSDesc, &m_pRasterizerState ) );
	
	return hr;
}
HRESULT TDxState::SetCullMode( ID3D11Device* pDevice, D3D11_CULL_MODE CullMode )
{
	HRESULT hr;
	m_CullMode = CullMode; 	
	m_RasterizerDesc.CullMode = m_CullMode;	
	if( FAILED( hr = SetRasterizerState( pDevice, m_RasterizerDesc ) ))
	{
		return hr;
	}
	return hr;
}
HRESULT TDxState::SetFillMode(ID3D11Device* pDevice, D3D11_FILL_MODE FillMode)
{
	HRESULT hr;
	m_FillMode = FillMode;		
	m_RasterizerDesc.FillMode = m_FillMode; 
	if( FAILED( hr = SetRasterizerState( pDevice, m_RasterizerDesc ) ))
	{
		return hr;
	}
	return hr;
}

void TDxState::SetPrimitive( D3D11_PRIMITIVE_TOPOLOGY Primitive)
{
	m_PrimitiveType = Primitive; 	
}
void TDxState::SetPrimitive( int iPrimitive)
{
	m_PrimitiveType = (D3D11_PRIMITIVE_TOPOLOGY)iPrimitive;	
}
HRESULT TDxState::ApplyRasterizer(ID3D11Device* pDevice,ID3D11DeviceContext*    pImmediateContext)
{
	HRESULT hr = S_OK;
	assert( pDevice );

	if( m_pRasterizerState == NULL )
	{
		if( FAILED( hr = SetRasterizerState( pDevice, m_CullMode, m_FillMode ) ))
		{
			return hr;
		}
	}		
	pImmediateContext->RSSetState( m_pRasterizerState );
	pImmediateContext->IASetPrimitiveTopology( (D3D11_PRIMITIVE_TOPOLOGY)m_PrimitiveType ); 	
	return hr;
}
HRESULT TDxState::CreateDepthStencilState(	ID3D11Device* pDevice, 
											BOOL DepthEnable, 
											D3D11_COMPARISON_FUNC DepthFunc )
{
	assert(pDevice);
	if(m_pDepthStencilState) m_pDepthStencilState->Release();
	HRESULT hr;
	//--------------------------------------------------------------------------------------
	// ±íÀÌ ¹× ½ºÅÙ½Ç ¹öÆÛ »óÅÂ°´Ã¼ »ý¼º 
	//--------------------------------------------------------------------------------------		
	/*enum D3D11_COMPARISON_FUNC
    {	
		D3D11_COMPARISON_NEVER			= 1,
		D3D11_COMPARISON_LESS			= 2,
		D3D11_COMPARISON_EQUAL			= 3,
		D3D11_COMPARISON_LESS_EQUAL		= 4,
		D3D11_COMPARISON_GREATER		= 5,
		D3D11_COMPARISON_NOT_EQUAL		= 6,
		D3D10_COMPARISON_GREATER_EQUAL	= 7,
		D3D11_COMPARISON_ALWAYS			= 8
    }D3D11_COMPARISON_FUNC;*/
	memset(&m_DepthStencilDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC)); 
	m_DepthStencilDesc.DepthEnable			= DepthEnable; 
	m_DepthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
	m_DepthStencilDesc.DepthFunc			= DepthFunc;
	if( FAILED( hr = pDevice->CreateDepthStencilState( &m_DepthStencilDesc, &m_pDepthStencilState) ))
	{
		return hr;
	}
	return hr;
}
HRESULT TDxState::CreateDepthStencilState(	ID3D11Device* pDevice, 
											D3D11_DEPTH_STENCIL_DESC dsd )
{
	assert(pDevice);
	if(m_pDepthStencilState) m_pDepthStencilState->Release();
	HRESULT hr;
	m_DepthStencilDesc = dsd;
	//--------------------------------------------------------------------------------------
	// ±íÀÌ ¹× ½ºÅÙ½Ç ¹öÆÛ »óÅÂ°´Ã¼ »ý¼º 
	//--------------------------------------------------------------------------------------		
	if( FAILED( hr = pDevice->CreateDepthStencilState( &m_DepthStencilDesc, &m_pDepthStencilState) ))
	{
		return hr;
	}
	return hr;
}

HRESULT TDxState::ApplyDepthStencil(ID3D11Device* pDevice, ID3D11DeviceContext*   pImmediateContext, UINT iStencilRef)
{
	HRESULT hr = S_OK;
	assert( pDevice );

	if( m_pDepthStencilState == NULL )
	{
		return E_FAIL;
	}		
	pImmediateContext->OMSetDepthStencilState( m_pDepthStencilState, iStencilRef );	 	
	return hr;
}

bool TDxState::Release()
{
	SAFE_RELEASE(m_StateBackCullSolid);
	SAFE_RELEASE(m_StateNoneCullSolid);
	SAFE_RELEASE(m_pDepthEnableLessDSV);
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pSrcNoBlend);
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pDepthStencilState);	
	return true;
}
TDxState::TDxState(void)
{
	m_CullMode				= D3D11_CULL_BACK;
	m_FillMode				= D3D11_FILL_SOLID;
	m_PrimitiveType			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pRasterizerState		= NULL;	
	m_pDepthStencilState	= NULL;
	m_pAlphaBlend			= NULL;
	m_StateBackCullSolid	= NULL;
	m_StateNoneCullSolid	= NULL;
	m_pDepthEnableLessDSV	= NULL;
	m_pSrcNoBlend			= NULL;
	m_pAlphaBlend			= NULL;

	memset(&m_RasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC) );
	m_RasterizerDesc.DepthClipEnable = TRUE;
    m_RasterizerDesc.CullMode = m_CullMode;
	m_RasterizerDesc.FillMode = m_FillMode;    	
}

TDxState::~TDxState(void)
{
	Release();
}
