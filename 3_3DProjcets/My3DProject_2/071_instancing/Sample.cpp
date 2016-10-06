#include "Sample.h"

bool		Sample::Init()
{
	srand(timeGetTime());

	m_pObj2D[0] = make_shared<TObject2D>();
	m_pObj2D[0]->SetCounter(1000);
	if (!m_pObj2D[0]->Create(GetDevice(),L"Instacing.fx",L"../../data/particle.dds", 
		DX::TDxState::g_pAlphaBlend))
	{
		MessageBox(0, _T("m_pObj2D->Create"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pObj2D[1] = make_shared<TObject2D>();
	m_pObj2D[1]->SetCounter(1000);
	if (!m_pObj2D[1]->Create(GetDevice(), L"Instancing.fx", L"../../data/bul.dds",
		DX::TDxState::g_pBSColorOne))
	{
		MessageBox(0, _T("m_pObj2D->Create"), _T("Fatal error"), MB_OK);
		return 0;
	}

	return true;
}
bool		Sample::Frame()
{
	return true;
}
bool		Sample::Render()
{
	return true;
}
bool		Sample::Release()
{
	return true;
}
HRESULT		Sample::CreateResource()
{
	HRESULT hr = S_OK;
	return hr;
}
HRESULT		Sample::DeleteResource()
{
	HRESULT hr = S_OK;
	return hr;
}

Sample::Sample()
{
}
Sample::~Sample()
{
}

int Sample::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
	}
	return -1;
}
TBASIS_RUN(L"TBasisSample Instancing");