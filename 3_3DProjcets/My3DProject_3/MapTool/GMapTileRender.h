#pragma once
#include "GQuadTree.h"
#include "TMap.h"

class GMapTileRender : public GQuadTree
{
public:
	TMap*					m_pMap;
	PNCT_VERTEX*			m_pVertexList;
	vector<DWORD>			m_IndexList;
	ComPtr<ID3D11Buffer>	m_pTileIndexBuffer;
	int						m_iNumFace;
	int						m_iMinSplitDistance;	// �ּ� ���� ũ��( ���� �� ���� )
	DWORD					m_dwWidth;
	DWORD					m_dwHeight;
	bool					m_bSharedVertexBuffer;	// ���� ���� ���� ���
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
	size_t m_dstibOffset;
	size_t m_ibOffset;
public:
	bool					Build(TMap* pMap, DWORD dwWidth, DWORD dwHeight);
	GNode*					CreateNode(GNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	D3DXVECTOR2				GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	void					ComputeBoungigBox(GNode* pNode);
	void					UpdateBoundingBox(GNode* pNode);
	bool					SubDivide(GNode* pNode);
	bool					Release();
	bool					Render(ID3D11DeviceContext*    pContext);
	bool					CreateVertexList(GNode* pNode);
	int						CreateIndexList(GNode* pNode);
public:
	GMapTileRender();
	virtual ~GMapTileRender();
};

