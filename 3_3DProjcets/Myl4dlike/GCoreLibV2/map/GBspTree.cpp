#include ".\gbsptree.h"


bool GBspTree::Load( ID3D11Device*  pd3dDevice, TCHAR* szFileName, bool Binary )
{
	SAFE_NEW( m_pRootNode, GNode );	
	if( !m_BspParser.Load( GetDevice(), m_pRootNode, szFileName, Binary ) )
	{
		return false;
	}
	m_pRootNode->iNumPolygon = CalculateBox( &m_pRootNode->m_tBox, m_pRootNode->m_pSplitter );
	
	return true;
}
bool	GBspTree::Build(ID3D11Device*   pd3dDevice, GPolygon* pPolyList, TCHAR*	pLoadShaderString )
{	
	if( !m_BspRender.Create(pd3dDevice, pLoadShaderString ) )
	{
		return false;
	}
	SAFE_NEW ( m_pRootNode, GNode )	
	m_pRootNode->m_ChildList.resize(2);		
	m_pRootNode->iNumPolygon = CalculateBox( &m_pRootNode->m_tBox, pPolyList );
	
	BuildBSPTree( m_pRootNode, pPolyList );		

	m_BspParser.SetDevice( pd3dDevice );
	m_BspParser.LoadTexture();	
	m_BspParser.SetNodePerRenderGroup(m_pRootNode);

	m_DrawObjList.reserve( 100 );
	m_DrawNodeList.reserve( 100 );
	return true;
}

bool	GBspTree::Init()
{	
	m_pRootNode		=	NULL;		
	m_pCollisionPoly=   NULL; 

	m_vCurrentPosition = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );	
	return true;
}
bool	GBspTree::Frame()
{
	m_pCollisionNode = NULL;
	m_DrawObjList.clear();
	m_DrawNodeList.clear();
	if( !m_pCamera ) return false;
	VisibleNode(m_pRootNode);	

	GNode* pNode = m_pRootNode;
	if( !pNode )
		return false;
	while( pNode->m_ChildList[0] && pNode->m_ChildList[0]->m_isLeaf != TRUE )
	{		
		switch( ClassifyPoint( pNode->m_pSplitter, &m_vCurrentPosition))
		{
			case P_ONPLANE:		
			case P_FRONT:
			pNode = pNode->m_ChildList[0];
			break;
			case P_BACK:
			pNode = pNode->m_ChildList[1];
			break;
			default:
			 DEBUGMSG("BSP Ʈ���� �̻��մϴ�.");
		}
	}	
	if( pNode->m_ChildList[0] )
	{
		// m_vCurrentPosition(������Ʈ ��ġ)�� ��ġ�� ��� �� �浹(������)������
		m_pCollisionPoly = pNode->m_ChildList[0]->m_pSplitter;
		m_pCollisionNode = pNode->m_ChildList[0];
	}

	return true;
}
bool	GBspTree::Render(ID3D11DeviceContext*		pContext )
{
	m_BspRender.m_cbData.Color = D3DXVECTOR4( 1, 1, 1, 1 );		

	// ȭ�鿡 ���̴³�� ������Ʈ ������
	for( int iNode=0; iNode < m_DrawNodeList.size(); iNode++ )
	{		
		m_BspRender.RenderNode(pContext,m_DrawNodeList[iNode] );
	}

	// m_vCurrentPosition�� ��ġ�� ��常 ������
	m_BspRender.m_cbData.Color= D3DXVECTOR4( 1, 1, 1, 1 );
	m_BspRender.RenderNode(pContext,m_pCollisionNode);
	// ��ü Ʈ�� ��ȸ�Ͽ� ������
	m_BspRender.RecursionRender(pContext, m_pRootNode);
	return true;	
}
bool	GBspTree::Release()
{	
	SAFE_DEL(m_pRootNode);	
	return true;
}
void GBspTree::BuildBSPTree( GNode* pNode, GPolygon* pPolyList )
{
	GPolygon*	pFrontList		= NULL;
	GPolygon*	pBackList		= NULL;
	GPolygon*	pCurrentPoly		= pPolyList;
	GPolygon*	pNextPoly		= NULL;

	pNode->m_pSplitter = SelectBestSplitter( pCurrentPoly );
	if( pNode->m_pSplitter )	pNode->iNumPolygon = 1;

	while( pCurrentPoly != NULL )
	{
		pNextPoly = pCurrentPoly->pNext;
		if( pNode->m_pSplitter )
		{
			switch( ClassifyPoly( pNode->m_pSplitter, pCurrentPoly ) )
			{
			case P_FRONT:
			case P_ONPLANE:
				{
					pCurrentPoly->pNext	= pFrontList;
					pFrontList			= pCurrentPoly; 		
				}break;
			case P_BACK:
				{
					pCurrentPoly->pNext	= pBackList;
					pBackList			= pCurrentPoly;					
				}break;
			case P_SPANNING:
				{						
					GPolygon*	pFrontSplitter  = NULL;
					GPolygon*	pBackSplitter  = NULL;
					SAFE_NEW( pFrontSplitter, GPolygon ) 
					SAFE_NEW( pBackSplitter, GPolygon ) 
					ZeroMemory( pFrontSplitter, sizeof( GPolygon ) );
					ZeroMemory( pBackSplitter, sizeof( GPolygon ) );

					SplitterPolygon( pCurrentPoly,pNode->m_pSplitter, pFrontSplitter, pBackSplitter );
					pFrontSplitter->pNext	= pFrontList;
					pFrontList				= pFrontSplitter; 
					
					pBackSplitter->pNext	= pBackList;
					pBackList				= pBackSplitter;
							
					SAFE_DEL(pCurrentPoly);			
				}break;
			default:
				break;
			}
		}
		pCurrentPoly = pNextPoly;
	}

	if( !pBackList )
	{			
		SAFE_NEW( pNode->m_ChildList[0], GNode )		
		pNode->m_ChildList[0]->m_ChildList.resize(2);		
		pNode->m_ChildList[0]->m_isLeaf = true; 	
		if( !pFrontList ) 
		{
			pNode->m_ChildList[0]->m_pSplitter = pPolyList;				
		}
		else
		{
			pNode->m_ChildList[0]->m_pSplitter = pFrontList;				
		}
		pNode->m_ChildList[0]->iNumPolygon = CalculateBox( &pNode->m_ChildList[0]->m_tBox, pNode->m_ChildList[0]->m_pSplitter );
	}
	else
	{
		SAFE_NEW( pNode->m_ChildList[0], GNode )
		pNode->m_ChildList[0]->m_ChildList.resize(2);		
		pNode->m_ChildList[0]->m_isLeaf = false; 			
		BuildBSPTree( pNode->m_ChildList[0], pFrontList ); 
	}
	if( !pBackList )
	{
		SAFE_NEW( pNode->m_ChildList[1], GNode )
		pNode->m_ChildList[1]->m_isLeaf = 3;
	}
	else
	{
		SAFE_NEW( pNode->m_ChildList[1], GNode )
		pNode->m_ChildList[1]->m_ChildList.resize(2);		
		pNode->m_ChildList[1]->m_isLeaf = false; 			
		BuildBSPTree( pNode->m_ChildList[1], pBackList ); 
	}
}

