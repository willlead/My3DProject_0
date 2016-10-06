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
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	Sample();
	~Sample();
};

