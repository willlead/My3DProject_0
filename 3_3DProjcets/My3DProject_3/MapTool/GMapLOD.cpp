#include "stdafx.h"
#include "GMapLOD.h"


GMapLOD::GMapLOD()
{
	m_fExpansionRatio = 0.1f;
	m_iNumCell = 1;
	m_iPatchLodCount = 0;
	m_pCamera = NULL;
	m_bThresHoldValue = false;
}


GMapLOD::~GMapLOD()
{
}

void GMapLOD::InitLevelOrder(GNode * pRootNode, int iMaxDepth)
{
	m_LevelList.resize(iMaxDepth + 1);
	for (int iLevel = 0; iLevel < iMaxDepth; iLevel++)
	{
		m_LevelList[iLevel].resize(pow(4.0f, iLevel));
	}
	m_LevelList[0].push_back(pRootNode);
}

void GMapLOD::SetLOD(DWORD dwWidth, int iNumDepth)
{
	m_iNumCell = (dwWidth - 1) / pow(2.0f, (float)iNumDepth);

	m_iPatchLodCount = (log((float)m_iNumCell) / log(2.0f));

}

DWORD GMapLOD::GetLodSubIndex(GNode * pNode)
{
	int iNumPatchIndex = m_iNumCell;
	/*float fRatio = GetExpansionRatio(	pNode->m_tBox.vCenter,
	D3DXVec3Length( &(pNode->m_tBox.vMax-pNode->m_tBox.vCenter )));*/

	/*	float fRatio = GetExpansionRatio(	pNode->m_tBox.vMax, pNode->m_tBox.vMin);	*/

	float fRatio = GetExpansionRatio(pNode->m_tBox.vCenter);
	DWORD dwCurentRatio = fRatio * m_iPatchLodCount;
	DWORD dwMaxRatio = fRatio * m_iPatchLodCount + 0.5f;
	DWORD dwMinRatio = fRatio * m_iPatchLodCount - 0.5f;


	// ����ȿ�� (����� LOD�� ���� �ٲ�� ���� �����Ѵ�.)
	if (m_bThresHoldValue)
	{
		if (pNode->m_dwLodLevel < dwCurentRatio)
		{
			if (pNode->m_dwLodLevel < dwMinRatio)		// B
			{
				pNode->m_dwLodLevel = dwCurentRatio;
			}
		}
		else if (pNode->m_dwLodLevel > dwCurentRatio)
		{
			if (pNode->m_dwLodLevel > dwMaxRatio)		// B
			{
				pNode->m_dwLodLevel = dwCurentRatio;
			}
		}
	}
	else
	{
		pNode->m_dwLodLevel = dwCurentRatio;
	}
	///����ó��
	if (m_iPatchLodCount < pNode->m_dwLodLevel)
	{
		pNode->m_dwLodLevel -= 1;
	}

	return pNode->m_dwLodLevel;
}

