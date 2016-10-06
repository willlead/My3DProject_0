#include "WObject.h"
#include "WSys.h"
#include "WBitmapMgr.h"

WObject::WObject()
{
}


WObject::~WObject()
{
}


bool WObject::Load(HDC hScreenDC, HDC hOffScreenDC, TCHAR * pFileName) {
	m_pBitmap = I_BimapMgr.Load(pFileName);
	if (m_pBitmap == NULL)
	{
		return false;
	}
	return true;
}
bool WObject::Init() {

}
bool WObject::Frame() {

}
bool WObject::Render() {

}
bool WObject::Release() {

}

bool WObject::Draw() {
	StretchBlt(m_pBitmap->m_hOffScreenDC,
		m_fPosX, m_fPosY,
		m_rt.w, m_rt.h,
		m_pBitmap->m_hMemDC,
		m_rt.x, m_rt.y,
		m_pBitmap->m_hBitInfo.bmWidth,
		m_pBitmap->m_hBitInfo.bmHeight,
		SRCCOPY);
	return true;
}
void WObject::SetPos(int iX, int iY, float fSpeed) {
	m_rtCollide.x = iX - (m_rt.w / 2);
	m_rtCollide.y = iY - (m_rt.h / 2);
	m_rtCollide.w = m_rtCollide.x + m_rt.w;
	m_rtCollide.h = m_rtCollide.y + m_rt.h;

	m_fPosX = iX;
	m_fPosY = iY;
	m_fSpeed = fSpeed;
}
void WObject::SetSpeed(float fSpeed = 1.0f) {
	m_fSpeed += fSpeed * g_fSecondPerFrame;
	if (m_fSpeed < 10.0f) m_fSpeed = 10.0f;
}
void WObject::SetRect(int x, int y, int w, int h) {
	m_rt.x = x;
	m_rt.y = y;
	m_rt.w = w;
	m_rt.h = h;
}