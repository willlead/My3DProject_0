#include "Shape.h"
#include "Sys.h"

bool Shape::Load(TCHAR* pszFileName1, TCHAR* pszFileName2)
{
	m_ColorBitmap = I_BitmapManager.AddData(pszFileName1);
	if (pszFileName2 != 0)
	{
		m_MaskBitmap = I_BitmapManager.AddData(pszFileName2);
	}
	return true;
}

void Shape::SetPosition(int iX, int iY)
{
	m_fX = iX;
	m_fY = iY;
}

void Shape::DrawBackGround()
{
	StretchBlt(g_hOffScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom,
		m_ColorBitmap->m_hMemoryDC, 0, 0, m_ColorBitmap->m_BitmapInfo.bmWidth, m_ColorBitmap->m_BitmapInfo.bmHeight, SRCCOPY);
}

bool Shape::Init()
{
	return true;
}

bool Shape::Frame()
{
	return true;
}

bool Shape::Render()
{
	if (m_bUsed)
	{
			BitBlt(g_hOffScreenDC, m_fX - m_rt.right / 2, m_fY - m_rt.bottom / 2, m_rt.right, m_rt.bottom, m_MaskBitmap->m_hMemoryDC, m_rt.left, m_rt.top, SRCAND);
			BitBlt(g_hOffScreenDC, m_fX - m_rt.right / 2, m_fY - m_rt.bottom / 2, m_rt.right, m_rt.bottom, m_ColorBitmap->m_hMemoryDC, m_rt.left, m_rt.top, SRCINVERT);
			BitBlt(g_hOffScreenDC, m_fX - m_rt.right / 2, m_fY - m_rt.bottom / 2, m_rt.right, m_rt.bottom, m_MaskBitmap->m_hMemoryDC, m_rt.left, m_rt.top, SRCINVERT);
	}
	else
	{
		return false;
	}
	return true;
}

bool Shape::Release()
{
	return true;
}

Shape::Shape()
{
	m_bUsed = true;
}

Shape::~Shape()
{
}
