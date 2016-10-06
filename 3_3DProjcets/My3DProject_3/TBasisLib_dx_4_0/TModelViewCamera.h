#pragma once
#include "TCamera.h"

class TModelViewCamera : public TCamera
{
public:		
	//--------------------------------------------------------------------------------------
	// ī�޶� ��ġ ���� 
	//--------------------------------------------------------------------------------------
	void				MoveLook( float fValue );
	void				MoveSide( float fValue );	
	void				MoveUp( float fValue );	
	//--------------------------------------------------------------------------------------
	// ���Ϸ� ���� ���� �� ��� ��� 
	//--------------------------------------------------------------------------------------
	D3DXMATRIX			Update( float fElapseTime );
	// ���� ���� ��ȯ ����� ���⺤��
	D3DXVECTOR3			m_vObjectVector[3];
public:

	//--------------------------------------------------------------------------------------
	// �� ��� ���  
	//--------------------------------------------------------------------------------------
	D3DXMATRIX			SetViewMatrix( D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f ));	

public:
	TModelViewCamera(void);
	virtual ~TModelViewCamera(void);
};
