#include "GShape.h"
void GShape::CreateOBBBox(float fExtentX, float fExtentY, float fExtentZ,
	D3DXVECTOR3 vCenter,
	D3DXVECTOR3 vDirX,
	D3DXVECTOR3 vDirY,
	D3DXVECTOR3 vDirZ)
{
	m_Box.fExtent[0] = fExtentX;
	m_Box.fExtent[1] = fExtentY;
	m_Box.fExtent[2] = fExtentZ;
	m_Box.vCenter = vCenter;

	m_Box.vAxis[0] = m_Box.fExtent[0] * vDirX;
	m_Box.vAxis[1] = m_Box.fExtent[1] * vDirY;
	m_Box.vAxis[2] = m_Box.fExtent[2] * vDirZ;

	m_Box.vPos[0] = vCenter - m_Box.vAxis[0] - m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[1] = vCenter - m_Box.vAxis[0] + m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[2] = vCenter + m_Box.vAxis[0] + m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[3] = vCenter + m_Box.vAxis[0] - m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[4] = vCenter - m_Box.vAxis[0] - m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[5] = vCenter - m_Box.vAxis[0] + m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[6] = vCenter + m_Box.vAxis[0] + m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[7] = vCenter + m_Box.vAxis[0] - m_Box.vAxis[1] + m_Box.vAxis[2];

	m_Box.vMax = m_Box.vPos[0];
	m_Box.vMin = m_Box.vPos[0];

	for (int iPoint = 1; iPoint < 8; iPoint++)
	{
		if (m_Box.vMax.x < m_Box.vPos[iPoint].x)
		{
			m_Box.vMax.x = m_Box.vPos[iPoint].x;
		}
		if (m_Box.vMax.y < m_Box.vPos[iPoint].y)
		{
			m_Box.vMax.y = m_Box.vPos[iPoint].y;
		}
		if (m_Box.vMax.z < m_Box.vPos[iPoint].z)
		{
			m_Box.vMax.z = m_Box.vPos[iPoint].z;
		}

		if (m_Box.vMin.x > m_Box.vPos[iPoint].x)
		{
			m_Box.vMin.x = m_Box.vPos[iPoint].x;
		}
		if (m_Box.vMin.y > m_Box.vPos[iPoint].y)
		{
			m_Box.vMin.y = m_Box.vPos[iPoint].y;
		}
		if (m_Box.vMin.z > m_Box.vPos[iPoint].z)
		{
			m_Box.vMin.z = m_Box.vPos[iPoint].z;
		}
	}
	D3DXVec3Normalize(&m_Box.vAxis[0], &m_Box.vAxis[0]);
	D3DXVec3Normalize(&m_Box.vAxis[1], &m_Box.vAxis[1]);
	D3DXVec3Normalize(&m_Box.vAxis[2], &m_Box.vAxis[2]);

}
void GShape::CreateAABBBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	m_Box.vMax = max;
	m_Box.vMin = min;
	m_Box.vPos[0] = D3DXVECTOR3(min.x, min.y, min.z);
	m_Box.vPos[1] = D3DXVECTOR3(min.x, max.y, min.z);
	m_Box.vPos[2] = D3DXVECTOR3(max.x, max.y, min.z);
	m_Box.vPos[3] = D3DXVECTOR3(max.x, min.y, min.z);

	m_Box.vPos[4] = D3DXVECTOR3(min.x, min.y, max.z);
	m_Box.vPos[5] = D3DXVECTOR3(min.x, max.y, max.z);
	m_Box.vPos[6] = D3DXVECTOR3(max.x, max.y, max.z);
	m_Box.vPos[7] = D3DXVECTOR3(max.x, min.y, max.z);

	m_Box.vCenter = (max + min) / 2.0f;
	m_Box.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Box.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Box.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TLineShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TLineShape::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
