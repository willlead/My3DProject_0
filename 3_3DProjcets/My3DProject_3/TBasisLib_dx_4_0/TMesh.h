#pragma once
#include "TModel.h"
using namespace DX;

template< class T > struct TTri
{
	T						vVertex[3];
	D3DXVECTOR3  vNormal;
	int						iSubIndex;
	TTri(int Index) : iSubIndex(Index) {}
	TTri() : iSubIndex(-1) {}
};
template< class T > struct DescendingTriSort
{
	bool operator()(TTri<T>& rpStart, TTri<T>& rpEnd)
	{
		return rpStart.iSubIndex < rpEnd.iSubIndex;
	}
};
template< class T > struct IsTriangleSame
{
	TTri<T>			m_Tri;
	IsTriangleSame(TTri<T> data) : m_Tri(data) {}
	bool operator()(TTri<T>& value)
	{
		return value.iSubIndex == m_Tri.iSubIndex;
	}
};

template <class T >
class TMesh : public TModel
{
public:
	T_STR					m_strNodeName;
	T_STR					m_strParentName;
	OBJECTCLASSTYPE			m_ClassType;
	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matInverse;
	D3DXMATRIX				m_matWorldTrans;
	D3DXMATRIX				m_matCalculation;
	D3DXMATRIX				m_matWorldRotate;
	D3DXMATRIX				m_matWorldScale;
	bool					m_bUsed;
	int						m_iNumFace;
	int						m_iDiffuseTex;
	T_BOX					m_BBox;
	vector<T>			m_VertexArray;
	vector<TTri<T>>		m_TriList;
	TMesh*				m_pParent;
	vector<DWORD>		m_IndexArray;
	vector<shared_ptr<TMesh<T>>>		m_pSubMesh;
	vector<TMesh*>		m_pChildMesh;
	vector<shared_ptr<TAnimTrack>>		m_pPosTrack;
	vector<shared_ptr<TAnimTrack>>		m_pRotTrack;
	vector<shared_ptr<TAnimTrack>>		m_pSclTrack;
	vector<shared_ptr<TAnimTrack>>		m_pVisTrack;
	float					m_fVisibility;

	//TBS
	int   m_iNumTrack[4];
	int   m_iMtrlRef;

	// skm
	// 텍스쳐 정보( 텍스쳐 맵 단위 )	
	int						m_iNumMtrl;
	int						m_iNumVertexs;
	T*						m_pDrawVertex;
	int						m_iNumTex;
	int						m_iTexType[13];
	TCHAR					m_szTexName[13][30];
public:
	int IsEqulVertexList(vector<T>&  VertexArray, T& Vertex);
	int SetUniqueBuffer(vector <TTri<T>>& TriList, int iMtrl, int iStartTri);
public:
	TMesh(void)
	{
		m_pDrawVertex = NULL;
		m_iNumFace = 0;
		m_iDiffuseTex = -1;
		m_pParent = 0;
		m_iNumVertexs = 0;
		m_iNumMtrl = 0;
		D3DXMatrixIdentity(&m_matWorldTrans);
		D3DXMatrixIdentity(&m_matWorldRotate);
		D3DXMatrixIdentity(&m_matWorldScale);
		D3DXMatrixIdentity(&m_matCalculation);
	};;
	virtual ~TMesh(void) {};
};

template <class T >
int TMesh<T>::IsEqulVertexList(vector<T>&  VertexArray, T& Vertex)
{
	for (int iVertex = 0; iVertex < VertexArray.size(); iVertex++)
	{
		if (VertexArray[iVertex] == Vertex)
		{
			return iVertex;
		}
	}
	return -1;
}

template <class T >
int TMesh<T>::SetUniqueBuffer(vector <TTri<T>>& TriList, int iMtrl, int iStartTri)
{
	int iNumFaces = TriList.size();
	m_VertexArray.reserve(iNumFaces * 3);
	m_IndexArray.reserve(iNumFaces * 3);

	TTri<T>  triSame(iMtrl);
	if (iMtrl >= 0)
	{
		iNumFaces = count_if(TriList.begin(), TriList.end(), IsTriangleSame<T>(triSame));
	}

	for (int iFace = 0; iFace < iNumFaces; iFace++)
	{
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			int iPosReturn = IsEqulVertexList(m_VertexArray, TriList[iStartTri + iFace].vVertex[iCnt]);
			if (iPosReturn < 0)
			{
				m_VertexArray.push_back(TriList[iStartTri + iFace].vVertex[iCnt]);
				iPosReturn = m_VertexArray.size() - 1;
			}
			m_IndexArray.push_back(iPosReturn);
		}
	}

	m_iNumFace = iNumFaces;
	return iNumFaces;
}
