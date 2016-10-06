#include "TCore.h"
#include "TWrite.h"
class Sample : public TCore
{
public:
	ID3D11Texture2D*  m_Texture1;
	ID3D11Texture2D*  m_Texture2;
	ID3D11Texture2D*  m_pBackBufferTex;
	D3D11_TEXTURE2D_DESC m_td1;
	D3D11_TEXTURE2D_DESC m_td2;
	void WriteDotPixel(ID3D11Texture2D* pTex);
//public:
//	ID3D11Buffer*           g_pVertexBuffer = NULL;
//	ID3D11Buffer*           g_pIndexBuffer = NULL;
//	ID3D11Buffer*           g_pConstantBuffer = NULL;
public:
	ID3D11Texture2D* GetTexture2DFromFile(
		LPCWSTR filename,
		D3DX11_IMAGE_LOAD_INFO* pLoadInfo);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	virtual ~Sample();
};
