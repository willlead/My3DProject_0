#include "WObject.h"

WObject::WObject(){}
WObject::~WObject(){}

bool WObject::Load(HDC hScreenDC, HDC hOffScreenDC, TCHAR * pFileName){
	m_Bitmap.Load(hScreenDC, hOffScreenDC, pFileName);
	
	return true;}

bool WObject::Init(){
	return true;}

bool WObject::Frame(){
	return true;}

bool WObject::Render(){
	BitBlt(m_Bitmap.m_hScreenDC, 
		m_fPosX, m_fPosY, m_rt.w, m_rt.h,
		m_Bitmap.m_hMemDC,
		m_rt.x, m_rt.y, SRCCOPY);
	return true;}

bool WObject::Release(){
	m_Bitmap.Release();

	return true;}

bool WObject::Draw(){
	StretchBlt(m_Bitmap.m_hOffScreenDC,
		m_fPosX, m_fPosY,
		m_rt.w, m_rt.y,
		m_Bitmap.m_hMemDC,
		m_rt.x, m_rt.y,
		m_Bitmap.m_hBitInfo.bmWidth,
		m_Bitmap.m_hBitInfo.bmHeight,
		SRCCOPY);
	
	return true;}

void WObject::SetPos(int iX, int iY, float fSpeed)
{
	m_rtCollide.x = iX - (m_rt.w / 2);
	m_rtCollide.y = iY - (m_rt.h / 2);
	m_rtCollide.w = m_rtCollide.x + m_rt.w;
	m_rtCollide.h = m_rtCollide.y + m_rt.h;

	m_fPosX = iX;
	m_fPosY = iY;
	m_fSpeed = fSpeed;

}

void WObject::SetSpeed(float fSpeed)
{
}

void WObject::SetRect(int x, int y, int w, int h)
{
}
