#pragma once
#include "TModel.h"
class TShape : public TModel
{
public:
	TShape() {};
	virtual ~TShape(void) {};
};
class TLineShape : public TShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT				SetInputLayout();
	bool				CreateVertexData();
	bool				CreateIndexData();
	HRESULT				CreateVertexBuffer();
	HRESULT				CreateResource();
	bool				Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 dwColor);
public:
	TLineShape(void);
	virtual ~TLineShape(void);
};

class TDirectionLineShape : public TShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT			SetInputLayout();
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateResource();
public:
	TDirectionLineShape(void);
	virtual ~TDirectionLineShape(void);
};

class TBoxShape : public TShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateResource();
public:
	TBoxShape(void);
	virtual ~TBoxShape(void);
};

class TPlaneShape : public TShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateResource();
public:
	TPlaneShape(void);
	virtual ~TPlaneShape(void);
};
class TSphereShape : public TShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			Render(ID3D11DeviceContext*	pImmediateContext);
	HRESULT			CreateResource();
	void CreateSphere(UINT Slices = 20, float fRadius = 1.0f);
	void MakeSpherePoint(std::vector<D3DXVECTOR3> &spherePoints,
		UINT Slices = 20, double r = 1.0f,
		D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0));
public:
	TSphereShape(void);
	virtual ~TSphereShape(void);
};