GPolygon* GBspTree::SelectBestSplitter( GPolygon* pPolyList )
{
	GPolygon*	pSelectedPoly	= NULL;
	GPolygon*	m_pSplitter= NULL;
	int			iBestScore		= 100000;
	int iNumFront, iNumBack, iNumSplitter;

	int iScore = 0;
	for( GPolygon* pSplitterPoly = pPolyList; pSplitterPoly ; pSplitterPoly = pSplitterPoly->pNext )
	{		
		iNumFront = iNumBack = iNumSplitter = 0;
		if( !pSplitterPoly->bBeenUsedAsSplitter )
		{
			for( GPolygon* pCurrentPoly = pPolyList; pCurrentPoly ; pCurrentPoly = pCurrentPoly->pNext )
			{	
				if( ( pSplitterPoly != pCurrentPoly ) &&  !pCurrentPoly->bBeenUsedAsSplitter )
				{
					switch( ClassifyPoly( pSplitterPoly, pCurrentPoly ) )
					{
					case P_ONPLANE:
						{
							iNumFront++;							
						}break;
					case P_FRONT:
						{
							iNumFront++;
						}break;
					case P_BACK:
						{
							iNumBack++;
						};break;
					case P_SPANNING:
						{
							iNumSplitter+=300;
						}break;
					}
				}
			}

			iScore= abs( iNumFront - iNumBack ) + (iNumSplitter);
			if( iScore < iBestScore )
			{
				iBestScore		= iScore;
				pSelectedPoly	= pSplitterPoly; 				
			}
		}
	}
	if( pSelectedPoly != NULL )
		pSelectedPoly->bBeenUsedAsSplitter = true;

	if( pSelectedPoly )
	{		
		SAFE_NEW( m_pSplitter, GPolygon );		
		ZeroMemory( m_pSplitter, sizeof( GPolygon ));
		SAFE_NEW_ARRAY( m_pSplitter->pVertexList, PNCT_VERTEX, pSelectedPoly->iNumVertices )		 
		memset( m_pSplitter->pVertexList, 0, sizeof( PNCT_VERTEX ) * pSelectedPoly->iNumVertices );
		memcpy( m_pSplitter->pVertexList, pSelectedPoly->pVertexList, sizeof(PNCT_VERTEX) * pSelectedPoly->iNumVertices); 
		SAFE_NEW_ARRAY( m_pSplitter->pIndices, DWORD, pSelectedPoly->iNumIndices )
		ZeroMemory( m_pSplitter->pIndices, sizeof( DWORD ) * pSelectedPoly->iNumIndices);
		memcpy( m_pSplitter->pIndices, pSelectedPoly->pIndices, sizeof(DWORD)*pSelectedPoly->iNumIndices); 
		m_pSplitter->bBeenUsedAsSplitter = pSelectedPoly->bBeenUsedAsSplitter;
		m_pSplitter->iNumIndices = pSelectedPoly->iNumIndices;
		m_pSplitter->iNumVertices = pSelectedPoly->iNumVertices;
		m_pSplitter->iTexIndex= pSelectedPoly->iTexIndex;
		m_pSplitter->vNormal = pSelectedPoly->vNormal;
		_tcscpy( m_pSplitter->strTexName, pSelectedPoly->strTexName);
		m_pSplitter->pNext = NULL;		
	}
	return m_pSplitter;
	
}
// �������� ��ü ������ ���Ҹ� ��, �� ��ġ�� 
G_POSITION GBspTree::ClassifyPoint( GPolygon* pPlane, D3DXVECTOR3* pPos )
{
	D3DXVECTOR3 *vec1	= (D3DXVECTOR3*)&pPlane->pVertexList[0];
	D3DXVECTOR3 vDirection = (*vec1) - (*pPos );
	float	fResult;
	fResult = D3DXVec3Dot( &vDirection, &pPlane->vNormal );
	if( fResult < 0.0f )
	{
		return P_FRONT;
	}
	if( fResult > 0.0f )
	{
		return P_BACK;
	}
	return P_ONPLANE;	
}