bool TLineShape::CreateVertexData()
{
	m_LineVertexList.resize(2);
	m_LineVertexList[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[1].p = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_LineVertexList[1].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	m_dxobj.m_iVertexSize = sizeof(PC_VERTEX);
	m_dxobj.m_iNumVertex = m_LineVertexList.size();
	return true;
}
bool TLineShape::CreateIndexData()
{
	m_IndexList.resize(2);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	
	m_dxobj.m_iNumIndex = m_IndexList.size();
	m_dxobj.m_iIndexSize = sizeof(DWORD);
	return true;
}
HRESULT TLineShape::CreateVertexBuffer()
{
	if (m_dxobj.m_iNumVertex <= 0) return S_OK;
	m_dxobj.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice,
		&m_LineVertexList.at(0),
		m_dxobj.m_iNumVertex,
		m_dxobj.m_iVertexSize));
	return S_OK;
}
bool TLineShape::Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor)
{
	PC_VERTEX vertices[2];
	vertices[0].p = vStart;
	vertices[0].c = vColor;
	vertices[1].p = vEnd;
	vertices[1].c = vColor;
	// ���� ���ҽ� ���� ��� 1 : D3D11_USAGE_DEFAULT ���
	pContext->UpdateSubresource(GetVB(), 0, NULL, &vertices, 0, 0);
	return Render(pContext);
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TLineShape::CreateResource()
{
	m_dxobj.m_iPrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	return S_OK;
}
TLineShape::TLineShape(void)
{
}

TLineShape::~TLineShape(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TLineShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT GDirectionLineShape::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
bool GDirectionLineShape::CreateVertexData()
{
	m_LineVertexList.resize(6);
	m_LineVertexList[0] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	m_LineVertexList[1] = PC_VERTEX(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	m_LineVertexList[2] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_LineVertexList[3] = PC_VERTEX(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_LineVertexList[4] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
	m_LineVertexList[5] = PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));

	m_dxobj.m_iVertexSize = sizeof(PC_VERTEX);
	m_dxobj.m_iNumVertex = m_LineVertexList.size();
	return true;
}
bool GDirectionLineShape::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 3;	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5;

	m_dxobj.m_iNumIndex = m_IndexList.size();
	m_dxobj.m_iIndexSize = sizeof(DWORD);
	return true;
}
HRESULT GDirectionLineShape::CreateVertexBuffer()
{
	if (m_dxobj.m_iNumVertex <= 0) return S_OK;
	m_dxobj.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice,
		&m_LineVertexList.at(0),
		m_dxobj.m_iNumVertex,
		m_dxobj.m_iVertexSize));
	return S_OK;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GDirectionLineShape::CreateResource()
{
	m_dxobj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	return S_OK;
}
GDirectionLineShape::GDirectionLineShape(void)
{
}

