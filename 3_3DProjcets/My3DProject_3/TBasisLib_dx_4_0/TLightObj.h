#pragma once
#include "TBasisStd.h"

struct TLightInfo
{
	D3DXVECTOR3				m_vPosition;	// ����Ʈ�� ��ġ
	D3DXVECTOR3				m_vDirection;	// ����Ʈ�� ����
	float					m_fRange;		// ������ ��ȿ �Ÿ�
	float					m_fFalloff;		// ���� �ܰ� �ܺ� ���� �ֵ�( Luminance )
	float					m_fTheta;		// ���� ���� ���� ����		
	float					m_fPhi;			// �ܺ� ���� ���� ����
	D3DXVECTOR4				m_DiffuseColor;
};

class TLightObj : public TLightInfo
{
public:
	D3DXMATRIX				m_matInitWorld;
	D3DXMATRIX				m_matWorld;
	D3DXVECTOR3				m_vInitPosition;// �ʱ� ����Ʈ�� ��ġ��
	D3DXVECTOR3				m_vScale;		// ����Ʈ ������Ʈ�� ũ�⺯ȯ ��
	D3DXVECTOR3				m_vRotation;    // ����Ʈ ������Ʈ�� ȸ��(xyz)
	D3DXVECTOR3				m_vOffset;		// ���� ���̿��� �߰��� ��ġ(x,y,z)
public:
	T_STR		m_szName;
	T_STR		m_szPath;
public:
	HRESULT				Load(ID3D11Device* pDevice, const TCHAR* strFilePath);
	void				SetPath(TCHAR* pPath);
	void					SetValue(D3DXVECTOR3* pvPos = NULL,
		D3DXVECTOR3* pvDir = NULL,
		float* pfRange = NULL,
		D3DXVECTOR4* pvColor = NULL,
		D3DXVECTOR3* pvScale = NULL,
		D3DXVECTOR3* pvRotation = NULL,
		float fTheta = D3DX_PI / 4.0f,
		float fPhi = 5.0f);
public:
	bool					Init();
	bool					Frame(float fElapaseTime = 0.0f, float fSecondPerFrame = 0.0f);
	bool					Update(D3DXMATRIX* pmatLightWorld);
	bool					Render();
	bool					Release();
	//--------------------------------------------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	TLightObj(void);
	~TLightObj(void);
};
class TLightMgr : public TSingleton < TLightMgr >
{
private:
	friend class TSingleton<TLightMgr>;
	ID3D11Device*			m_pd3dDevice;
public:
	typedef map <INT, TLightObj*>					TemplateMap;
	typedef TemplateMap::iterator				TemplateMapItor;
	TemplateMapItor								TItor;
	TemplateMap									TMap;
	INT											m_iCurIndex;


public:
	void			SetDevice(ID3D11Device*	 pDevice);

	//D3DXVECTOR3 vPosition( 0.0f, 10.0f, 0.0f );
	//D3DXVECTOR3 vDir( 0.0f, -1.0f, 0.0f );
	INT				Add(D3DXVECTOR3,
		D3DXVECTOR3,
		const TCHAR *pFileName = _T("NULL"));

	TLightObj* const		GetPtr(INT iIndex);
	TLightObj* const		GetPtr(T_STR strFindName);
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
	bool			Update(D3DXMATRIX* pmatLightWorld);
public:
	TLightMgr();
	~TLightMgr();
};
#define I_Light	 TLightMgr::GetInstance()