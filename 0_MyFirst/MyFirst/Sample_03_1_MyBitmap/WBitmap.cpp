#include "WBitmap.h"
#include "WSys.h"
bool     WBitmap::Load(HDC hScreenDC, TCHAR* pFileName)
{
	m_hScreenDC = hScreenDC;
	BITMAP info;
	//HANDLE'���� 'HBITMAP'(��)�� ��ȯ�� �� �����ϴ�.
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		pFileName, IMAGE_BITMAP, 0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);
	GetObject(m_hBitmap, sizeof(BITMAP), &info);
	// �޸� DC �Ҵ�( ȭ�� DC�� ȣȯ������ �� 2�� DC ����)
	m_hMemDC = CreateCompatibleDC(hScreenDC);
	// �޸� DC�� ��Ʈ���� ����
	SelectObject(m_hMemDC, m_hBitmap);
	return true;
}

WBitmap::WBitmap()
{
}


WBitmap::~WBitmap()
{
}