GDirectionLineShape::~GDirectionLineShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            GBoxShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool GBoxShape::CreateVertexData()
{
	// ���
	// 5    6
	// 1    2
	// �ϴ�
	// 4    7
	// 0    3  
	// �ո�
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	// �޸�
	m_VertexList[4] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// ������
	m_VertexList[8] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// ����
	m_VertexList[12] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// ����
	m_VertexList[16] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// �Ʒ���
	m_VertexList[20] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	
	m_dxobj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxobj.m_iNumVertex = m_VertexList.size();
	return true;
}
bool GBoxShape::CreateIndexData()
{	
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;
	
	m_dxobj.m_iNumIndex = m_IndexList.size();
	m_dxobj.m_iIndexSize = sizeof(DWORD);
	return true;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GBoxShape::CreateResource()
{
	m_dxobj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
GBoxShape::GBoxShape(void)
{
}

GBoxShape::~GBoxShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            GPlaneShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// x,y�� ���� ��ũ�� ��ġ, w,h ���� ��, ���� ��, vScreen�� ������ ���� �� ���� ũ��
bool GPlaneShape::SetScreenVertex(float x,
	float y,
	float w,
	float h,
	D3DXVECTOR2 vScreen)
{
	m_VertexList.resize(4);

	D3DXVECTOR2 vPoint;
	// 0 ~ 1
	vPoint.x = x / vScreen.x;
	vPoint.y = y / vScreen.y;
	// 0~1 => -1 ~ +1
	vPoint.x = vPoint.x *  2.0f - 1.0f;
	vPoint.y = -(vPoint.y *  2.0f - 1.0f);

	D3DXVECTOR2 vOffset;
	// 0 ~ 1
	vOffset.x = (x + w) / vScreen.x;
	vOffset.y = (y + h) / vScreen.y;
	// 0~1 => -1 ~ +1
	vOffset.x = (vOffset.x *  2.0f - 1.0f) - vPoint.x;
	vOffset.y = vPoint.y + (vOffset.y *  2.0f - 1.0f);

	m_VertexList[0].p.x = vPoint.x;
	m_VertexList[0].p.y = vPoint.y;
	m_VertexList[0].p.z = 0.5f;

	m_VertexList[1].p.x = vPoint.x + vOffset.x;
	m_VertexList[1].p.y = vPoint.y;
	m_VertexList[1].p.z = 0.5f;

	m_VertexList[2].p.x = vPoint.x + vOffset.x;
	m_VertexList[2].p.y = vPoint.y - vOffset.y;
	m_VertexList[2].p.z = 0.5f;

	m_VertexList[3].p.x = vPoint.x;
	m_VertexList[3].p.y = vPoint.y - vOffset.y;
	m_VertexList[3].p.z = 0.5f;
	return true;
}
bool GPlaneShape::CreateVertexData()
{
	if (m_VertexList.size() <= 0)
	{
		m_VertexList.resize(4);
		m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
		m_VertexList[2] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
		m_VertexList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	}
	else
	{
		m_VertexList[0] = PNCT_VERTEX(m_VertexList[0].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		m_VertexList[1] = PNCT_VERTEX(m_VertexList[1].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
		m_VertexList[2] = PNCT_VERTEX(m_VertexList[2].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
		m_VertexList[3] = PNCT_VERTEX(m_VertexList[3].p, D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	}
	m_dxobj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxobj.m_iNumVertex = m_VertexList.size();
	return true;
}
bool GPlaneShape::CreateIndexData()
{
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 0;
	m_IndexList[4] = 2;
	m_IndexList[5] = 3;

	m_dxobj.m_iNumIndex = m_IndexList.size();
	m_dxobj.m_iIndexSize = sizeof(DWORD);
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GPlaneShape::CreateResource()
{
	m_dxobj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
GPlaneShape::GPlaneShape(void)
{
}

GPlaneShape::~GPlaneShape(void)
{
}

bool GSphereShape::CreateVertexData()
{
	CreateSphere(40);

	m_dxobj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxobj.m_iNumVertex = m_VertexList.size();
	return true;
}
bool GSphereShape::CreateIndexData()
{
	return true;
}

void GSphereShape::MakeSpherePoint(std::vector<D3DXVECTOR3> &spherePoints,
	UINT Slices,
	double fRadius, D3DXVECTOR3 center)
{

	spherePoints.clear();
	spherePoints.resize(Slices* Slices);

	for (DWORD j = 0; j < Slices; j++)
	{
		FLOAT theta = (D3DX_PI*j) / (Slices);
		for (DWORD i = 0; i< Slices; i++)
		{
			int  iPos = j*Slices + i;
			FLOAT phi = (2 * D3DX_PI*i) / (Slices);
			spherePoints[iPos].x = fRadius * (float)(sin(theta)*cos(phi)) + center.x;
			spherePoints[iPos].z = fRadius * (float)(sin(theta)*sin(phi)) + center.y;
			spherePoints[iPos].y = fRadius * (float)(cos(theta)) + center.z;
		}
	}
}


void GSphereShape::CreateSphere(UINT Slices, float fRadius)
{
	const double PI = 3.141592653589793238462643383279502884197;
	int iPos = 0;
	vector<D3DXVECTOR3> VertexArray;	
	MakeSpherePoint(VertexArray, Slices, fRadius, D3DXVECTOR3(0, 0, 0));

	//m_VertexList.resize(VertexArray.size());

	int iNext = 0;
	for (DWORD j = 0; j < Slices; j++)
	{
		for (DWORD i = 0; i<Slices; i++)
		{
			if (i == Slices - 1)         iNext = 0;
			else iNext = i + 1;
			PNCT_VERTEX v0, v1, v2, v3, v4, v5;
			iPos = (j*Slices * 6) + (i * 6);

			v0.p = VertexArray[j*Slices + i];
			v1.p = VertexArray[j*Slices + iNext];

			if (j != Slices - 1) v2.p = VertexArray[((j + 1)*Slices) + i];
			else                v2.p = D3DXVECTOR3(0, -1, 0);

			v3.p = v2.p;
			v4.p = v1.p;

			if (j != Slices - 1) v5.p = VertexArray[((j + 1)*Slices) + iNext];
			else                 v5.p = D3DXVECTOR3(0, -1, 0);

			D3DXVec3Normalize(&v0.n, &v0.p);
			D3DXVec3Normalize(&v1.n, &v1.p);
			D3DXVec3Normalize(&v2.n, &v2.p);
			D3DXVec3Normalize(&v3.n, &v3.p);
			D3DXVec3Normalize(&v4.n, &v4.p);
			D3DXVec3Normalize(&v5.n, &v5.p);

			v0.c = D3DXVECTOR4(v0.n.x, v0.n.y, v0.n.z, 1.0f);
			v1.c = D3DXVECTOR4(v1.n.x, v1.n.y, v1.n.z, 1.0f);
			v2.c = D3DXVECTOR4(v2.n.x, v2.n.y, v2.n.z, 1.0f);
			v3.c = D3DXVECTOR4(v3.n.x, v3.n.y, v3.n.z, 1.0f);
			v4.c = D3DXVECTOR4(v4.n.x, v4.n.y, v4.n.z, 1.0f);
			v5.c = D3DXVECTOR4(v5.n.x, v5.n.y, v5.n.z, 1.0f);

			// �� �ؽ�ó ��ǥ
			v0.t = D3DXVECTOR2(atan2(v0.n.z, v0.n.x) / (PI*2.0f) + 0.5f, asinf(-v0.n.y) / PI + 0.5f);
			v1.t = D3DXVECTOR2(atan2(v1.n.z, v1.n.x) / (PI*2.0f) + 0.5f, asinf(-v1.n.y) / PI + 0.5f);
			v2.t = D3DXVECTOR2(atan2(v2.n.z, v2.n.x) / (PI*2.0f) + 0.5f, asinf(-v2.n.y) / PI + 0.5f);
			v3.t = D3DXVECTOR2(atan2(v3.n.z, v3.n.x) / (PI*2.0f) + 0.5f, asinf(-v3.n.y) / PI + 0.5f);
			v4.t = D3DXVECTOR2(atan2(v4.n.z, v4.n.x) / (PI*2.0f) + 0.5f, asinf(-v4.n.y) / PI + 0.5f);
			v5.t = D3DXVECTOR2(atan2(v5.n.z, v5.n.x) / (PI*2.0f) + 0.5f, asinf(-v5.n.y) / PI + 0.5f);

			// ���� ȯ�� ��( ���游 �������� )
			/*	v0.t = D3DXVECTOR2( (v0.n.x + 1) / 2.0f,  (-v0.n.y+1) / 2.0f );
			v1.t = D3DXVECTOR2( (v1.n.x + 1) / 2.0f,  (-v1.n.y+1) / 2.0f);
			v2.t = D3DXVECTOR2( (v2.n.x + 1) / 2.0f,  (-v2.n.y+1) / 2.0f);
			v3.t = D3DXVECTOR2( (v3.n.x + 1) / 2.0f,  (-v3.n.y+1) / 2.0f);
			v4.t = D3DXVECTOR2( (v4.n.x + 1) / 2.0f,  (-v4.n.y+1) / 2.0f);
			v5.t = D3DXVECTOR2( (v5.n.x + 1) / 2.0f,  (-v5.n.y+1) / 2.0f);
			*/
			//////////////////////////////////////
			//   V0.t.x( 0.94f) ---- V1.t.x(0.0f) �� ��쿡 
			//   v1.t.x= 1.0f���� �����Ѵ�.
			//////////////////////////////////////
			if (fabs(v1.t.x - v0.t.x) > 0.9f)
			{
				if (fabs(v1.t.x) <= 0.01f)
				{
					v1.t.x = 1.0f;
				}
				if (fabs(v0.t.x) <= 0.01f)
				{
					v0.t.x = 1.0f;
				}
			}
			if (fabs(v2.t.x - v0.t.x) > 0.9f)
			{
				if (fabs(v2.t.x) <= 0.01f)
				{
					v2.t.x = 1.0f;
				}
				if (fabs(v0.t.x) <= 0.01f)
				{
					v0.t.x = 1.0f;
				}
			}
			if (fabs(v4.t.x - v3.t.x) > 0.9f)
			{
				if (fabs(v4.t.x) <= 0.01f)
				{
					v4.t.x = 1.0f;
				}
				if (fabs(v3.t.x) <= 0.01f)
				{
					v3.t.x = 1.0f;
				}
			}
			if (fabs(v5.t.x - v3.t.x) > 0.9f)
			{
				if (fabs(v5.t.x) <= 0.01f)
				{
					v5.t.x = 1.0f;
				}
				if (fabs(v3.t.x) <= 0.01f)
				{
					v3.t.x = 1.0f;
				}
			}
			m_VertexList.push_back(v0);
			m_VertexList.push_back(v1);
			m_VertexList.push_back(v2);
			m_VertexList.push_back(v3);
			m_VertexList.push_back(v4);
			m_VertexList.push_back(v5);
		}
	}
	VertexArray.clear();
}

bool GSphereShape::Render(ID3D11DeviceContext*	pContext)
{
	PreRender(pContext);
	pContext->UpdateSubresource(m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	pContext->Draw(m_dxobj.m_iNumVertex, 0 );
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GSphereShape::CreateResource()
{
	m_dxobj.m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	return S_OK;
}
GSphereShape::GSphereShape(void)
{
}

GSphereShape::~GSphereShape(void)
{
}