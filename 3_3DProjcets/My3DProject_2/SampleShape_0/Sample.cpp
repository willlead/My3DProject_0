#include "Sample.h"

bool	Sample::Init()
{
	m_pShape[0] = nullptr;
	m_pShape[0] = make_shared<TDirectionLineShape>();
	m_pShape[1] = make_shared<TBoxShape>();
	m_pShape[2] = make_shared<TPlaneShape>();

	if (!m_pShape[0]->Create(g_pd3dDevice,
		L"Line.hlsl", 0))
	{
		return false;
	}
	if (!m_pShape[1]->Create(g_pd3dDevice,
		L"Box.hlsl", L"../../data/Front.bmp"))
	{
		return false;
	}
	if (!m_pShape[2]->Create(g_pd3dDevice,
		L"Plane.hlsl",
		L"../../data/Back.bmp"))
	{
		return false;
	}
	for (int iObj = 0; iObj < MAX_CNT; iObj++)
	{
		D3DXMatrixIdentity(&m_matWorld[iObj]);
	}

	if (!m_Line.Create(g_pd3dDevice,
		L"Line.hlsl", 0))
	{
		return false;
	}

	D3DXVECTOR3 m_vEye = D3DXVECTOR3(10.0f, 10.0f, -10.0f);
	D3DXVECTOR3 m_vAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView,
		&m_vEye,
		&m_vAt,
		&m_vUp);

	float m_fAspet = 800.0f / 600.0f;
	float m_fFov = D3DX_PI * 0.25f;
	float fNear = 1.0f;
	float fFar = 1000.0f;
	D3DXMatrixPerspectiveFovLH(&m_matProj,
		m_fFov, m_fAspet, fNear, fFar);

	return true;
}
bool	Sample::Frame() {

	D3DXMATRIX matRotation, matTrans;
	D3DXMatrixIdentity(&matTrans);
	for (int iObj = 1; iObj < MAX_CNT; iObj++)
	{
		if (iObj == 2)
		{
			m_matWorld[iObj]._41 = iObj *
				10.0f * cosf(m_Timer.m_fAccumulation);

			D3DXMatrixTranslation(
				&matTrans,
				-iObj * 10.0f * cosf(m_Timer.m_fAccumulation),
				0,
				0);
		}

		D3DXMatrixRotationY(&matRotation,
			m_Timer.m_fAccumulation);

		m_matWorld[iObj] = matTrans * matRotation;
	}

	return true;
}
bool	Sample::Render()
{
	for (int iObj = 1; iObj < MAX_CNT; iObj++)
	{
		m_pShape[iObj]->SetMatrix(
			&m_matWorld[iObj],
			&m_matView,
			&m_matProj);
		m_pShape[iObj]->Render(g_pImmediateContext);
	}
	//m_vStart = m_pShape[1]->m_vPos;
	//m_vEnd = m_vStart + m_pShape[1]->m_vLook * 5.0f;
	//m_vColor = D3DXVECTOR4(1, 0, 0, 1);

	//m_Line.SetMatrix(0,	&m_matView,	&m_matProj);
	//m_Line.Draw(g_pImmediateContext,
	//	m_vStart, 
	//	m_vEnd, 
	//	m_vColor);

	//m_vEnd = m_vStart + m_pShape[1]->m_vUp * 5.0f;
	//m_vColor = D3DXVECTOR4(0, 1, 0, 1);
	//m_Line.Draw(g_pImmediateContext,
	//	m_vStart,
	//	m_vEnd,
	//	m_vColor);

	//m_vEnd = m_vStart + m_pShape[1]->m_vSide * 5.0f;
	//m_vColor = D3DXVECTOR4(0, 0, 1, 1);
	//m_Line.Draw(g_pImmediateContext,
	//	m_vStart,
	//	m_vEnd,
	//	m_vColor);
	D3DXMATRIX matScale, matRotation, matWorld;
	D3DXMatrixScaling(&matScale, 5, 5, 5);
	matWorld = matScale * m_matWorld[1];
	m_pShape[0]->SetMatrix(
		&matWorld,
		&m_matView,
		&m_matProj);
	m_pShape[0]->Render(g_pImmediateContext);

	matWorld = matScale * m_matWorld[2];
	m_pShape[0]->SetMatrix(
		&matWorld,
		&m_matView,
		&m_matProj);
	m_pShape[0]->Render(g_pImmediateContext);

	return true;
}
bool	Sample::Release() {
	return true;
}

Sample::Sample()
{

}


Sample::~Sample()
{
}
#ifndef _DEBUG
#define TCORE_RUN(s,x,y) TCORE_START;TCORE_WIN(s,x,y);
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int       nCmdShow)
{
	Sample sample;
	if (sample.SetWindow(hInstance, _T("Sample Win"), 800, 600))
	{
		sample.Run();
	}
}
#endif