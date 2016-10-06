#pragma once
#include "TCore.h"
class Sample : public TCore
{
public:

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};

