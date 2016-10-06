#include "TInputLayout.h"

HRESULT TInputLayout::Create(		ID3D11Device* pDevice,
									D3D11_INPUT_ELEMENT_DESC* pLayout, 
									UINT numElements,
									ID3DX11EffectTechnique**  ppSetTechnique, 
									ID3D11InputLayout** ppGetVertexLayout  )
{
	HRESULT hr;
	assert( pLayout || numElements > 0 );
	assert( *ppSetTechnique );

	ID3DX11EffectTechnique**  ppTechnique =  ppSetTechnique;
	ID3D11InputLayout** ppVertexLayout = ppGetVertexLayout;
	if( ppGetVertexLayout == NULL )
	{
		ppVertexLayout = &m_pVertexLayout;
	}

    // Create the input layout
    D3DX11_PASS_DESC PassDesc;
   (*ppTechnique)->GetPassByIndex( 0 )->GetDesc( &PassDesc );

    hr = pDevice->CreateInputLayout( pLayout, numElements, PassDesc.pIAInputSignature,
                                          PassDesc.IAInputSignatureSize, ppVertexLayout );
    if( FAILED( hr ) )
	{
        return hr;
	}
	return S_OK;
}
HRESULT TInputLayout::Create(		ID3D11Device* pDevice,
									D3D11_INPUT_ELEMENT_DESC* pLayout,
									UINT numElements,
									ID3DBlob* pVSBuf, 
									ID3D11InputLayout** ppGetVertexLayout  )
{
	HRESULT hr;
	assert( pVSBuf || pLayout );
	assert( pVSBuf );

	ID3D11InputLayout** ppVertexLayout = ppGetVertexLayout;
	if( ppGetVertexLayout == NULL ) 
	{
		ppVertexLayout = &m_pVertexLayout;
	}
	
    // Create the input layout
	hr =  pDevice->CreateInputLayout( pLayout, numElements, pVSBuf->GetBufferPointer(),pVSBuf->GetBufferSize(), ppVertexLayout );
	
    //SAFE_RELEASE( pVSBuf );

    if( FAILED( hr ) )
	{
        return hr;
	}
	return S_OK;
}
void TInputLayout::Apply(ID3D11DeviceContext*    pImmediateContext)
{
	assert( pImmediateContext);
	pImmediateContext->IASetInputLayout( m_pVertexLayout );
}
bool TInputLayout::Release()
{
	SAFE_RELEASE(m_pVertexLayout);
	return true;
}
TInputLayout::TInputLayout(void)
{
	m_pVertexLayout	= NULL;
}

TInputLayout::~TInputLayout(void)
{
	Release();
}
