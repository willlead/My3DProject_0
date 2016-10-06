#include "Sample.h"
#include "WSys.h"

//HWND g_hWnd = NULL;
//HINSTANCE g_hInstance = NULL;

//vector<NodeType*> g_vecNode[NodeCnt];

Sample::Sample()
{
	m_fSecPerRender = 0.0f;
	m_iSpriteID = 0;
	m_iFrame = 0;
	m_fTimer = 0.0f;
}
Sample:: ~Sample()
{

}
bool Sample::DrawBackGround() {
	BitBlt(g_hOffScreenDC, 0, 0,
		m_BackGround.m_hBitInfo.bmWidth,
		m_BackGround.m_hBitInfo.bmHeight,
		m_BackGround.m_hMemDC,
		0, 0, SRCCOPY);

	StretchBlt(g_hOffScreenDC, 0, 0,
		m_BackGround.m_hBitInfo.bmWidth,
		m_BackGround.m_hBitInfo.bmHeight,

		m_BackGround.m_hMemDC,
		0, 0, 1280, 720, SRCCOPY);

	return true;
}
void Sample::DrawObject() {

	// Hero
	BitBlt(g_hOffScreenDC, 0, 300,
		225,
		225,
		m_HeroMaskBitmap.m_hMemDC,
		0, 0, SRCAND);

	BitBlt(g_hOffScreenDC, 0, 300,
		225,
		225,
		m_HeroColorBitmap.m_hMemDC,
		0, 0, SRCINVERT);

	BitBlt(g_hOffScreenDC, 0, 300,
		225,
		225,
		m_HeroMaskBitmap.m_hMemDC,
		0, 0, SRCINVERT);

	//Mob
	StretchBlt(g_hOffScreenDC, 700, 80,
		500, 500,
		m_MobMaskBitmap.m_hMemDC, 0, 0,
		225, 225, SRCAND);

	StretchBlt(g_hOffScreenDC, 700, 80,
		500, 500,
		m_MobColorBitmap.m_hMemDC, 0, 0,
		225, 225, SRCINVERT);

	StretchBlt(g_hOffScreenDC, 700, 80,
		500, 500,
		m_MobMaskBitmap.m_hMemDC, 0, 0,
		225, 225, SRCINVERT);
	//BitBlt(g_hOffScreenDC, 1000, 320,
	//	225,
	//	225,
	//	m_MobMaskBitmap.m_hMemDC,
	//	0, 0, SRCAND);
	//BitBlt(g_hOffScreenDC, 1000, 320,
	//	225,
	//	225,
	//	m_MobColorBitmap.m_hMemDC,
	//	0, 0, SRCINVERT);
	//BitBlt(g_hOffScreenDC, 1000, 320,
	//	225,
	//	225,
	//	m_MobMaskBitmap.m_hMemDC,
	//	0, 0, SRCINVERT);
}
bool Sample::Init() {
	m_hMaskMemDC = CreateCompatibleDC(g_hScreenDC);
	m_hColorMemDC = CreateCompatibleDC(g_hScreenDC);
	COLORREF bkColor = RGB(255, 255, 255);
	m_back = CreateSolidBrush(bkColor);


	LoadSprite(L"Sprite.txt");
	//int iIndex = I_Sprite.Load();

	// Bitmap ==================================================
	if (!m_BackGround.Load(_T("../../data/NH_BK_4.bmp")))
	{
		return false;
	}
	if (!m_HeroColorBitmap.Load(_T("../../data/NH_Char_1.bmp")))
	{
		return false;
	}
	if (!m_HeroMaskBitmap.Load(_T("../../data/NH_Char_2.bmp")))
	{
		return false;
	}
	if (!m_MobColorBitmap.Load(_T("../../data/NH_Mob_1.bmp")))
	{
		return false;
	}
	if (!m_MobMaskBitmap.Load(_T("../../data/NH_Mob_2.bmp")))
	{
		return false;
	}

	m_Node.Init();

	return true;
}
void Sample::SetSprite(int iIndex, float fLifeTime)
{
	m_iSpriteID = iIndex;
	m_fLifeTime = fLifeTime;
	int iFrame = m_SpriteList[m_iSpriteID].size();
	m_fSecPerRender = m_fLifeTime / iFrame;
	m_Bitmap.m_Position.x = 400;
	m_Bitmap.m_Position.y = 300;
	m_iFrame = 0;
	m_fTimer = 0.0f;

}
bool Sample::LoadSprite(TCHAR* pFileName)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	FILE* fp;
	_wfopen_s(&fp, pFileName, _T("rt"));
	if (fp == NULL) return false;

	int iNumSprite = 0;
	_fgetts(pBuffer, _countof(pBuffer), fp);
	_stscanf_s(pBuffer, _T("%s%d"),
		pTemp, _countof(pTemp), &iNumSprite);

	m_SpriteList.resize(iNumSprite);
	// *m_SpriteList[3]

	int iNumFrame = 0;
	for (int iSprite = 0; iSprite < iNumSprite; iSprite++)
	{
		_fgetts(pBuffer, _countof(pBuffer), fp);
		_stscanf_s(pBuffer, _T("%s%d"),
			pTemp, _countof(pTemp), &iNumFrame);

		int iNumber;
		m_SpriteList[iSprite].resize(iNumFrame);
		// m_SpriteList[0][iNumFrame]
		// m_SpriteList[1][?]
		// m_SpriteList[2][?]
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			RECT rt;
			_fgetts(pBuffer, _countof(pBuffer), fp);
			_stscanf_s(pBuffer, _T("%d %d%d%d%d"),
				&iNumber,
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			// 메모리가 부족하면 재할당을 통해서 저장한다.
			//m_SpriteList.push_back(rt);
			m_SpriteList[iSprite][iFrame] = rt;
		}
	}
	fclose(fp);
	return true;
}
bool Sample::Frame()
{

		SetSprite(0);

	if (m_Input.KeyCheck('3') == KEY_PUSH)
	{
		m_fLifeTime += 0.5f;
		m_fSecPerRender = m_fLifeTime / m_SpriteList[m_iSpriteID].size();
	}
	if (m_Input.KeyCheck('4') == KEY_PUSH)
	{
		m_fLifeTime -= 0.5f;
		m_fSecPerRender = m_fLifeTime / m_SpriteList[m_iSpriteID].size();
	}
	m_fTimer += g_fSecondPerFrame;
	if (m_fTimer > m_fSecPerRender)
	{
		if (++m_iFrame >= m_SpriteList[m_iSpriteID].size())
		{
			//m_iFrame = 0;
			//m_iSpriteID++;
			//if (m_iSpriteID >= 3)
			//{
			//	m_iSpriteID = 0;
			//}
			//SetSprite(m_iSpriteID);
		}
		m_fTimer = 0.0f;// m_fSecPerRender;
	}
	if (m_iFrame < 21)
		m_Bitmap.m_rtDraw = m_SpriteList[m_iSpriteID][m_iFrame];
	else
		m_iFrame = 0;

	for (int i = 0; i < m_Node.vData.size(); i++)
	{

		//if (RectInPt(m_Node.m_iNodeType[i].rect, m_Input.m_MovePt) == true)
		//{
		//	m_Node.findNode(i);
		//}
		//if (RectInPt(m_Node.vData[i]->rect, m_Input.m_MovePt) == true)
		//{
		//	m_Node.findNode(i);
		//}

		if (RectInPt(m_Node.vData[i]->rect, m_Input.m_MovePt) == true)
		{
#ifdef  NodeCnt
			if (m_Input.m_dwMouseState[0] == true)
			{
				m_Node.findNode(i);
				SetSprite(0);
				
			}
#else
			if (m_Input.m_dwMouseState[1] == false)
			{
				m_Node.findNode(i);
			}
#endif
		}
	}

	return true;
}
bool Sample::Render() {

	DrawBackGround();
	DrawObject();
	m_Bitmap.Render();
	m_Node.Render();
	return true;
}
bool Sample::Release() {

	return true;
}

int WINAPI wWinMain(
	HINSTANCE hInstatnce,
	HINSTANCE hPrevInstatnce,
	LPWSTR   lpCmdLine,
	int      nCmdShow)
{
	Sample win;
	if (win.SetWindow(hInstatnce) == true)
	{
		win.Run();
	}
	return 0;
}