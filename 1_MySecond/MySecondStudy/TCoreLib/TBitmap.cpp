#include "TBitmap.h"
bool TBitmap::Load(TCHAR* pszLoadFile)
{
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		pszLoadFile, IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE |
		LR_LOADFROMFILE);
	if (m_hBitmap != NULL)
	{
		GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
		m_hMemDC = CreateCompatibleDC(m_hScreenDC);
		//// ������ �޸𸮿� ��Ʈ�� ����
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
		m_szName = pszLoadFile;
	}
	
	return true;
};
bool TBitmap::Load(DWORD bitmap)
{
	m_hBitmap = LoadBitmap(g_hInstance,
		MAKEINTRESOURCE(bitmap));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// ������ �޸𸮿� ��Ʈ�� ����
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
void		TBitmap::Set(HDC hdc)
{
	m_hScreenDC = hdc;
};
bool		TBitmap::Draw(HDC hDC, 
	tPOINT pos, 
	RECT rt,
	DWORD dwScale,
	DWORD opMode,
	BLENDFUNCTION* pAlphaBF)
{
	switch (dwScale)
	{
	case LEFT_ROTATION:
	{
		StretchBlt(
			// ��ǥ ��� ����
			hDC, pos.x + rt.right, pos.y,
			-(rt.right), rt.bottom,
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);
	}break;
	case RIGHT_ROTATION:
	{
		StretchBlt(
			// ��ǥ ��� ����
			hDC, pos.x, pos.y + rt.bottom,
			rt.right, -(rt.bottom),//+pos.y*1.33f,
								   // ���� ����
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);
	}break;
	case 0x03:
	{
		StretchBlt(
			// ��ǥ ��� ����			
			hDC, pos.x + rt.right,
			pos.y + rt.bottom,
			-(rt.right), -(rt.bottom),
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);

		//StretchBlt(
		//	// ��ǥ ��� ����
		//	hDC, rt.right - rt.left,
		//	rt.bottom - rt.top,
		//	-(rt.right) + rt.left * 2,
		//	-(rt.bottom) + rt.top * 2,
		//	// ���� ����
		//	m_hMemDC, 0, 0, m_BmpInfo.bmWidth,
		//	m_BmpInfo.bmHeight,
		//	opMode);
	}break;
	case ALPHA_DRAW:
	{
		AlphaBlend(
			// ��ǥ ��� ����			
			hDC, pos.x, pos.y,
			rt.right,
			rt.bottom, 
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			*pAlphaBF);
	}break;
	default:
	{	
		BitBlt(
			// ��ǥ ��� ����
			hDC, pos.x, pos.y,
			rt.right,
			rt.bottom,
			// ���� ����
			m_hMemDC, 
			rt.left, 
			rt.top,
			opMode);		
	}break;
	}

	return true;
};
bool		TBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
};
bool TBitmap::SaveFile(HBITMAP hBitmap, TCHAR* strSaveFile)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	BITMAPINFOHEADER bitHeader;
	memset(&bitHeader, 0, sizeof(BITMAPINFOHEADER));
	bitHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitHeader.biWidth = bitmap.bmWidth;
	bitHeader.biHeight = bitmap.bmHeight;
	bitHeader.biPlanes = 1;
	bitHeader.biBitCount = bitmap.bmBitsPixel;
	bitHeader.biCompression = BI_RGB;
	//bitHeader.biSizeImage = bitmap.bmWidthBytes * bitmap.bmHeight;

	HDC hdc = GetDC(GetDesktopWindow());
	// biSizeImage ���� ���
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, NULL, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	// ��Ʈ�� ������ ���
	LPBYTE lpBits = new BYTE[bitHeader.biSizeImage];
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, lpBits, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	ReleaseDC(GetDesktopWindow(), hdc);

	// ���� ���� �� ����
	HANDLE hFile = CreateFile(strSaveFile, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		delete[] lpBits;
		return false;
	}
	// ���� ��� ����
	BITMAPFILEHEADER bmFileHeader;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bmFileHeader.bfType = ((WORD)('M' << 8) | 'B');//MAKEWORD(
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bitHeader.biSizeImage;

	DWORD dwWritten;
	//������� ����
	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//������� ����
	WriteFile(hFile, &bitHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	//������ ����
	WriteFile(hFile, lpBits, bitHeader.biSizeImage, &dwWritten, NULL);
	CloseHandle(hFile);

	delete[] lpBits;
	return true;
}
TBitmap::TBitmap()
{
}


TBitmap::~TBitmap()
{
}
