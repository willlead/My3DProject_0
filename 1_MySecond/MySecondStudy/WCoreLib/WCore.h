#pragma once
#include "WWindow.h"
#include "WTimer.h"
#include "WInput.h"
class WCore : public WWindow 
{
public:
	HDC			m_hScreenDC;  // �� 1 DC => ���
							  // �� 2 DC=> ����
	HDC			m_hOffScreenDC;
	// �� 2 DC�� ��Ʈ��
	HBITMAP		m_hOffScreenBitmap;
public:
	WTimer     m_Timer;
	WInput     m_Input;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool     PreRender();// ��ο�
	virtual bool     Render();// ��ο�
	virtual bool     PostRender();// ��ο�
	virtual bool     Release();// ����, �Ҹ�
	virtual void     DebugString();
	virtual void     MsgEvent(MSG msg);
public:
	bool Run();
	bool GameRun();
	bool GameInit();
	bool GameFrame();
	bool GameRender();
	bool GameRelease();

public:
	WCore();
	virtual ~WCore();
};

