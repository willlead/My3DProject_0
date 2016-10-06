#pragma once
#include <tchar.h>
#include "TObjStd.h"

class TModel
{
public:
	ID3D11Device*				m_pd3dDevice;
	ID3D11DeviceContext*		m_pContext;
	DX::TDxObject				m_Object;
	VS_CONSTANT_BUFFER			m_cbData;
	D3DXVECTOR3					m_vCenterPos;
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matView;
	D3DXMATRIX					m_matProj;
	vector<DWORD>				m_IndexList;
	vector<PNCT_VERTEX>			m_VertexList;
	TScene				m_Scene;
	FILETYPE			m_ObjType;
	float				m_fElapseTime;
	int					m_iStartFrame;
	int					m_iLastFrame;

	D3DXMATRIX			m_matControlWorld;
	T_STR				m_szDirName;
	T_STR				m_szName;
	///  Buffer Comine 
	UINT				m_iMaxVertex;
	UINT				m_iMaxIndex;
	UINT				m_iBeginPosVB;
	UINT				m_iBeginPosIB;
	bool				m_bOptimization;

	int					m_iMatrixIndex;
public:
	ID3D11Buffer*				GetVB();
	ID3D11Buffer*				GetIB();
	virtual void				SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	virtual void				SetAmbientColor(float fR, float fG, float fB, float fA);
	virtual bool				Convert(ID3D11Device* pDevice);
	virtual bool				Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	virtual HRESULT				LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile);
	virtual HRESULT				SetInputLayout();
	virtual bool				CreateVertexData();
	virtual bool				CreateIndexData();
	virtual HRESULT				CreateVertexBuffer();
	virtual HRESULT				CreateIndexBuffer();
	virtual HRESULT				CreateConstantBuffer();
	virtual HRESULT				LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);
	virtual HRESULT				CreateResource();
	virtual HRESULT				DeleteResource();
	virtual bool				UpdateBuffer();
	virtual bool				Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString = 0);
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
	virtual bool				Render(ID3D11DeviceContext*		pContext);
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual bool				Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 dwColor);
	virtual bool				Release();
	virtual void				SetMatrixIndex(int iMatrixIndex) { m_iMatrixIndex = iMatrixIndex; }
public:
	TModel(void);
	virtual ~TModel(void);
};