G_POSITION GBspTree::ClassifyPoly( GPolygon* pPlane, GPolygon* pPoly )
{
	D3DXVECTOR3 *vec1 = (D3DXVECTOR3*)&pPlane->pVertexList[0];
	DWORD dwInFront, dwBehind, dwOnPlane;	
	dwInFront = dwBehind = dwOnPlane = 0;

	float	fResult;
	for( DWORD dwVec = 0; dwVec < pPoly->iNumVertices; dwVec++ )
	{	
		D3DXVECTOR3 *vec2	= (D3DXVECTOR3*)&pPoly->pVertexList[dwVec];
		D3DXVECTOR3 vDirection = (*vec1) - (*vec2);
		// ���տ� ���� ������ ����� ���� ��� �ȴ�. 
		// �׷��� ���⼭�� D3DXVec3Dot()�� ����� �״�� ���� ������ �����
		// ��ȣ�� �ٲ��.
		fResult = D3DXVec3Dot( &vDirection, &pPlane->vNormal );
		if( fResult < 0.0f )
		{
			dwInFront++;
		}
		else if( fResult > 0.0f )
		{
			dwBehind++;
		}
		else
		{
			dwOnPlane++;			
		}			
	}	

	if( dwOnPlane == pPoly->iNumVertices )
	{
		// �������� ���� ��� �ִ� ��������� ���� �븻�� ���� �ֱ� ������
		// ������ ����� ������ �ݴ�� �Ͽ� ����ؾ� �Ѵ�.
		// ��� ���� ���Ҹ� ������ ���� ���� �븻�� ���⿡ ���� �յ��� ���Ѵ�.
		fResult = D3DXVec3Dot( &pPoly->vNormal, &pPlane->vNormal ) ;
		if( fResult >= 0.0f)
		{
			return P_FRONT;
		}
		else if( fResult < 0.0f)
		{
			return P_BACK;
		}
	}
	// �Ϻ��� ������ ��� �� ������ ��� ������ ���� ��ġ�� ���� ��ȯ��.
	if( dwInFront == pPoly->iNumVertices || (dwInFront > 0 && dwBehind == 0) ) 
	{
		return P_FRONT;
	}
	if( dwBehind == pPoly->iNumVertices ||	(dwBehind > 0 && dwInFront == 0) ) 
	{
		return P_BACK;
	}
	return P_SPANNING;
}


