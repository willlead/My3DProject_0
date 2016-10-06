#pragma once
#include "Std.h"
#include "BitmapManager.h"
#include "Shape.h"

class Sprite : public Shape
{
public:
	wstring				m_szName;
	vector<RECT>		m_rtList;
	float				m_fTimer;
	float				m_fLifeTime;
	float				m_fSecPerRender;
	int					m_iCurrentFrame;
	int					m_iNumFrame;
	
public:
	void				SetRectArray(vector<RECT>, float);
	
public:
	bool				Init();
	bool				Frame();
	bool				Release();

public:
	Sprite();
	virtual ~Sprite();
};

