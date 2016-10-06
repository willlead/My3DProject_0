#pragma once
#include "GNode.h"
typedef vector<GNode*> GNODE_VECTOR;

class GMapLOD
{
public:
	ID3D11Device*	m_pd3dDevice;
	TCamera*		m_pCamera;
	vector<GNODE_VECTOR>		m_LevelList;
	vector<GNode*>	m_DrawPatchNodeList;
	vector<GNode*>	m_DrawNodeList;
	float m_fExpansionRatio;
	int m_iNumCell;
	int m_iPatchLodCount;
	bool m_bThresHoldValue;
	void SetThresHold(bool bFlag) { m_bThresHoldValue = bFlag; };
public:
	virtual void	InitLevelOrder(GNode* pRootNode, int iMaxDepth);
	virtual void	SetLOD(DWORD dwWidth, int iNumDepth);
	virtual void	SetExpansionRatio(int iRatio) { m_fExpansionRatio = iRatio; }
	virtual DWORD	GetLodSubIndex(GNode* pNode);
	virtual DWORD	GetLodType(GNode* pNode);
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter, float fRadius);
	virtual float	GetExpansionRatio(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin);
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter);
	virtual void	GetDrawPatchNode();
	virtual void	AddDrawPatchNode(GNode* pNode);
	virtual void	Update(ID3D11Device* pd3dDevice, TCamera* pCamera = NULL);
public:
	GMapLOD();
	virtual ~GMapLOD();
};

