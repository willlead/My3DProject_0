#pragma once
#include <tchar.h>
#include "TDxHelperEx.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct T_PLANE
{
	float	fA, fB, fC, fD;
	bool	CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	bool	CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v0);
	void	Normalize()
	{
		float fMag = sqrt(fA*fA + fB*fB + fC*fC);
		fA = fA / fMag;
		fB = fB / fMag;
		fC = fC / fMag;
		fD = fD / fMag;
	}

};
struct T_SPHERE
{
	D3DXVECTOR3		vCenter;
	float			fRadius;
};
struct T_BOX
{
	// Common
	D3DXVECTOR3		vCenter;
	D3DXVECTOR3		vPos[8];
	// AABB
	D3DXVECTOR3		vMax;
	D3DXVECTOR3		vMin;
	// OBB
	D3DXVECTOR3		vAxis[3];
	float			fExtent[3];
};
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct PCT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
	bool operator == (const PCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && c == Vertex.c    && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PCT_VERTEX() {}
	PCT_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt)
	{
		p = vp, c = vc, t = vt;
	}
};
struct PCT2_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t0;
	D3DXVECTOR2 t1;
	bool operator == (const PCT2_VERTEX & Vertex)
	{
		if (p == Vertex.p  && c == Vertex.c    && t0 == Vertex.t0 && t1 == Vertex.t1)
		{
			return true;
		}
		return  false;
	}
	PCT2_VERTEX() {}
	PCT2_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt0,
		D3DXVECTOR2     vt1)
	{
		p = vp, c = vc, t0 = vt0, t1 = vt1;
	}
};
struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	bool operator == (const PC_VERTEX & Vertex)
	{
		if (p == Vertex.p  && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PC_VERTEX() {}
	PC_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR4		vc)
	{
		p = vp, c = vc;
	}
};
struct PNC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	bool operator == (const PNC_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PNC_VERTEX() {}
	PNC_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc)
	{
		p = vp, n = vn, c = vc;
	}
};
struct PNCT_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR4		c;
	D3DXVECTOR2     t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};
struct VS_CONSTANT_BUFFER
{
	D3DXMATRIX matWorld;// c0						
	D3DXMATRIX matView;	// c4						
	D3DXMATRIX matProj;	// c8						
	D3DXVECTOR4 Color; // 12
};
class TShape
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
public:
	ID3D11Buffer*				GetVB();
	ID3D11Buffer*				GetIB();
	void						SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	void						SetAmbientColor(float fR, float fG, float fB, float fA);
	virtual HRESULT				LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile   );
	virtual HRESULT				SetInputLayout();
	virtual HRESULT				CreateVertexBuffer();
	virtual HRESULT				CreateIndexBuffer();
	virtual HRESULT				CreateConstantBuffer();
	virtual HRESULT				LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);
	virtual HRESULT				CreateResource();
	virtual HRESULT				DeleteResource();
	virtual bool				Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString = 0);
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
	virtual bool				Render(ID3D11DeviceContext*		pContext);
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual bool				Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 dwColor);
	virtual bool				Release();
public:
	TShape(void);
	virtual ~TShape(void);
};

class TLineShape : public TShape
{
public:
	vector<PC_VERTEX>					m_VertexList;
public:
	HRESULT				SetInputLayout();
	HRESULT				CreateVertexBuffer();
	HRESULT				CreateIndexBuffer();
	HRESULT				CreateResource();
	bool				Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 dwColor);
public:
	TLineShape(void);
	virtual ~TLineShape(void);
};

class TDirectionLineShape : public TShape
{
public:
	vector<PC_VERTEX>					m_VertexList;
public:
	HRESULT			SetInputLayout();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateIndexBuffer();
	HRESULT			CreateResource();
public:
	TDirectionLineShape(void);
	virtual ~TDirectionLineShape(void);
};

class TBoxShape : public TShape
{
public:
	vector<PNCT_VERTEX>		m_VertexList;
public:
	HRESULT			SetInputLayout();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateIndexBuffer();
	HRESULT			CreateResource();
public:
	TBoxShape(void);
	virtual ~TBoxShape(void);
};

class TPlaneShape : public TShape
{
public:
	vector<PCT_VERTEX>		m_VertexList;
public:
	HRESULT			SetInputLayout();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateIndexBuffer();
	HRESULT			CreateResource();
public:
	TPlaneShape(void);
	virtual ~TPlaneShape(void);
};