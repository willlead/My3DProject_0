#pragma once
#include "TCore.h"

class Sample : public TCore
{
public:
	HRESULT CompileShaderFromFile(
		WCHAR* szFileName, 
		LPCSTR szEntryPoint, 
		LPCSTR szShaderModel, 
		ID3DBlob** ppBlobOut);

	void CleanupDevice();
	HRESULT InitDevice();

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};