#include "GBspRender.h"

void	GBspRender::RenderLeafBoungingBox(ID3D11DeviceContext*		pContext, GCamera* pCamera, GNode* pNode )
{
	if( pNode == NULL )		return;		
	if( pNode->m_isLeaf == TRUE )
	{		
		if( pCamera->CheckOBBInPlane( &pNode->m_tBox ) )
		{
			m_cbData.Color =  D3DXVECTOR4( 1, 1, 1, 1 );
			RenderNode(pContext, pNode );
		}
		else
		{
			m_cbData.Color = D3DXVECTOR4( 0, 0, 0, 1 );
			RenderNode(pContext, pNode );
		}
	}
	for( int iChild=0; iChild < pNode->m_ChildList.size(); iChild++ )
	{
		RenderLeafBoungingBox(pContext, pCamera, pNode->m_ChildList[iChild] );
	}	
}
void	GBspRender::RecursionRender(ID3D11DeviceContext*		pContext, GNode* pNode )
{
	if( pNode == NULL )		return;	

	if( pNode->m_isLeaf == TRUE )
	{	
		RenderNode(pContext, pNode );
	}
	for( int iChild=0; iChild < pNode->m_ChildList.size(); iChild++ )
	{
		RecursionRender(pContext, pNode->m_ChildList[iChild] );
	}
	
}
void GBspRender::RenderNode(ID3D11DeviceContext*		pContext, GNode* pNode )
{
	PreRender(pContext);
	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;
	pContext->UpdateSubresource(m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);

	for( int iMtrl = 0; iMtrl < pNode->iNumMtrl; iMtrl++)
	{		
		pContext->PSSetShaderResources(0, 1, &I_Texture.GetPtr(pNode->m_MtrlList[iMtrl].iMtrl)->m_pTextureRV);
		pContext->IASetVertexBuffers(0,1,pNode->m_pVBList[iMtrl].GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer( pNode->m_pIBList[iMtrl].Get(), DXGI_FORMAT_R32_UINT, 0 );
		pContext->DrawIndexed( pNode->m_MtrlList[iMtrl].iNumFaces*3, 0, 0 );
	}
}
GBspRender::GBspRender(void)
{
}
GBspRender::~GBspRender(void)
{
}
