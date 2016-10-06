#include "TStreamOut.h"
ID3D11Buffer*	TStreamOut::GetDrawBuffer()
{
	return m_pDrawFrom;
}
HRESULT TStreamOut::CreateStreams( D3D11_BUFFER_DESC vbdesc, ID3D11Device* pd3dDevice )
{
    HRESULT hr = S_OK;
    V_RETURN( pd3dDevice->CreateBuffer( &vbdesc, NULL, &m_pDrawFrom ) );
    V_RETURN( pd3dDevice->CreateBuffer( &vbdesc, NULL, &m_pStreamTo ) );
    return hr;
}

HRESULT TStreamOut::Begin( ID3D11DeviceContext*    pImmediateContext )
{
    HRESULT hr = S_OK;

    UINT offset = 0;
    ID3D11Buffer* pVB[1] = {m_pStreamTo};
    pImmediateContext->SOSetTargets( 1, pVB, &offset );  
    return hr;
}

void TStreamOut::End(  ID3D11DeviceContext*  pImmediateContext )
{
    UINT offset = 0;
	ID3D11Buffer* pVB[1] = { NULL };
    pImmediateContext->SOSetTargets( 1, pVB, &offset );

    ID3D11Buffer* pTemp = m_pStreamTo;
    m_pStreamTo = m_pDrawFrom;
    m_pDrawFrom = pTemp;
}
bool TStreamOut::Release()
{
	SAFE_RELEASE( m_pDrawFrom );
	SAFE_RELEASE( m_pStreamTo );
	return true;
}
TStreamOut::TStreamOut(void)
{
	m_pDrawFrom = NULL;
	m_pStreamTo = NULL;
}


TStreamOut::~TStreamOut(void)
{
}
