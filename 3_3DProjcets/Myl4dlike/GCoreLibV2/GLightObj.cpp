#include "GLightObj.h"
void GLightObj::SetPath(TCHAR* pPath)
{
	m_szPath = pPath;
}
HRESULT		GLightObj::Load(ID3D11Device* pDevice, const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;

	TCHAR szFileName[MAX_PATH] = { 0, };
	_stprintf_s(szFileName, _T("%s%s"), m_szPath.c_str(), strFilePath);

	m_szName = strFilePath;
	return hr;
}
void GLightObj::SetValue(D3DXVECTOR3* pvPos,
	D3DXVECTOR3* pvDir,
	float* pfRange,
	D3DXVECTOR4* pvColor,
	D3DXVECTOR3* pvScale,
	D3DXVECTOR3* pvRotation,
	float fTheta,
	float fPhi)
{
	if (pvPos != NULL)	m_vInitPosition = *pvPos;
	if (pvDir != NULL)	m_vDirection = *pvDir;
	if (pfRange != NULL) m_fRange = *pfRange;
	if (pvColor != NULL) m_DiffuseColor = *pvColor;
	// 외부
	m_fTheta = cosf(D3DXToRadian(fTheta / 2));
	// 내부 
	m_fPhi = cosf(D3DXToRadian(fPhi / 2));

	if (pvScale != NULL) m_vScale = *pvScale;
	if (pvRotation != NULL) m_vRotation = *pvRotation;

	D3DXMATRIX matScale, matRotation;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&matRotation, m_vRotation.x, m_vRotation.y, m_vRotation.z);
	D3DXMatrixMultiply(&m_matInitWorld, &matScale, &matRotation);

	m_matInitWorld._41 = m_vInitPosition.x;
	m_matInitWorld._42 = m_vInitPosition.y;
	m_matInitWorld._43 = m_vInitPosition.z;
}

bool GLightObj::Init()
{
	SetValue();
	return true;
}
bool GLightObj::Update(D3DXMATRIX* pmatLightWorld)
{
	m_vPosition.x = pmatLightWorld->_41;
	m_vPosition.y = pmatLightWorld->_42;
	m_vPosition.z = pmatLightWorld->_43;

	D3DXVec3Normalize(&m_vDirection, &m_vPosition);
	m_vDirection *= -1.0f;

	return true;
}
bool GLightObj::Frame(float fElapaseTime, float fSecondPerFrame)
{
	// 2초당 1회전( 1 초 * D3DX_PI = 3.14 )
	float t = fElapaseTime * D3DX_PI;

	D3DXMATRIX mRotation;
	D3DXMatrixRotationY(&mRotation, t*0.1f);
	D3DXMatrixMultiply(&m_matWorld, &m_matInitWorld, &mRotation);

	m_vPosition.x = m_matWorld._41;
	m_vPosition.y = m_matWorld._42;
	m_vPosition.z = m_matWorld._43;

	D3DXVec3Normalize(&m_vDirection, &m_vPosition);
	m_vDirection *= -1.0f;
	return true;
}

bool GLightObj::Render()
{
	return true;
}
bool GLightObj::Release()
{
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GLightObj::CreateResource()
{
	HRESULT hr = S_OK;
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GLightObj::DeleteResource()
{
	HRESULT hr = S_OK;
	return hr;
}
GLightObj::GLightObj(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matInitWorld);

	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vInitPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_fRange = 20.0f;
	m_fFalloff = 4.0f;

	m_DiffuseColor = D3DXVECTOR4(1.0f, 0.5f, 0.5f, 1.0f);

	m_fTheta = D3DX_PI / 4.0f;
	m_fPhi = m_fTheta + D3DXToRadian(5);

	m_vOffset = D3DXVECTOR3(0.0f, 5.0f, 0.0f);

	m_szPath = _T("");
}

GLightObj::~GLightObj(void)
{
}

INT TLightMgr::Add(D3DXVECTOR3 vPosition, D3DXVECTOR3 vDir, const TCHAR *pFileName)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pFileName)
	{
		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			GLightObj *pPoint = (GLightObj *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	GLightObj *pPoint = NULL;
	SAFE_NEW(pPoint, GLightObj);

	TCHAR szPath[MAX_PATH] = { 0, };
	_stprintf_s(szPath, _T("%s%s"), Drive, Dir);
	pPoint->SetPath(szPath);

	if (FAILED(pPoint->Load(m_pd3dDevice, szFileName)))
	{
		return 0;
	}
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	return m_iCurIndex;
}

bool TLightMgr::Init()
{
	GLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (GLightObj *)(*itor).second;
		pPoint->Init();
	}
	return true;
}
bool TLightMgr::Frame()
{
	GLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (GLightObj *)(*itor).second;
		pPoint->Frame();
	}
	return true;
}
bool TLightMgr::Render()
{
	GLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (GLightObj *)(*itor).second;
		pPoint->Render();
	}
	return true;
}
bool TLightMgr::Update(D3DXMATRIX* matLightWorld)
{
	GLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (GLightObj *)(*itor).second;
		pPoint->Update(matLightWorld);
	}
	return true;
}
bool TLightMgr::Release()
{
	GLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (GLightObj *)(*itor).second;
		pPoint->Release();
	}
	TMap.clear();
	return true;
}
GLightObj* const TLightMgr::GetPtr(INT iIndex)
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return NULL;
	GLightObj *pPoint = (*itor).second;
	return pPoint;
}
GLightObj* const  TLightMgr::GetPtr(T_STR strFindName)
{
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		GLightObj *pPoint = (GLightObj *)(*itor).second;
		if (pPoint->m_szName == strFindName)
		{
			return (*itor).second;
		}
	}
	return NULL;
}
void TLightMgr::SetDevice(ID3D11Device*	 pDevice)
{
	m_pd3dDevice = pDevice;
}
TLightMgr::TLightMgr(void)
{
	m_pd3dDevice = NULL;
	m_iCurIndex = 0;
	TMap.clear();
}

TLightMgr::~TLightMgr(void)
{
	GLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (GLightObj *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
}