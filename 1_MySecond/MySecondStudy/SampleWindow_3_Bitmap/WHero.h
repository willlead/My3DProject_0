#pragma once
#include "WObject.h"
class WHero : public WObject
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	void Left();
	void Right();
	void Up();
	void Down();
public:
	WHero();
	virtual ~WHero();
};

