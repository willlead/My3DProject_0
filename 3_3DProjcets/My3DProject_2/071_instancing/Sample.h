#pragma once
#include "TCore.h"
#include "TModelViewCamera.h"
#include <memory>	
#include "TObject2D.h"
//#include "TPointParticle.h"
class Sample : public TCore
{
public:
	shared_ptr<TModelViewCamera> m_pMainCamera;
	shared_ptr<TObject2D> m_pObj2D[2];
	D3DXMATRIX	m_matWorld;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	Sample();
	~Sample();
};

