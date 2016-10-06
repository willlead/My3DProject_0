#pragma once
#include "bgStd.h"

LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class bgWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	DWORD		m_dwStyle;		// ������ ��Ÿ��
	DWORD		m_dwStyleEx;	// ������ ��Ÿ��(Ȯ��)
	RECT		m_rtWindow;		// ������ �����¿� ��ġ��
	int			m_iScreenW, m_iScreenH; // ȭ�� ũ��
	int			m_iClientW, m_iClientH; // Ŭ���̾�Ʈ ũ��
	int			m_iWindowW, m_iWindowH; // ������ ũ��
	int			m_iWindowX, m_iWindowY; // ������ ��ġ
	bool		m_bSizeLarge;	// ������ ũ�� (1024x768 <=> 800x600)
	bool		m_bFullScreen;	// ������ <=> ��üȭ�� ��庯ȯ
	bool		m_bActive;		// ������ Ȱ������

public:
	bgWindow();
	virtual ~bgWindow();

public:
	bool	AppRun();
	virtual LRESULT CALLBACK AppProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool	InitWindow(HINSTANCE hInstance, TCHAR* pszTitle = L"BG Project!", int iWidth = 800, int iHeight = 600, BOOL bFullScreen = FALSE, bool bCenter = true);
	bool	SetWindowSize(HWND hWnd, int iWidth, int iHeight);
	
public:
	virtual bool	GameRun() { return true; }
	virtual bool	GameInit() { return true; }
	virtual bool	GameFrame() { return true; }
	virtual bool	GameRender() { return true; }
	virtual bool	GameRelease() { return true; }
};