DWORD GMapLOD::GetLodType(GNode * pNode)
{
	if (pNode->m_NeighborList.size() <= 0) return 0;
	DWORD dwType = 0;
	if (pNode->m_NeighborList[0] && pNode->m_NeighborList[0]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 1;
	if (pNode->m_NeighborList[1] && pNode->m_NeighborList[1]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 4;
	if (pNode->m_NeighborList[2] && pNode->m_NeighborList[2]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 8;
	if (pNode->m_NeighborList[3] && pNode->m_NeighborList[3]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 2;

	pNode->m_dwLodType = dwType;
	return dwType;
}

float GMapLOD::GetExpansionRatio(D3DXVECTOR3 vCenter, float fRadius)
{
	float fRatio = 0.0f;
	D3DXVECTOR3 vCenterViewPos, vSideViewPos;
	vSideViewPos = vCenter + (*m_pCamera->GetRightVector() * fRadius);
	D3DXVec3TransformCoord(&vCenterViewPos, &vCenter, m_pCamera->GetViewMatrix());
	D3DXVec3TransformCoord(&vSideViewPos, &vSideViewPos, m_pCamera->GetViewMatrix());

	D3DXVECTOR3 vCenterProjPos, vSideProjPos;
	D3DXVec3TransformCoord(&vCenterProjPos, &vCenterViewPos, m_pCamera->GetProjMatrix());
	D3DXVec3TransformCoord(&vSideProjPos, &vSideViewPos, m_pCamera->GetProjMatrix());

	D3DXVec3Normalize(&vCenterProjPos, &vCenterProjPos);
	D3DXVECTOR2 vA = D3DXVECTOR2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	D3DXVECTOR2 vB = D3DXVECTOR2(vSideProjPos.x* 0.5f + 0.5f, vSideProjPos.y* 0.5f + 0.5f);

	//TViewPort m_MainVP;
	//m_MainVP.Set( 0, 0, 800, 600, 0.0f, 1.0f );
	//D3DXVec3Project( &vCenterProjPos, &vCenter,&m_MainVP.m_ViewPort,m_pCamera->GetProjMatrix(), m_pCamera->GetViewMatrix(), 0 );
	//D3DXVec3Unproject(&vCenterProjPos, &vCenterProjPos,&m_MainVP.m_ViewPort,m_pCamera->GetProjMatrix(), m_pCamera->GetViewMatrix(), 0 );
	//fRatio = D3DXVec2Length( &(D3DXVECTOR2(0.0f,0.0f) - D3DXVECTOR2(1.0f,1.0f)));
	//fRatio = vCenterProjPos.z;//D3DXVec2Length( &(D3DXVECTOR2(vA-vB)));
	// ī�޶� �������� �ָ� ������ ��쿡 �ش���.
	float fDistance = D3DXVec3Length(&(*m_pCamera->GetEyePt() - vCenter));
	if (fDistance > m_pCamera->m_fFarPlane)
	{
		return 1.0f;
	}
	fRatio = 1.0f - fDistance / m_pCamera->m_fFarPlane;
	return fRatio;
}

float GMapLOD::GetExpansionRatio(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin)
{
	float fRatio = 0.0f;
	D3DXVECTOR3 vCenterViewPos, vSideViewPos;

	//D3DXMATRIX matInverse = *m_pCamera->GetViewMatrix();
	//D3DXMatrixInverse( &matInverse, 0, &matInverse);
	//matInverse._41 = 0.0f;
	//matInverse._42 = 0.0f;
	//matInverse._43 = 0.0f;
	//matInverse._44 = 1.0f;
	//D3DXVec3TransformCoord( &vMax, &vMax, &matInverse );		
	//D3DXVec3TransformCoord( &vMin, &vMin, &matInverse );

	D3DXVec3TransformCoord(&vCenterViewPos, &vMax, m_pCamera->GetViewMatrix());
	D3DXVec3TransformCoord(&vSideViewPos, &vMin, m_pCamera->GetViewMatrix());

	D3DXVECTOR3 vCenterProjPos, vSideProjPos;
	D3DXVec3TransformCoord(&vCenterProjPos, &vCenterViewPos, m_pCamera->GetProjMatrix());
	D3DXVec3TransformCoord(&vSideProjPos, &vSideViewPos, m_pCamera->GetProjMatrix());

	D3DXVECTOR2 vA = D3DXVECTOR2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	D3DXVECTOR2 vB = D3DXVECTOR2(vSideProjPos.x* 0.5f + 0.5f, vSideProjPos.y* 0.5f + 0.5f);

	fRatio = D3DXVec3Length(&(vCenterProjPos - vSideProjPos));
	//fRatio = D3DXVec2Length( &(D3DXVECTOR2(vA-vB)));
	return fRatio;
}

float GMapLOD::GetExpansionRatio(D3DXVECTOR3 vCenter)
{
	float fDistance = D3DXVec3Length(&(*m_pCamera->GetEyePt() - vCenter));
	if (fDistance > m_pCamera->m_fFarPlane)
	{
		return 1.0f;
	}
	float fRatio = fDistance / m_pCamera->m_fFarPlane;
	return fRatio;
}

void GMapLOD::GetDrawPatchNode()
{
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		AddDrawPatchNode(m_DrawNodeList[iNode]);
	}
}

void GMapLOD::AddDrawPatchNode(GNode * pNode)
{
	if (pNode->m_isLeaf)
	{
		m_DrawPatchNodeList.push_back(pNode);
		GetLodSubIndex(pNode);
		return;
	}
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		AddDrawPatchNode(pNode->m_ChildList[iNode]);
	}
}

void GMapLOD::Update(ID3D11Device * pd3dDevice, TCamera * pCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_pCamera = pCamera;
}
