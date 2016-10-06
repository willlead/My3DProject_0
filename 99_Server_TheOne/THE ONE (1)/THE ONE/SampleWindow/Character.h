#pragma once
#include "Object.h"

//enum CharacterState { STOT = 0, UP, DOWN=1, LEFT, RIGHT };

class Character : public Object
{
public:
	int						m_CharacterState;
	bool					m_bShow;
	bool					m_bPlaySound;
	int						m_iLife;
	int						m_iDirection;

public:						// ∫Œ»∞
	bool					m_bRebornShow;
	bool					m_bReborn;
	float					m_fRebornEndTime;
	float					m_fRebornTime;
	float					m_fRebornTimer;
	float					m_fRebornShowTime;

public:
	void					Up();
	void					Down();
	void					Left();
	void					Right();
	void					Reset();

public:
	Character();
	virtual ~Character();
};

