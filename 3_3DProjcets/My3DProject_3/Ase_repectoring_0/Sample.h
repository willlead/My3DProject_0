#pragma once

#include "TBasisLib_0.h"
#include "ParserASE.h"
#include "TCamera.h"

class Sample : public TBasisLib_0
{
public:
	ParserASE    m_ParserASE;
	TAseObject m_Object;
	shared_ptr<TCamera > m_pMainCamera;
	vector<ID3D11ShaderResourceView*> m_TexSRV;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Relase();


public:
	Sample();
	virtual ~Sample();

};


