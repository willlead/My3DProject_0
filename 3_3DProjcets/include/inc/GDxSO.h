#pragma once
#include "GBasisStd.h"
namespace DX
{
	class GDxSO
	{
	public:
		ComPtr<ID3D11Buffer>	m_pDrawFrom;
		ComPtr<ID3D11Buffer>	m_pStreamTo;
	public:
		bool BeginRTS(ID3D11DeviceContext*    pImmediateContext);
		void EndRTS(ID3D11DeviceContext*    pImmediateContext);

		HRESULT		CreateStreams(ID3D11Device* pd3dDevice,
			DWORD dwSize,
			DWORD dwCount);
	public:
		GDxSO(void);
		~GDxSO(void);
	};
}
using namespace DX;