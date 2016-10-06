#pragma once
#include "TMesh.h"
#include "TParser.h"

typedef TMesh<PNCT_VERTEX>			tTbsMesh;
typedef vector<shared_ptr<tTbsMesh>>	tTbsMeshList;

class TTbsObj : public TModel
{
public:	
	tTbsMeshList	m_pMesh;
	TParser			m_Parser;
	vector<TMtrl>	m_Material;	
	DWORD			m_dwNumChildren;
public:
	bool		Init();
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Draw(ID3D11DeviceContext*    pContext, TModel* pParent);
	bool		Release();
	bool		ResetResource();
	////////////////////////////////////////////////
	//  ������Ʈ �ε� ���� �Լ�
	////////////////////////////////////////////////
	bool		Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	//  �� ���� �ε�
	bool		LoadScene( const TCHAR* strFileName );
	//  ���͸��� ���� �ε�
	bool		LoadMaterial();
	void		LoadMaterialHeader(TMtrl* pMtrl);
	void		LoadTexMap(	TMtrl* pMtrl,T_STR szDirName );
	//  ������Ʈ ���� �ε�
	bool		LoadObject();
	int			LoadVertexIndex(tTbsMesh* pMesh);
	//  ���ϸ��̼� ���� �ε�
	void		LoadAnimation(tTbsMesh* pMesh);
	bool		LoadAnimationTrack( int iNumTrack, vector<shared_ptr<TAnimTrack>>& pTrackList );
	//--------------------------------------------------------------------------------------
	// ������ ���� ���� ����
	//--------------------------------------------------------------------------------------
	// �ε� �� ������ TMesh�� ��ȯ
	bool		Convert(ID3D11Device* pd3dDevice);
	// ���͸��� ����
	bool		SetMaterial();
	int			GetMapID( TMtrl* pMtrl, int iTexMapType=ID_TBASIS_DI ); // �� �ε��� �˻�
    // ��Ӱ��� ����
	bool		InheriteCollect();
	tTbsMesh*	SearchToCollects(T_STR	m_strParentName );	
	//--------------------------------------------------------------------------------------
	// ���ϸ��̼� ���� ���� �Լ� 
	//--------------------------------------------------------------------------------------
	TAnimTrack*	SetDoublyLinkedList( TAnimTrack* pCurrentTrack, TAnimTrack* pPrev );//���� ���� ����Ʈ ����
	bool		GetAnimationTrack(	float fFrame,vector<shared_ptr<TAnimTrack>> pTrackList,
									TAnimTrack** ppStartTrack, 
									TAnimTrack** ppEndTrack );
	void		Interpolate(tTbsMesh* pMesh,D3DXMATRIX* matParent, float fFrameTick );
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	HRESULT		SetInputLayout();
	bool		UpdateBuffer();
	bool		CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB);
public:
	TTbsObj(void);
	virtual ~TTbsObj(void);
};
