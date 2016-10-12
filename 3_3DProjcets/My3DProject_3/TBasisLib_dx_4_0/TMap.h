#pragma once
#include "TShape.h"
using namespace DX;

#define RANDOM(n)			(rand() % (n))
#define RANDOMSCOPE(s,e)	(rand() %(e-s))+s;
#define RANDOMIZE()			srand((unsigned)time(NULL))

struct TMapDesc
{
	int				iNumCols;
	int				iNumRows;
	float			fSellDistance;	
	float			fScaleHeight;
	T_STR			strTextureFile;
	T_STR			strShaderFile;	
};

class TMap : public TModel
{
public:
	TMapDesc				m_MapDesc;
	vector<PNCT_VERTEX>		m_VertexList;
	DWORD					m_dwShaderIndex;
	int						m_iNumVertices;
	int						m_iNumFace;
	int						m_iDiffuseTex;
	int						m_iNumCols;
	int						m_iNumRows;
	int						m_iNumSellCols;
	int						m_iNumSellRows;
	float					m_fSellDistance;
	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matView;
	D3DXMATRIX				m_matProj;
	D3DXMATRIX				m_matNormal;
	D3DXVECTOR3				m_vLightDir;
	int*					m_pNormalLookupTable;
	D3DXVECTOR3*			m_pFaceNormals;
	bool					m_bStaticLight;
public:
	virtual HRESULT		SetInputLayout();
	virtual bool		ReLoadVBuffer();
	virtual bool		ReLoadIBuffer();
	virtual void		CalcVertexColor(D3DXVECTOR3 vLightDir);
	virtual float		GetHeightOfVertex(UINT Index);
	virtual D3DXVECTOR3 GetNormalOfVertex(UINT Index);
	virtual D3DXVECTOR4 GetColorOfVertex(UINT Index);
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual bool		CreateVertexList();
	virtual bool		CreateIndexList();
	virtual bool		CreateMap(TMapDesc& MapDesc);
	virtual bool		Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext*  pImmediateContext);
	virtual bool		Load(TMapDesc& MapDesc);
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext*  pContext);
	virtual bool		Release();
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	virtual HRESULT		CreateResource();
	virtual HRESULT		DeleteResource();
	//--------------------------------------------------------------------------------------
	// ���� �븻 ���긦 ���� �ʱ�ȭ �Լ� �� ���̽� �븻 ���
	//--------------------------------------------------------------------------------------	
	void				InitFaceNormals();
	virtual void		CalcFaceNormals();
	D3DXVECTOR3			ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
	//--------------------------------------------------------------------------------------
	// ���� �븻 ��� ���̺� ���� �� ���
	//--------------------------------------------------------------------------------------	
	void				GenNormalLookupTable();
	virtual void		CalcPerVertexNormalsFastLookup();

	DXGI_FORMAT			MAKE_TYPELESS(DXGI_FORMAT format);
	DXGI_FORMAT			MAKE_SRGB(DXGI_FORMAT format);
public:
	TMap(void);
	virtual ~TMap(void);
};
