#include "TObject2D.h"

bool TObject2D::Create(ID3D11Device* pDevice,
	TCHAR* pLoadShaderString,
	TCHAR* pLoadTextureString,
	ID3D11BlendState* pBlendState)
{
	m_pBlendState = pBlendState;
	if (! TShape::Create(g_pd3dDevice, pLoadShaderString, pLoadTextureString)		)
	{
		return false;
	}
	InitBufferData(g_pd3dDevice);
	return true;

}
void TObject2D::SetMatrix(const D3DXMATRIX* pWorld, const D3DXMATRIX* pView,
	const D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenterPos.x = pWorld->_41;
		m_vCenterPos.y = pWorld->_42;
		m_vCenterPos.z = pWorld->_43;
	}
	if (pView)
	{
		m_matView = *pView;
	}
	if (pProj)
	{
		m_matProj = *pProj;
	}

	FLOAT fDeterminant;
	D3DXMATRIX matViewInverse;
	D3DXMatrixInverse(&m_matBillboard, &fDeterminant, &m_matView);
	m_matBillboard._41 = 0.0f;
	m_matBillboard._42 = 0.0f;
	m_matBillboard._43 = 0.0f;
	m_matBillboard._44 = 0.0f;
	m_matWorld = m_matBillboard * m_matWorld;
	TShape::SetMatrix(&m_matWorld, &m_matView, &m_matProj);


}
bool TObject2D::Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime)
{
	if (!TSprite::Frame(pContext, fGlobalTime, fElapsedTime))
	{
		return false;
	}

	for (int i = 0; i < m_ParticleList.size(); i++)
	{
		m_ParticleList[i].m_vPos += fElapsedTime * m_ParticleList[i].m_vAdd;
		m_ParticleList[i].m_fTimer += fElapsedTime;

		m_matBillboard._41 + m_ParticleList[i].m_vPos.x;
		m_matBillboard._42 + m_ParticleList[i].m_vPos.y;
		m_matBillboard._43 + m_ParticleList[i].m_vPos.z;

		m_pInstanceList[i].mMatWorld = m_matBillboard;
		m_pInstanceList[i].vColor = m_ParticleList[i].m_vColor;

		if (m_ParticleList[i].m_fTimer >= m_ParticleList[i].m_fLifeTime)
		{
			m_ParticleList[i].m_fTimer = 0;
			m_ParticleList[i].m_vAdd *= -1.0f;
		}
		if (m_BufferInstance)
		{
			TInstance* pInstances = NULL;
			D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
			if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_BufferInstance.Get(),
				0,D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
			{
				pInstances = (TInstance*)MappedFaceDest.pData;
				memcpy(pInstances, m_pInstanceList, sizeof(TInstance)* m_ParticleList.size());
				pContext->Unmap(m_BufferInstance.Get(), 0);
			}
		}
	}
	return true;
}
bool TObject2D::Render(ID3D11DeviceContext* pContext)
{
	ID3D11Buffer* Buffer[2] = { m_Object.g_pVertexBuffer.Get(), m_BufferInstance.Get() };
	UINT stride[2] = { sizeof(PCT_VERTEX), sizeof(TInstance) };
	UINT offset[2] = { 0,0 };
	DX::ApplyBS(pContext, m_pBlendState);
	PreRender(pContext);
	if (m_pAnimSRV != nullptr)
	{
		pContext->PSSetShaderResources(0, 1, &m_pAnimSRV);
	}
	pContext->IASetVertexBuffers(0, 2, Buffer, stride, offset);
	pContext->UpdateSubresource(m_Object.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	pContext->DrawIndexedInstanced(6, m_ParticleList.size(), 0, 0, 0);
	return true;
}
HRESULT TObject2D::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA,	0 },

		{ "mTransform", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA,     1 },
		{ "mTransform", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA,    1 },
		{ "mTransform", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA,    1 },
		{ "mTransform", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA,    1 },
		{ "POSCOLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 64,	D3D11_INPUT_PER_INSTANCE_DATA,	1 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_Object.g_pInputlayout.Attach(
		DX::CreateInputlayout(m_pd3dDevice, m_Object.g_pVSBlob.Get()->GetBufferSize(),
			m_Object.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
HRESULT TObject2D::InitBufferData(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;
	srand(timeGetTime());
	D3DXMATRIX matTrans;

	if (m_iCounter < 1)
	{
		m_iCounter = MAX_PARTICLE;	
	}
	m_ParticleList.reserve(m_iCounter);
	m_ParticleList.resize(m_iCounter);

	SAFE_NEW_ARRAY(m_pInstanceList, TInstance, m_ParticleList.size());
	for (int iPlane = 0; iPlane < m_ParticleList.size(); iPlane++)
	{
		TParticle particle;
		D3DXMatrixTranslation(&matTrans,
			particle.m_vPos.x,
			particle.m_vPos.y,
			particle.m_vPos.z); 
		m_pInstanceList[iPlane].mMatWorld = matTrans;
		m_pInstanceList[iPlane].vColor = particle.m_vColor;
		m_ParticleList[iPlane] = particle;
	}
	m_BufferInstance.Attach(DX::CreateVertexBuffer(pd3dDevice, m_pInstanceList,
		(UINT)m_ParticleList.size(), sizeof(TInstance), true));

	return hr;
}

TObject2D::TObject2D()
{
	m_pInstanceList = NULL;
	m_iCounter = 0;
}


TObject2D::~TObject2D()
{
	SAFE_DELETE_ARRAY(m_pInstanceList);
}
