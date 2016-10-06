#pragma once
#include "TCore.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>
//#include <xnamath.h>  // d3dmath�� �������̴ϱ� ����� ���� 
//#include <winerror.h>
class Sample : public TCore
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};
