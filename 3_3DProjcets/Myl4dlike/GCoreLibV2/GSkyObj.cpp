#include "GSkyObj.h"

void GSkyObj::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
		m_matView._41 = 0.0f;
		m_matView._42 = 0.0f;
		m_matView._43 = 0.0f;
		m_matView._44 = 1.0f;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
}
GSkyObj::GSkyObj(void)
{
	D3DXMatrixScaling(&m_matWorld, 10.0f, 10.0f, 10.0f);
}

GSkyObj::~GSkyObj(void)
{
}
