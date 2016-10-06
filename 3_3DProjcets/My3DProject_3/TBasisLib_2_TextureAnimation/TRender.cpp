#include "TRender.h"
void TRender::SetMatrix( D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj )
{
	if(pWorld != NULL)
	{
		m_matWorld = *pWorld;
	}
	if(pView != NULL)
	{
		m_matView = *pView;
	}
	if(pProj != NULL)
	{
		m_matProj = *pProj;
	}	
}
void TRender::SetTechnique(ID3DX11EffectTechnique* pTechnique)
{
	if( pTechnique )	m_pTechniqueApply	= pTechnique;
	else				m_pTechniqueApply	= m_pTechnique;
}

HRESULT TRender::LoadEffectFile(	const TCHAR* strShaderFile, 								
									D3D10_SHADER_MACRO* pShaderMacros )
{	
	if( strShaderFile )
	{
		m_dwShaderIndex = I_Shader.Add(strShaderFile);
		if( m_dwShaderIndex <= 0 ) 
		{
			MessageBox( 0, _T("Lighting.fx  실패"), _T("Load error"), MB_OK );
			return E_FAIL;
		}
		m_pShader = I_Shader.GetPtr( m_dwShaderIndex );
		GetEffectVariable();	
	}
	return S_OK;
}
bool	TRender::LoadEffectFile(int iShaderID)
{ 
	if (iShaderID < 0) return false;
	m_iShaderID = iShaderID;
	m_pShader = I_Shader.GetPtr(m_iShaderID);
	if (m_pShader == NULL) return false;
	return true;
}

