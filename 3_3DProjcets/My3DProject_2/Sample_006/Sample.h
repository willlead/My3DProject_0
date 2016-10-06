#pragma once
#include "TCore.h"
class Sample : public TCore
{
public:
	HRESULT InitDevice();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	Sample();
	~Sample();
};

