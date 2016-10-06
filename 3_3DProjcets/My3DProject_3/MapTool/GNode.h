#pragma once
#include "GBaseObj.h"
#include "TCamera.h"

struct NodeMtrlInfo
{
	int		iMtrl;	// �ؽ��� �ε���
	int		iNumFaces; // ���̽� ����
	int		iNumVertex;// ������ ����
};

struct GPolygon
{
	int				iNumVertices;		// ���� ����
	int				iNumIndices;		// �ε��� ����	
	int				iTexIndex;			// �ؽ��� �ε���
	bool			bBeenUsedAsSplitter;// ���Ҹ� ��뿩�� �Ǵ�  	
	D3DXVECTOR3		vNormal;			// ��� �븻	
	PNCT_VERTEX*	pVertexList;		// ���� ������ 
	TCHAR			strTexName[30];
	DWORD			*pIndices;			// �ε��� ������	
	GPolygon		*pNext;				// ���� ����Ʈ	
	virtual ~GPolygon()
	{
		SAFE_DEL(pIndices);
		SAFE_DEL(pVertexList);
	}
};

class GNode
{
public:
	DWORD				m_dwDepth;
	BOOL				m_isLeaf;
	BOOL				m_isTile;
	DWORD				m_dwLodLevel;
	DWORD				m_dwLodType;
	// �ٿ�� �ڽ�
	T_BOX				m_tBox;
	vector<DWORD>		m_CornerIndex;
	// ����� �ڳ� ���� �ε��� �� ��ġ
	vector<D3DXVECTOR3>	m_CornerList;
	vector<GNode*>		m_ChildList;
	vector<GNode*>		m_NeighborList;
	vector<GBaseObj*>	m_ObjectList;    //�� �ִ°���? Box�� ���� ���·� vector<T_BOX>
	// �߰�
	DWORD				m_dwPositionIndex[2];// X,Y	

	D3D11_BOX			m_BoxVB;
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector <PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;

	// bsp
	DWORD				m_dwIndex;
	vector<ComPtr<ID3D11Buffer>>	m_pVBList;
	vector<ComPtr<ID3D11Buffer>>	m_pIBList;
	int					iNumPolygon;
	int					iNumMtrl;	// ������������ ��� ����ϴ� �� �ؽ����� ����
	vector<NodeMtrlInfo> m_MtrlList;
	GPolygon			*m_pSplitter; // Splitter poly for this node
public:
	GNode();
	virtual ~GNode();
};

