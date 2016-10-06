#pragma once
#include "TArcBall.h"
#include "TFrustum.h"

#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON  0x04
#define MOUSE_WHEEL         0x08

class TCamera : public TFrustum
{
public:	
	TArcBall			m_WorldArcBall;
    TArcBall			m_ViewArcBall;
	//--------------------------------------------------------------------------------------
	// ��� ����
	//--------------------------------------------------------------------------------------
	D3DXVECTOR3			m_vModelCenter;
    D3DXMATRIX			m_mModelLastRot;        // Last arcball rotation matrix for model 
    D3DXMATRIX			m_mModelRot;            // Rotation matrix of model
  	D3DXMATRIX			m_mCameraRotLast;

	// �̵� �ӵ�
	float				m_fSpeed;
public:
	//--------------------------------------------------------------------------------------
	// �� ȸ���� ���� ���ʹϾ�
	//--------------------------------------------------------------------------------------
	D3DXQUATERNION		m_qRotation;
	//--------------------------------------------------------------------------------------
	// �� �࿡ ���� ������ w=������( �������κ����� �Ÿ� )
	//--------------------------------------------------------------------------------------
	D3DXVECTOR3			m_vPosDelta;
           
	//--------------------------------------------------------------------------------------
	// �� ��� �� ���� ���
	//--------------------------------------------------------------------------------------
	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matView;	
	D3DXMATRIX			m_matProj;
	//--------------------------------------------------------------------------------------
	// ī�޶� ���⺤�� �� ��ġ / Ÿ�� ����	
	//--------------------------------------------------------------------------------------
	D3DXVECTOR3			m_vRightVector;
	D3DXVECTOR3			m_vLookVector;
	D3DXVECTOR3			m_vUpVector; 
	D3DXVECTOR3			m_vCameraPos;
	D3DXVECTOR3			m_vTargetPos;	
	D3DXVECTOR3			m_vDefaultEye;          // Default camera eye position
	D3DXVECTOR3			m_vDefaultLookAt;       // Default LookAt position
	//--------------------------------------------------------------------------------------
	// ���� ����� �Ӽ�����
	//--------------------------------------------------------------------------------------	
	float				m_fFOV;                 // Field of view
    float				m_fAspect;              // Aspect ratio
    float				m_fNearPlane;           // Near plane
    float				m_fFarPlane;            // Far plane
	// �߰� : ���Ϸ� �� ���� 	
	float				m_fCameraYawAngle;
	float				m_fCameraPitchAngle;
	float				m_fCameraRollAngle;
    // �߰� : ������( �ʱ� �������κ����� �Ÿ�) �� ���� ���� 	
	float				m_fRadius;   
    float				m_fDefaultRadius;       // Distance from the camera to model 
    float				m_fMinRadius;           // Min radius
    float				m_fMaxRadius;           // Max radius
	//--------------------------------------------------------------------------------------
	// ���콺 �� ��ư�� ��뿩�θ� �Ǵ��ϴ� ����ũ ���� ���콺 �� ������
	//--------------------------------------------------------------------------------------	
	int					m_nMouseWheelDelta;     // Amount of middle wheel scroll (+/-) 
	int					m_nRotateModelButtonMask;
	int					m_nZoomButtonMask;
	int					m_nRotateCameraButtonMask;
public :
	//--------------------------------------------------------------------------------------
	// Ÿ�� ��ġ�� ���� �Ÿ�(������)�� �����Ѵ�.
	//--------------------------------------------------------------------------------------
	void						SetRadius( float fDefaultRadius=5.0f, float fMinRadius=1.0f, float fMaxRadius=FLT_MAX  );
public:			
	//--------------------------------------------------------------------------------------
	// �߰� : �� ���⺤�� ���� 
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			UpdateVector();
	//--------------------------------------------------------------------------------------
	// ������Ʈ�� ȭ�鿡 ���߾� �� ��� ���� 
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			SetObjectView(	D3DXVECTOR3 vMax, 
												D3DXVECTOR3 vMin );	
	//--------------------------------------------------------------------------------------
	// �� ��� ���  
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			SetViewMatrix(	D3DXVECTOR3 vPos, 
												D3DXVECTOR3 vTarget= D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 
												D3DXVECTOR3 vUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	//--------------------------------------------------------------------------------------
	// ���� ��� ��� 
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			SetProjMatrix(	FLOAT fFOV, 
												FLOAT fAspect, 
												FLOAT fNearPlane, 
												FLOAT fFarPlane );
	//--------------------------------------------------------------------------------------
	// ī�޶� ��ġ ���� 
	//--------------------------------------------------------------------------------------
	virtual void				MoveLook( float fValue );
	virtual void				MoveSide( float fValue );	
	virtual void				MoveUp( float fValue );	
	//--------------------------------------------------------------------------------------
	// ���Ϸ� ���� ���� �� ��� ��� 
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			Update( D3DXVECTOR4 vDirValue );
	virtual D3DXMATRIX			Update( float fElapseTime );

	// ȸ�� �� �̵��� ������ ��ư ���� 
	virtual void				SetButtonMasks( int nRotateModelButtonMask, int nZoomButtonMask, int nRotateCameraButtonMask );
	//--------------------------------------------------------------------------------------
	// ������ ����� ������ �ݿ��Ѵ�. 
	//--------------------------------------------------------------------------------------
	virtual void				SetWindow( int nWidth, int nHeight, float fArcballRadius=0.9f );		
	//--------------------------------------------------------------------------------------
	// ������ �޼����� ����Ͽ� ī�޶� ���� 
	//--------------------------------------------------------------------------------------
	virtual int					WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//--------------------------------------------------------------------------------------
	// �� ȸ�� �߽���ġ�� �����Ѵ�.
	//--------------------------------------------------------------------------------------
    void						SetModelCenter( D3DXVECTOR3 vModelCenter );

	void						SetTargetPos( D3DXVECTOR3 vPos ) { m_vTargetPos = vPos; };

public:
	//--------------------------------------------------------------------------------------
	// ����Լ�
	//--------------------------------------------------------------------------------------
	D3DXMATRIX*			GetWorldMatrix() 	{ return &m_matWorld; };
	D3DXMATRIX*			GetViewMatrix() 	{ return &m_matView; }
	D3DXMATRIX*			GetProjMatrix() 	{ return &m_matProj; }
	
	D3DXVECTOR3*		GetEyePt() 			{ return &m_vCameraPos; }
	D3DXVECTOR3*		GetLookAtPt() 		{ return &m_vTargetPos; }
	float				GetNearClip() 		{ return m_fNearPlane; }
	float				GetFarClip() 		{ return m_fFarPlane; }
	
	float				GetYawAngle() 		{ return m_fCameraYawAngle; }
	float				GetPitchAngle() 	{ return m_fCameraPitchAngle; }
	float				GetRollAngle() 		{ return m_fCameraRollAngle; }
	
	D3DXVECTOR3*		GetRightVector() 	{ return &m_vRightVector; }
	D3DXVECTOR3*		GetLookVector() 	{ return &m_vLookVector; }
	D3DXVECTOR3*		GetUpVector() 		{ return &m_vUpVector; }

	float				GetRadius() 		{ return m_fRadius; }
public:
	TCamera(void);
	virtual ~TCamera(void);
};

