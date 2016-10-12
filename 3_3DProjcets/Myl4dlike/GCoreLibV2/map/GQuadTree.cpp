#include "GQuadTree.h"
bool GQuadTree::Build( float fWidth, float fHeight )
{
	m_fWidth = fWidth, m_fHeight = fHeight;
	m_pRootNode = CreateNode( NULL, -fWidth/2,fWidth/2, -fHeight/2, fHeight/2 );

	if( BuildTree(m_pRootNode) )
	{
		return true;
	}
	return false;
}
bool GQuadTree::BuildTree( GNode* pNode)
{
	// ���� �����ϳ�
	if(SubDivide(pNode))
	{
		for( int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++ )
		{	
			if (m_bUsedIndexList)
			{
				DWORD dwIndex = pNode->m_ChildList[iNode]->m_dwPositionIndex[1] * pow(2.0f, (float)pNode->m_ChildList[iNode]->m_iDepth) +
					pNode->m_ChildList[iNode]->m_dwPositionIndex[0];
				m_LevelList[pNode->m_ChildList[iNode]->m_iDepth][dwIndex] = pNode->m_ChildList[iNode];
			}
			BuildTree( pNode->m_ChildList[iNode]);
		}
	}
	return true;
}
bool GQuadTree::SubDivide(GNode* pNode)
{	
	// 4��� �Ҽ� ������ ���̻� �������� �ʴ´�.
	if( pNode == NULL )	
	{
		return false;	
	}

	// �ִ� ���� �ѵ� �ʰ��� ���� ������� ����
	if( m_iMaxDepthLimit <= pNode->m_iDepth )
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}
	
	// ���� ����� ���� ũ�⸦ ����Ѵ�.
	
	float fWidthSplit = (pNode->m_CornerList[tTR].x - pNode->m_CornerList[tTL].x ) / 2;
	float fHeightSplit = (pNode->m_CornerList[tTL].z - pNode->m_CornerList[tBL].z ) / 2;

	// �ڽ� ��尡 ������ ����ũ�⺸�� �۴ٸ� ���̻� �������� �ʴ´�.
	if( fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize  )
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}	

	pNode->m_ChildList.push_back( CreateNode(	pNode,	pNode->m_CornerList[tTL].x,	
														pNode->m_CornerList[tTL].x + fWidthSplit,			
														pNode->m_CornerList[tTL].z - fHeightSplit,			
														pNode->m_CornerList[tTL].z) );

	pNode->m_ChildList.push_back( CreateNode(	pNode,	pNode->m_CornerList[tTL].x + fWidthSplit,			
														pNode->m_CornerList[tTR].x,	
														pNode->m_CornerList[tTL].z - fHeightSplit,		
														pNode->m_CornerList[tTL].z));

	pNode->m_ChildList.push_back( CreateNode(	pNode,	pNode->m_CornerList[tTL].x,	
														pNode->m_CornerList[tTL].x + fWidthSplit,			
														pNode->m_CornerList[tBL].z ,			
														pNode->m_CornerList[tTL].z- fHeightSplit ) );

	pNode->m_ChildList.push_back( CreateNode(	pNode,	pNode->m_CornerList[tTL].x + fWidthSplit,			
														pNode->m_CornerList[tTR].x,	
														pNode->m_CornerList[tBR].z ,		
														pNode->m_CornerList[tTL].z - fHeightSplit) );

	
	return true;
}
GNode* GQuadTree::CreateNode(GNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight)
{
	GNode* pNode = 0;
	SAFE_NEW(pNode, GNode);
	assert(pNode);

	pNode->m_ChildList.reserve(4);
	pNode->m_CornerList.reserve(4);

	// AABB
	pNode->m_tBox.vMin = D3DXVECTOR3(fTopLeft, 0.0f, fBottomLeft);
	pNode->m_tBox.vMax = D3DXVECTOR3(fTopRight, 0.0f, fBottomRight);
	pNode->m_tBox.vCenter = (pNode->m_tBox.vMax + pNode->m_tBox.vMin) / 2.0f;

	pNode->m_tBox.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pNode->m_tBox.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pNode->m_tBox.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;

	pNode->m_tBox.fRadius[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fRadius[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fRadius[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;

	pNode->m_CornerList.push_back(D3DXVECTOR3(pNode->m_tBox.vMin.x, 0.0f, pNode->m_tBox.vMax.z));
	pNode->m_CornerList.push_back(pNode->m_tBox.vMax);
	pNode->m_CornerList.push_back(pNode->m_tBox.vMin);
	pNode->m_CornerList.push_back(D3DXVECTOR3(pNode->m_tBox.vMax.x, 0.0f, pNode->m_tBox.vMin.z));

	if (pParentNode)
	{
		pNode->m_iDepth = pParentNode->m_iDepth + 1;
		if (m_iMaxDepth < pNode->m_iDepth)
		{
			m_iMaxDepth = pNode->m_iDepth;
		}
	}
	return pNode;
}

int GQuadTree::AddObject(GBaseObj* pObj)
{
	//pObj->m_iIndex = m_TotalObjList.size();
	//m_TotalObjList.push_back(pObj);

	if (CheckRect(m_pRootNode, pObj))
	{
		GNode* pNode = FindNode(m_pRootNode, pObj);
		if( pNode )
		{
			pNode->m_ObjectList.push_back( pObj );
		}
		return 1;
	}
	return 0;
}
int GQuadTree::CheckRect(GNode* pNode, GBaseObj* pObj)
{
	 //���ο� ���� ���� üũ
	if( pNode->m_tBox.vMin.x <= pObj->m_tBox.vMin.x && pNode->m_tBox.vMax.x >= pObj->m_tBox.vMax.x )
	{
		if( pNode->m_tBox.vMin.z <= pObj->m_tBox.vMin.z && pNode->m_tBox.vMax.z >= pObj->m_tBox.vMax.z )
		{
		return 1;
		}
	}	
	return 0;
	// AABB : AABB���� ���� �� ���� ���� ��쵵 üũ(overlap)
	//if (fabs(pNode->m_tBox.vCenter.x - pObj->m_tBox.vCenter.x) > (pNode->m_tBox.fRadius[0] + pObj->m_tBox.fRadius[0])) return 0;
	//if (fabs(pNode->m_tBox.vCenter.y - pObj->m_tBox.vCenter.y) > (pNode->m_tBox.fRadius[1] + pObj->m_tBox.fRadius[1])) return 0;
	//if (fabs(pNode->m_tBox.vCenter.z - pObj->m_tBox.vCenter.z) > (pNode->m_tBox.fRadius[2] + pObj->m_tBox.fRadius[2])) return 0;
	return 1;
}
GNode*	GQuadTree::FindNode(GNode* pNode, GBaseObj* pObj  )
{
	assert( pNode );
	do{		
		for( DWORD dwChild=0; dwChild < pNode->m_ChildList.size(); dwChild++ )
		{
			if( pNode->m_ChildList[dwChild] && CheckRect( pNode->m_ChildList[dwChild], pObj )  ) 
			{		
				m_QuadTreeQueue.push( pNode->m_ChildList[dwChild] );
			}
		}			
				
		if( m_QuadTreeQueue.empty() ) break;
		
		pNode = m_QuadTreeQueue.front();
		//pNode->m_ObjectList.push_back(pObj);

		m_QuadTreeQueue.pop();		
	}while( pNode );
	return pNode;
}
bool GQuadTree::PreFrame()
{
	m_DrawObjList.clear();
	m_DrawNodeList.clear();
	m_DrawObjSetList.set();
	m_DrawObjSetList.flip();

	if( !m_pCamera ) return false;
	return true;
}
bool GQuadTree::Frame()
{
	if( !PreFrame() ) return false;
	DrawFindNode( m_pRootNode );	
	return true;
}

void GQuadTree::VisibleNode( GNode* pNode )
{	
	assert( m_pCamera );
	if( pNode->m_iDepth < m_iRenderDepth ) return;
	
	if( m_pCamera->CheckOBBInPlane( &pNode->m_tBox ) )	
	{
		VisibleObject( pNode);
		for( int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++ )
		{		
			VisibleNode( pNode->m_ChildList[iNode] );
		}	
	}
}
void GQuadTree::VisibleObject( GNode* pNode )
{
	for( int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++ )
	{
		if( m_pCamera->CheckOBBInPlane( &pNode->m_ObjectList[iObj]->m_tBox ) )	
		{
			m_DrawObjList.push_back( pNode->m_ObjectList[iObj] );		
			//m_DrawObjSetList.set(pNode->m_ObjectList[iObj]->m_iIndex);
		}
	}
}
void GQuadTree::DrawFindNode(GNode* pNode)
{
	assert( pNode );

	G_POSITION t_Pos = m_pCamera->CheckPoitionOBBInPlane( &pNode->m_tBox );
	GetLodSubIndex(pNode);
	if( pNode->m_iDepth < m_iRenderDepth ) return;

	// ������� �� ���� ������ ���ܵ��� �ʾҴٸ�(���� �ְų� ���� ����)�߰��Ѵ�.
	if( pNode->m_isLeaf &&  t_Pos != P_BACK )
	{
		m_DrawNodeList.push_back( pNode );
		VisibleObject(pNode);
		return;
	}
	// ������ ���ԵǾ� ������ �ڽ��� Ž������ �ʰ� ��带 �߰��Ѵ�.
	if( t_Pos == P_FRONT)
	{
		m_DrawNodeList.push_back( pNode );
		VisibleNode(pNode);
		return;
	}

	// �θ� ��尡 ������ �ִ� ��쿡 �ش� �θ� ����� ������Ʈ���� üũ�Ѵ�.
	if( t_Pos == P_SPANNING )
	{
		VisibleObject(pNode);
	}

	for( int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++ )
	{
		DrawFindNode( pNode->m_ChildList[iNode]);
	}					
}
void GQuadTree::SetRenderDepth( DWORD dwDepth )
{
	if( m_iMaxDepth <= dwDepth ) m_iRenderDepth = -1; 
	else						m_iRenderDepth = dwDepth; 
}
void GQuadTree::FindNeighborNode(GNode* pNode)
{
	pNode->m_NeighborList.resize(4);

	DWORD dwNumPatchCount = pow(2.0f, (float)pNode->m_iDepth);

	DWORD dwNeighborCol, dwNeighborRow;
	if (pNode->m_dwPositionIndex[1] > 0)  // ��
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
		pNode->m_NeighborList[0] = m_LevelList[pNode->m_iDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1) // ��
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
		pNode->m_NeighborList[1] = m_LevelList[pNode->m_iDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] > 0) // ��
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_NeighborList[2] = m_LevelList[pNode->m_iDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) // ��
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_NeighborList[3] = m_LevelList[pNode->m_iDepth][dwNeighborRow + dwNeighborCol];
	}
}
void GQuadTree::SetNeighborNode(GNode* pNode)
{
	FindNeighborNode(pNode);
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		SetNeighborNode(pNode->m_ChildList[iNode]);
	}
}
DWORD GQuadTree::CheckSize(DWORD dwSize)
{
	float fCount = 0;
	DWORD dwChecker = dwSize / 2;
	if (dwChecker <= 0) return 0;

	while (dwChecker != 0)
	{
		dwChecker >>= 1;
		fCount += 1.0f;
	}
	float fMinimum = pow(2.0f, fCount - 1.0f);
	float fMaximum = pow(2.0f, fCount);
	// fMin = abs( fMin - P0 ) - abs( P1 - fMin );
	// fMax = abs( fMax - P0 ) - abs( P1 - fMax );
	// return min( fMin, fMax );
	float fMin = fabs(fMinimum - fabs(((float)dwSize - fMinimum)));
	float fMax = fabs(fMaximum - fabs(((float)dwSize - fMaximum)));

	// �񱳰��� ���ٸ� ���� ����ũ�⸦ ��ȯ�Ѵ�. ��) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
	if (fMin <= fMax)
	{
		return fMinimum;
	}
	return fMaximum;
}
bool GQuadTree::Release()
{
	SAFE_DEL(m_pRootNode);
	return true;
}


GQuadTree::GQuadTree(void)
{
	m_iNumCell	= 1;	
	m_iPatchLodCount	= 0;
	m_iRenderDepth		= 0;
	m_iMaxDepth			= 0;
	m_iMaxDepthLimit	= 4;
	m_fMinDivideSize	= 2;
	m_bUsedIndexList = false;
	SAFE_ZERO( m_pd3dDevice);
	SAFE_ZERO( m_pRootNode );
}

GQuadTree::~GQuadTree(void)
{
	Release();
}
