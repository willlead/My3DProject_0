#pragma once
#include "GMapLOD.h"
#include <queue>

class GQuadTree : public GMapLOD
{
public:
	enum QUADTREECORNER { tTL = 0, tTR = 1, tBL = 2, tBR = 3, };
	int				m_iRenderDepth;
	void			SetRenderDepth(DWORD dwDepth);
	GNode*					m_pRootNode;
	queue<GNode*>			m_QuadTreeQueue;
	vector<TBaseObj*>		m_DrawObjList;
	bool			m_bUsedIndexList;		// ���� �ε��� ����Ͽ� ��������	
	int				m_iMaxDepthLimit;		// ��� ������ �ִ� �ѵ�
	void			SetMaxDepthLimit(int iMaxDepth) { m_iMaxDepthLimit = iMaxDepth; }
	int				m_iMaxDepth;			// �� ���� ����� ����( ���� ) ����
	float			m_fWidth;
	float			m_fHeight;
	// �ּ�	���� ũ��
	float			m_fMinDivideSize;
	void			SetMinDivideSize(int iMinDivideSize) { m_fMinDivideSize = iMinDivideSize; }
public:
	bool			Build(float fWidth, float fHeight);
	virtual bool	BuildTree(GNode* pNode);
	virtual bool	SubDivide(GNode* pNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	GNode*			CreateNode(GNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight);

	int				AddObject(TBaseObj* pObj);
	GNode*			FindNode(GNode* pNode, TBaseObj* pObj);
	int				CheckRect(GNode* pNode, TBaseObj* pObj);

	void			VisibleNode(GNode* pNode);
	void			VisibleObject(GNode* pNode);
	virtual void	DrawFindNode(GNode* pNode);

	DWORD			CheckSize(DWORD dwSize);
	void			SetNeighborNode(GNode* pNode);
	void			FindNeighborNode(GNode* pNode);
public:
	GQuadTree();
	virtual ~GQuadTree();
};