void GBspTree::SplitterPolygon( GPolygon* pPoly,				//  ���� ������ 
								GPolygon* pPlane,			//  ���Ҹ� 
								GPolygon* pFrontSplitter,	//  ���Ҹ� �տ� ��ġ�� ������ 
								GPolygon* pBackSplitter )	//  ���Ҹ� �ڿ� ��ġ�� ������ 
{	
	vector<PNCT_VERTEX>	FrontList;
	vector<PNCT_VERTEX>	BackList;
	D3DXVECTOR3				Intersection;	
	float					fT;	
	D3DXVECTOR3	vPlanVertex = pPlane->pVertexList[0].p;

	D3DXVECTOR3 PointA, PointB;
	for( DWORD dwVertex = 0; dwVertex < pPoly->iNumVertices ; dwVertex++ )
	{
		PNCT_VERTEX vVertex	= pPoly->pVertexList[dwVertex]; 	
		switch( ClassifyPoint( pPlane, &vVertex.p ) )
		{
			case P_ONPLANE:
				FrontList.push_back(vVertex);
				BackList.push_back(vVertex);					continue;	
			case P_FRONT:	FrontList.push_back(vVertex);		break;
			case P_BACK:	BackList.push_back(vVertex);		break;		
		}
		// Ray : Plane
		DWORD dwNextVertex = dwVertex+1;
		if( dwVertex == pPoly->iNumVertices-1 )	dwNextVertex = 0;

		PointA			= pPoly->pVertexList[dwVertex].p;
		PointB			= pPoly->pVertexList[dwNextVertex].p;	
		int iPointA		= ClassifyPoint( pPlane, &PointA  );
		int iPointB		= ClassifyPoint( pPlane, &PointB  );		
		// PointA �� PointA�� ���Ҹ鿡 ���� ����� �ٸ��� �ᱹ �����ִٴ� ���� �ȴ�.		
		if(	iPointA != iPointB && iPointB != P_ONPLANE )
		{
			if(	GetIntersection( PointA, PointB, vPlanVertex, pPlane->vNormal, Intersection, fT ) )
			{
				PNCT_VERTEX vNew;
				PNCT_VERTEX vE = pPoly->pVertexList[dwNextVertex];
				PNCT_VERTEX vS= pPoly->pVertexList[dwVertex];
				vNew.p = Intersection;
				D3DXVec2Lerp( &vNew.t, &vS.t, &vE.t, fT );
				D3DXVec4Lerp( &vNew.c, &vS.c, &vE.c, fT );
				D3DXVec3Lerp( &vNew.n, &vS.n, &vE.n, fT );					
				FrontList.push_back(vNew);
				BackList.push_back(vNew);
			}
		}		
	}
	
	SetPolygon(pFrontSplitter, pPoly, FrontList );
	SetPolygon(pBackSplitter, pPoly, BackList );
}
void GBspTree::SetPolygon( GPolygon* pSplitter, GPolygon* pPoly,vector<PNCT_VERTEX>&	VertexList)
{
	if( VertexList.size() )	
	{
		SAFE_NEW_ARRAY( pSplitter->pVertexList, PNCT_VERTEX, VertexList.size() )
		for( int iB = 0; iB < VertexList.size(); iB++ )
		{
			pSplitter->pVertexList[iB]	= VertexList[iB]; 
		}
		pSplitter->iNumVertices		= VertexList.size();
		pSplitter->iNumIndices			= (pSplitter->iNumVertices - 2) * 3;	
		pSplitter->bBeenUsedAsSplitter	= pPoly->bBeenUsedAsSplitter;	
		pSplitter->iTexIndex			= pPoly->iTexIndex;
		pSplitter->vNormal				= pPoly->vNormal;

		SAFE_NEW_ARRAY( pSplitter->pIndices, DWORD, pSplitter->iNumIndices )
	
		int iIndexA = 0;
		int iIndexB = 0;
		int iIndexC = 0;
		D3DXVECTOR3 vNormal;

		for( DWORD dwFace = 0 ; dwFace < pSplitter->iNumIndices / 3; dwFace++ )
		{	
			// Triangle List( Fan )
			iIndexB = dwFace + 1;
			iIndexC = dwFace + 2;
			D3DXVec3Cross(&vNormal,	&(pSplitter->pVertexList[iIndexB].p - pSplitter->pVertexList[iIndexA].p), 
									&(pSplitter->pVertexList[iIndexC].p - pSplitter->pVertexList[iIndexA].p));	
			D3DXVec3Normalize(&vNormal, &vNormal);	
			float fResult = D3DXVec3Dot( &vNormal, &pSplitter->vNormal );
			if( fResult < 0.0f)
			{		
					int temp; 	
					temp	= iIndexB;	
					iIndexB = iIndexC;	
					iIndexC = iIndexB;
			}	
			pSplitter->pIndices[ dwFace * 3 + 0 ]	= iIndexA;
			pSplitter->pIndices[ dwFace * 3 + 1 ]	= iIndexB;
			pSplitter->pIndices[ dwFace * 3 + 2 ]	= iIndexC;		
		}		
	}	
}
bool GBspTree::GetIntersection( D3DXVECTOR3& vStart, D3DXVECTOR3& vEnd , 
							    D3DXVECTOR3& vPlaneVertex, 
								D3DXVECTOR3& vPlaneNormal, 
								D3DXVECTOR3& vIntersection,
								float&	fPercentage )
{
	//D3DXVECTOR3 L1 = vStart - vPlaneVertex;
	//D3DXVECTOR3 L2 = vStart - vEnd;
	//float f0,f1;
	//f0 = D3DXVec3Dot( &L1, &vPlaneNormal );
	//f1 = D3DXVec3Dot( &L2, &vPlaneNormal );	
	//fPercentage = f0 / f1;//(L1 | N) / (L2 | N);
	//if( 0.0f<= fPercentage && fPercentage <= 1.0f )
	//{
	//	vIntersection = vStart - L2 * fPercentage;			
	//	//return true;		
	//}
	////return false;

	D3DXVECTOR3 vDirection;
	float		fSRcDist, fDestDist;
	vDirection = vStart - vEnd;
	fSRcDist  = D3DXVec3Dot( &vDirection, &vPlaneNormal );
	fDestDist = D3DXVec3Dot( &( vStart - vPlaneVertex), &vPlaneNormal );
	fPercentage = fDestDist / fSRcDist;
	if( 0.0f<= fPercentage && fPercentage <= 1.0f )
	{
		vIntersection = vStart - vDirection * fPercentage;			
		return true;		
	}
	return false;
}

