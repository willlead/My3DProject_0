#pragma once
#include "TMesh.h"
#include "TAseParser.h"

typedef TMesh<PNCT_VERTEX>			tAseMesh;
typedef vector<shared_ptr<tAseMesh>>	tAseMeshList;

class TAseObj : public TModel
{
public:
	tAseMeshList	m_pMesh;
	TAseParser		m_Parser;
	vector<TMtrl>	m_Material;
public:
	bool					Init();
	bool					Frame();
	bool					Render(ID3D11DeviceContext*    pContext);
	bool					Release();
	bool					ResetResource();
	bool					Convert(ID3D11Device* pDevice);
	bool					Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread=false);
	void					SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	HRESULT					SetInputLayout();
	HRESULT					LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);
	void					SetTriangleBuffer(tAseMesh* pMesh,TAseMesh* pAseMesh,DWORD dwMask = 0x00000000);
	int						GetMapID(TMtrl* pMtrl, int iTexMapType = ID_TBASIS_DI);
	//--------------------------------------------------------------------------------------																					
	// ���ϸ��̼� ���� �Լ� 																					
	//--------------------------------------------------------------------------------------
	TAnimTrack*				SetDoublyLinkedList(TAnimTrack* pCurrentTrack, TAnimTrack* pPrev);//���� ���� ����Ʈ ����
	bool					GetAnimationTrack(float fFrame,	vector<shared_ptr<TAnimTrack>> pTrackList,	TAnimTrack** ppStartTrack,	TAnimTrack** ppEndTrack);
	void					Interpolate(tAseMesh* pMesh,	D3DXMATRIX* matParent,	float fFrameTick);
	bool					InheriteCollect();
	tAseMesh*	SearchToCollects(T_STR	m_strParentName);
	bool		UpdateBuffer();
	bool		CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB);
	bool		Draw(ID3D11DeviceContext*    pContext, TModel* pParent);
public:
	TAseObj(void);
	virtual ~TAseObj(void);
};