HRESULT TRender::LoadShaderFile(		const TCHAR* strShaderFile, 	
										LPCSTR strVSEntryPoint, 
										LPCSTR strPSEntryPoint, 
										LPCSTR strShaderModel,
										D3D10_SHADER_MACRO* pShaderMacros )		
{
	if( strShaderFile )
	{
		m_dwShaderIndex = I_Shader.Add(strShaderFile,strVSEntryPoint,strPSEntryPoint,  strShaderModel );
		if( m_dwShaderIndex <= 0 ) 
		{
			MessageBox( 0, _T("Lighting.fx  실패"), _T("Load error"), MB_OK );
			return E_FAIL;
		}
		m_pShader = I_Shader.GetPtr( m_dwShaderIndex );		
	}
	return S_OK;
}
void	TRender::GetEffectVariable()
{
	assert(m_pShader->GetEffect());

	  // Obtain the technique
    m_pTechnique = m_pShader->AddTechnique("Render");
	m_pTechniqueColor = m_pShader->AddTechnique("ColorRender");
	
	if( m_pTechnique == NULL  || m_pTechnique->IsValid() == FALSE ) 
	{
		MessageBox( 0, _T("m_pTechnique->IsValid()"), _T("Load error"), MB_OK );
		return;
	}
	if( m_pTechniqueColor == NULL  || m_pTechniqueColor->IsValid() == FALSE ) 
	{
		MessageBox( 0, _T("m_pTechniqueColor->IsValid()"), _T("Load error"), MB_OK );
		return;
	}

	SetTechnique(m_pTechnique);

	m_pShader->AddVariable("g_matWorld");
	m_pShader->AddVariable("g_matView");
	m_pShader->AddVariable("g_matProj");
	m_pShader->AddVariable("g_txDiffuse");	
}
HRESULT TRender::SetInputLayout()
{	
	if( m_InputLayout.m_pVertexLayout != NULL ) return S_OK;
	// 레이아웃 생성
	D3D11_INPUT_ELEMENT_DESC layoutObject[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

	if( m_pTechnique == NULL ) 
	{
		if( FAILED( m_InputLayout.Create( m_pd3dDevice, layoutObject, COUNTOF(layoutObject), m_pShader->m_pBlobVS, 0  ) ) )
		{
			MessageBox( 0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK );
			return 0;
		}
	}else
	{	
		if( FAILED( m_InputLayout.Create( m_pd3dDevice, layoutObject, COUNTOF(layoutObject), &m_pTechnique, 0  ) ) )
		{
			MessageBox( 0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK );
			return 0;
		}
	}
	m_iVertexSize = sizeof( layoutObject );
	return S_OK;
}
void TRender::BindVertexBuffer( ID3D11Buffer* pVertexBuffer, 							   
								UINT iStrides,						   
							    UINT iStartSlot,
								UINT iNumBuffers,								
								UINT iOffsets )
{
	if(pVertexBuffer!=NULL)
	{
		m_pImmediateContext->IASetVertexBuffers( iStartSlot, iNumBuffers, &pVertexBuffer, &iStrides, &iOffsets );	
	}else
	{
		m_pImmediateContext->IASetVertexBuffers( iStartSlot, iNumBuffers, &m_BufferVB.m_pBuffer, &m_iVertexSize, &iOffsets );	
	}
}
void TRender::BindVertexBuffer( ID3D11DeviceContext* pContext,
							    ID3D11Buffer* pVertexBuffer, 
							    UINT iStrides,						   
							    UINT iStartSlot,
								UINT iNumBuffers,								
								UINT iOffsets  )
{
	if(pVertexBuffer!=NULL)
	{
		pContext->IASetVertexBuffers( iStartSlot, iNumBuffers, &pVertexBuffer, &iStrides, &iOffsets );	
	}else
	{
		pContext->IASetVertexBuffers( iStartSlot, iNumBuffers, &m_BufferVB.m_pBuffer, &m_iVertexSize, &iOffsets );	
	}
}
void TRender::BindIndexBuffer( ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset )
{
	if(pIndexBuffer!=NULL)
	{
		m_pImmediateContext->IASetIndexBuffer( pIndexBuffer, Format, Offset ); 		
	}else
	{
		m_pImmediateContext->IASetIndexBuffer( m_BufferIB.m_pBuffer, Format, Offset );
	}
}
void TRender::BindIndexBuffer( ID3D11DeviceContext* pContext,ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset )
{
	if(pIndexBuffer!=NULL)
	{
		pContext->IASetIndexBuffer( pIndexBuffer, Format, Offset ); 		
	}else
	{
		pContext->IASetIndexBuffer( m_BufferIB.m_pBuffer, Format, Offset );
	}
}
void TRender::Draw( UINT VertexCount,UINT StartVertexLocation)
{
	D3DX11_TECHNIQUE_DESC techDesc;
	if( m_pTechniqueApply )
	{
		m_pTechniqueApply->GetDesc( &techDesc );
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			ID3DX11EffectPass* pPass = m_pTechniqueApply->GetPassByIndex( p );
			pPass->Apply( 0, m_pImmediateContext );
			m_pImmediateContext->Draw( VertexCount, StartVertexLocation );	
		}
	}else
	{
		// Set the shaders
		m_pImmediateContext->VSSetShader( m_pShader->m_pVertexShader, NULL, 0 );
		m_pImmediateContext->PSSetShader( m_pShader->m_pPixelShader, NULL, 0 );
		m_pImmediateContext->Draw( VertexCount, StartVertexLocation );	
	}
}
void TRender::Draw( ID3D11DeviceContext* pContext,UINT VertexCount,UINT StartVertexLocation )
{
	D3DX11_TECHNIQUE_DESC techDesc;
	if( m_pTechniqueApply)
	{
		m_pTechniqueApply->GetDesc( &techDesc );
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			ID3DX11EffectPass* pPass = m_pTechniqueApply->GetPassByIndex( p );
			pPass->Apply( 0, pContext );
			pContext->Draw( VertexCount, StartVertexLocation );	
		}
	}else
	{
		// Set the shaders
		pContext->VSSetShader( m_pShader->m_pVertexShader, NULL, 0 );
		pContext->PSSetShader( m_pShader->m_pPixelShader, NULL, 0 );
		pContext->Draw( VertexCount, StartVertexLocation );	
	}
}
void TRender::DrawIndex( UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation )
{
	D3DX11_TECHNIQUE_DESC techDesc;
	if(m_pTechniqueApply)
	{
		m_pTechniqueApply->GetDesc( &techDesc );
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			ID3DX11EffectPass* pPass = m_pTechniqueApply->GetPassByIndex( p );
			pPass->Apply( 0, m_pImmediateContext );
			m_pImmediateContext->DrawIndexed( IndexCount, StartIndexLocation, BaseVertexLocation );
		}  
	}else
	{		
		// Set the shaders
		m_pImmediateContext->VSSetShader( m_pShader->m_pVertexShader, NULL, 0 );
		m_pImmediateContext->PSSetShader( m_pShader->m_pPixelShader, NULL, 0 );
		m_pImmediateContext->DrawIndexed( IndexCount, StartIndexLocation, BaseVertexLocation );
	}
}
void TRender::DrawIndex( ID3D11DeviceContext* pContext,UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation )
{
	D3DX11_TECHNIQUE_DESC techDesc;
	if( m_pTechniqueApply )
	{
		m_pTechniqueApply->GetDesc( &techDesc );
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			ID3DX11EffectPass* pPass = m_pTechniqueApply->GetPassByIndex( p );
			pPass->Apply( 0, pContext );
			pContext->DrawIndexed( IndexCount, StartIndexLocation, BaseVertexLocation );
		}
	}
	else
	{
		// Set the shaders
		pContext->VSSetShader( m_pShader->m_pVertexShader, NULL, 0 );
		pContext->PSSetShader( m_pShader->m_pPixelShader, NULL, 0 );
		pContext->DrawIndexed( IndexCount, StartIndexLocation, BaseVertexLocation );
	}
}
TRender::TRender(void)
{
	m_pShader			= NULL;
	m_pTechnique		= NULL;
	m_pTechniqueApply	= NULL;
	m_pd3dDevice		= NULL;
	m_pImmediateContext	= NULL;
	m_dwShaderIndex		= 0;
	m_iVertexSize		= 0;
	m_iIndexSize		= 0;
	m_iShaderID			= -1;
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matProj );
}

TRender::~TRender(void)
{
}