GPolygon* GBspTree::GetCollisionPoly( GPolygon* pPoly )
{
	pPoly = m_pCollisionPoly;
	return pPoly;
}


void GBspTree::SetMatrix( D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj )
{
	m_BspRender.SetMatrix( pWorld, pView, pProj );
}

int GBspTree::AddObject( GBaseObj* pObj )
{
	if( CheckRect( m_pRootNode, pObj ) )
	{
		GNode* pNode = FindNode( m_pRootNode, pObj );	
		if( pNode )
		{
			pNode->m_ObjectList.push_back( pObj );
		}
		return 1;
	}
	return 0;
}
int GBspTree::CheckRect( GNode* pNode, GBaseObj* pObj )
{
	if( pNode->m_tBox.vMin.x <= pObj->m_tBox.vMin.x && pNode->m_tBox.vMax.x >= pObj->m_tBox.vMax.x )
	{
		if( pNode->m_tBox.vMin.y <= pObj->m_tBox.vMin.y && pNode->m_tBox.vMax.y >= pObj->m_tBox.vMax.y )
		{
			if( pNode->m_tBox.vMin.z <= pObj->m_tBox.vMin.z && pNode->m_tBox.vMax.z >= pObj->m_tBox.vMax.z )
			{	
				return 1;
			}			
		}
	}
	return 0;
}
GNode*	GBspTree::FindNode(GNode* pNode, GBaseObj* pObj  )
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
		m_QuadTreeQueue.pop();		
	}while( pNode );
	return pNode;
}
void GBspTree::VisibleNode( GNode* pNode )
{	
	if( !pNode ) return;
	if( m_pCamera->CheckOBBInPlane( &pNode->m_tBox ) )	
	{
		if( pNode->m_isLeaf== TRUE )
		{
			m_DrawNodeList.push_back( pNode );			
		}
		for( int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++ )
		{
			if( m_pCamera->CheckOBBInPlane( &pNode->m_ObjectList[iObj]->m_tBox ) )	
			{
				m_DrawObjList.push_back( pNode->m_ObjectList[iObj] );			
			}
		}		
	}
	for( int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++ )
	{		
		VisibleNode( pNode->m_ChildList[iNode] );
	}
}

GBspTree::GBspTree()
{
	m_pRootNode		=	NULL;	
	m_pCollisionPoly=   NULL; 
	m_vCurrentPosition = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
}
GBspTree::~GBspTree()
{
}
