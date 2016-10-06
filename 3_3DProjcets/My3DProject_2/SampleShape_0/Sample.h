#pragma once
#include "Tcore.h"
#include "TShape.h"
#include <memory>
#define MAX_CNT 3
class Sample : public TCore
{
public:
	shared_ptr<TShape> m_pShape[MAX_CNT];
	TLineShape   m_Line;
	D3DXMATRIX   m_matWorld[MAX_CNT];
	D3DXMATRIX   m_matView;
	D3DXMATRIX   m_matProj;
	D3DXVECTOR3  m_vStart;
	D3DXVECTOR3  m_vEnd;
	D3DXVECTOR4  m_vColor;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	Sample();
	virtual ~Sample();
};

