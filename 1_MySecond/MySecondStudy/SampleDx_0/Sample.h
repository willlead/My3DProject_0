#pragma once
#include "TCore.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>
//#include <xnamath.h>  // d3dmath를 직관적이니까 사용할 것임 
//#include <winerror.h>
class Sample : public TCore
{
//public:
//	D3DXVECTOR3 Eye;
//	D3DXVECTOR3 At;
//	D3DXVECTOR3 Up;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};

