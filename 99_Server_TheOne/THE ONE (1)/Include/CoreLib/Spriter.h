#pragma once
#include "Sprite.h"
#include "Shape.h"

class Spriter : public Shape
{
public:
	Sprite*			m_pSprite;
	float			m_fTimer;
	int				m_iCurrentFrame;
	int				m_iNumber;

public:
	void			Set();
	void			SetSprite();

public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();

public:
	Spriter();
	virtual ~Spriter();
};

