#pragma once
#include "TBaseObj.h"
#include "TCamera.h"

struct GNodeMtrlInfo
{
	int iMtrl;
	int iNumFaces;
	int iNumVertex;
};

struct GPolygon
{
	int				iNumVertices;			// 정점 개수
	int				iNumIndices;			// 인덱스 개수	
	int				iTexIndex;				// 텍스쳐 인덱스
	bool			bBeenUsedAsSplitter;	// 분할면 사용여부 판단  	
	D3DXVECTOR3		vNormal;				// 평면 노말	
	PNCT_VERTEX*	pVertexList;			// 정점 데이터 
	TCHAR			strTexName[30];
	DWORD			*pIndices;				// 인덱스 데이터	
	GPolygon		*pNext;					// 연결 리스트	
	virtual ~GPolygon()
	{
		SAFE_DEL(pIndices);
		SAFE_DEL(pVertexList);
	}
};

class GNode
{
public:
	DWORD	m_dwDepth;
	BOOL	m_isLeaf;
	BOOL	m_isTile;
	DWORD	m_dwLodLevel;
	DWORD	m_dwLodType;
	 
	T_BOX	m_tBox;
	vector<DWORD> m_CornerIndex;

	vector<D3DXVECTOR3>	m_CornerList;
	vector<GNode*>		m_ChildList;
	vector<GNode*>		m_NeighborList;
	vector<TBaseObj*>	m_ObjectList;

	DWORD m_dwPositionIndex[2];

	D3D11_BOX m_BoxVB;
	D3D11_BOX m_BoxIB;
	UINT m_iBeginVB;
	UINT m_iBeginIB;
	vector<DWORD> m_IndexList;
	vector<PNCT_VERTEX> m_VertexList;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	DWORD m_dwIndex;
	vector<ComPtr<ID3D11Buffer>> m_pVBList;
	vector<ComPtr<ID3D11Buffer>> m_pIBList;
	int iNumPolygon;
	int iNumMtrl;
	vector<GNodeMtrlInfo> m_MtrlList;
	GPolygon *m_pSplitter;
public:
	GNode();
	virtual ~GNode();
};


