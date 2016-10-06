#pragma once
#include "TBasisStd.h"

class TDxState
{
public:
	//--------------------------------------------------------------------------------------
	// ���� ����
	//--------------------------------------------------------------------------------------	
	D3D11_PRIMITIVE_TOPOLOGY	m_PrimitiveType;
	D3D11_RASTERIZER_DESC		m_RasterizerDesc;
	D3D11_CULL_MODE				m_CullMode;
	D3D11_FILL_MODE				m_FillMode;
	D3D11_DEPTH_STENCIL_DESC	m_DepthStencilDesc; 

	// ���� ���� ��ü
	ID3D11RasterizerState*      m_pRasterizerState;
	ID3D11DepthStencilState*	m_pDepthStencilState;

	// ���� ���� ��ü
	ID3D11RasterizerState* 		m_StateBackCullSolid;
	ID3D11RasterizerState* 		m_StateNoneCullSolid;
	ID3D11DepthStencilState*	m_pDepthEnableLessDSV;
	ID3D11BlendState*           m_pSrcNoBlend;
	ID3D11BlendState*			m_pAlphaBlend;


	HRESULT Create(ID3D11Device* pDevice);
public:
	//--------------------------------------------------------------------------------------
	// ����Ʈ������ ���� ����
	//--------------------------------------------------------------------------------------
	HRESULT		SetCullMode( ID3D11Device* pDevice, D3D11_CULL_MODE CullMode );
	HRESULT		SetFillMode( ID3D11Device* pDevice, D3D11_FILL_MODE FillMode );
	//--------------------------------------------------------------------------------------
	// �����Ͷ����� ���°� ����
	//--------------------------------------------------------------------------------------
	HRESULT		SetRasterizerState( ID3D11Device* pDevice, 
									D3D11_CULL_MODE CullMode	= D3D11_CULL_FRONT, 
									D3D11_FILL_MODE d3dFillMode	= D3D11_FILL_SOLID );
	HRESULT		SetRasterizerState(ID3D11Device* pDevice, D3D11_RASTERIZER_DESC  RSDesc );
	//--------------------------------------------------------------------------------------
	// ������Ƽ�� ���� ����
	//--------------------------------------------------------------------------------------
	void		SetPrimitive( D3D11_PRIMITIVE_TOPOLOGY Primitive);
	void		SetPrimitive( int iPrimitive);
	//--------------------------------------------------------------------------------------
	// �����Ͷ����� ���°� ����
	//--------------------------------------------------------------------------------------
	HRESULT		ApplyRasterizer(ID3D11Device* pDevice,ID3D11DeviceContext*   pImmediateContext);
	//--------------------------------------------------------------------------------------
	// ���� �� ���ٽ� ���� ���� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateDepthStencilState(	ID3D11Device* pDevice, 
											BOOL DepthEnable   = TRUE, 
											D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS );
	HRESULT		CreateDepthStencilState(	ID3D11Device* pDevice, 
											D3D11_DEPTH_STENCIL_DESC dsd );
	HRESULT		ApplyDepthStencil(			ID3D11Device* pDevice, 
											ID3D11DeviceContext*   pImmediateContext,
											UINT iStencilRef = 0x00);
	//--------------------------------------------------------------------------------------
	// ���� ����
	//--------------------------------------------------------------------------------------

	HRESULT						CreateBlendState(ID3D11Device* pDevice);
	void						ApplyAlphaBlend(ID3D11DeviceContext*   pImmediateContext, const FLOAT fBlendFactor[] = 0, UINT iMask = 0xffffffff );
	void						ApplySrcNoBlend(ID3D11DeviceContext*   pImmediateContext, const FLOAT fBlendFactor[] = 0, UINT iMask = 0xffffffff );
	//--------------------------------------------------------------------------------------
	// ���� ���� ����
	//--------------------------------------------------------------------------------------
	bool		Release();
public:
	TDxState(void);
	virtual ~TDxState(void);
};
static void ApplyRS(	ID3D11DeviceContext*   pImmediateContext, 
						ID3D11RasterizerState* pState )
{
	assert( pImmediateContext );
	pImmediateContext->RSSetState( pState );
}
static void ApplyDSS(	ID3D11DeviceContext*   pImmediateContext, 
							ID3D11DepthStencilState*	pDepthStencilState,
							UINT iRef = 0x01)
{
	assert( pImmediateContext );
	pImmediateContext->OMSetDepthStencilState( pDepthStencilState, iRef );	 	

};
static void ApplyBS(ID3D11DeviceContext*   pImmediateContext,
					ID3D11BlendState*      pBlendState,
					const FLOAT fBlendFactor[] = 0, 
					UINT iMask = 0xffffffff )
{
	assert( pImmediateContext );
	pImmediateContext->OMSetBlendState( pBlendState, fBlendFactor, iMask );
}