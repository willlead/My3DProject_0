#pragma once
#include "TMesh.h"

typedef TMesh<PNC_VERTEX>				tMatMesh;
typedef vector<shared_ptr<tMatMesh>>	tMatMeshList;

class TBoneObj : public TModel
{
public:
	tMatMeshList				m_pMesh;
	VersionMark					m_VerInfo;
	FLOAT						m_fLerpTime;
	int							m_iCurrentFrame;
	D3DXMATRIX*					m_pMatrix;
	D3DXMATRIX**				m_ppAniMatrix;
	D3DXQUATERNION**			m_ppAniQuater;
	D3DXVECTOR3**				m_ppScaleVector;
	D3DXVECTOR3**				m_ppTransVector;
	ID3D11Buffer*               m_pBoneBuffer;
	ID3D11ShaderResourceView*	m_pBoneBufferRV;
public:
	// ��� ���� �� ���ϸ��̼� ����
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext);
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext, D3DXMATRIX* pMatrix);
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext, D3DXMATRIX* pMatrix, vector<D3DXMATRIX>* pList);
	bool		AniFrame(FLOAT fCurFrame, FLOAT fElapsedTime,
		int iFirstFrame, int iLastFrame,
		D3DXMATRIX* pMatrix);
public:
	bool		Init();
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Release();
	bool		Load(	ID3D11Device* pDevice, 
								const TCHAR* szLoadName, 
								const TCHAR* pLoadShaderFile, 
								bool bThread = false);
	bool		Convert(const TCHAR* strPathName = 0);
public:
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	HRESULT		SetInputLayout();
	bool		SetBuffer(ID3D11Device* pd3dDevice);
	void		SetBoundBox(D3DXVECTOR3* Quad, tMatMesh* pMesh);
	bool		UpdateBuffer();
	bool		CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB);
	bool		Draw(ID3D11DeviceContext*    pContext, TModel* pParent);
public:
	TBoneObj(void);
	~TBoneObj(void);
};
