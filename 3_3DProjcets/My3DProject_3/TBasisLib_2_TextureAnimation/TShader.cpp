#include "TShader.h"
static const unsigned int MAX_INCLUDES = 9;
struct sInclude
{
	HANDLE         hFile;
	HANDLE         hFileMap;
	LARGE_INTEGER  FileSize;
	void           *pMapData;
};

class CIncludeHandler : public ID3DInclude
{
private:
	struct sInclude   m_includeFiles[MAX_INCLUDES];
	unsigned int      m_nIncludes;

public:
	CIncludeHandler()
	{
		// array initialization
		for (unsigned int i = 0; i<MAX_INCLUDES; i++)
		{
			m_includeFiles[i].hFile = INVALID_HANDLE_VALUE;
			m_includeFiles[i].hFileMap = INVALID_HANDLE_VALUE;
			m_includeFiles[i].pMapData = NULL;
		}
		m_nIncludes = 0;
	}
	~CIncludeHandler()
	{
		for (unsigned int i = 0; i<m_nIncludes; i++)
		{
			UnmapViewOfFile(m_includeFiles[i].pMapData);

			if (m_includeFiles[i].hFileMap != INVALID_HANDLE_VALUE)
				CloseHandle(m_includeFiles[i].hFileMap);

			if (m_includeFiles[i].hFile != INVALID_HANDLE_VALUE)
				CloseHandle(m_includeFiles[i].hFile);
		}

		m_nIncludes = 0;
	}

	STDMETHOD(Open(
		D3D_INCLUDE_TYPE IncludeType,
		LPCSTR pFileName,
		LPCVOID pParentData,
		LPCVOID *ppData,
		UINT *pBytes
		))
	{
		unsigned int   incIndex = m_nIncludes + 1;

		// Make sure we have enough room for this include file
		if (incIndex >= MAX_INCLUDES)
			return E_FAIL;

		// try to open the file
		m_includeFiles[incIndex].hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (INVALID_HANDLE_VALUE == m_includeFiles[incIndex].hFile)
		{
			return E_FAIL;
		}

		// Get the file size
		GetFileSizeEx(m_includeFiles[incIndex].hFile, &m_includeFiles[incIndex].FileSize);

		// Use Memory Mapped File I/O for the header data
		m_includeFiles[incIndex].hFileMap = CreateFileMappingA(m_includeFiles[incIndex].hFile, NULL, PAGE_READONLY, m_includeFiles[incIndex].FileSize.HighPart, m_includeFiles[incIndex].FileSize.LowPart, pFileName);
		if (m_includeFiles[incIndex].hFileMap == NULL)
		{
			if (m_includeFiles[incIndex].hFile != INVALID_HANDLE_VALUE)
				CloseHandle(m_includeFiles[incIndex].hFile);
			return E_FAIL;
		}

		// Create Map view
		*ppData = MapViewOfFile(m_includeFiles[incIndex].hFileMap, FILE_MAP_READ, 0, 0, 0);
		*pBytes = m_includeFiles[incIndex].FileSize.LowPart;

		// Success - Increment the include file count
		m_nIncludes = incIndex;

		return S_OK;
	}

	STDMETHOD(Close(LPCVOID pData))
	{
		// Defer Closure until the container destructor 
		return S_OK;
	}
};
HRESULT TShader::SetInputLayout(ID3D11Device* pd3dDevice, D3D11_INPUT_ELEMENT_DESC* pLayout, UINT numElements, ID3DBlob* pBuffer)
{
	HRESULT hr;

	if (pLayout == NULL)
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		pLayout = layout;
		numElements = 4;
	}
	if (pBuffer != NULL)
	{
		if (FAILED(hr = m_InputLayout.Create(pd3dDevice, pLayout, numElements, pBuffer)))
		{
			MessageBox(0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK);
			return hr;
		}
	}
	else
	{
		if (FAILED(hr = m_InputLayout.Create(pd3dDevice, pLayout, numElements, &m_pTechnique)))
		{
			MessageBox(0, _T("SetInputLayout  실패"), _T("Fatal error"), MB_OK);
			return hr;
		}
	}
	return S_OK;
}
void TShader::Apply(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	pd3dContext->HSSetShader(m_pHullShader, NULL, 0);
	pd3dContext->DSSetShader(m_pDomainShader, NULL, 0);
	pd3dContext->GSSetShader(m_pGeometryShader, NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	pd3dContext->CSSetShader(m_pComputeShader, NULL, 0);
}

HRESULT		TShader::Load(ID3D11Device* pd3dDevice,
	TCHAR* pLoadShaderFile,
	CHAR* pEntryPoint,
	CHAR* pCompiler,
	SHADER_TYPE type,
	ID3DBlob* pLoadBlob)
{
	HRESULT hr = S_OK;
	if (pLoadBlob == NULL)
	{
		// 스크립트 로드해서 컴파일
		DWORD dwShaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
#if defined( _DEBUG ) 
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob;

		if (FAILED(hr = D3DX11CompileFromFile(
			pLoadShaderFile,//L"VertexShader.vsh",
			NULL, NULL,
			pEntryPoint,//"VS",
			pCompiler, //"vs_5_0",
			dwShaderFlags,
			NULL, NULL,
			&m_pBuffer, &pErrorBlob, NULL)))
		{
			//DXTRACE_ERR_MSGBOX( DXGetErrorDescription(hr),hr);
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			SAFE_RELEASE(pErrorBlob);
			return hr;
		}
	}
	switch (type)
	{
	case t_VS:
	{
		V_RETURN(pd3dDevice->CreateVertexShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pVertexShader));

		D3DGetInputSignatureBlob(m_pBlobVS->GetBufferPointer(),
			m_pBlobVS->GetBufferSize(),
			&m_pBlobVS);
	}break;
	case t_HS:
	{
		V_RETURN(pd3dDevice->CreateHullShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pHullShader));

	}break;
	case t_DS:
	{
		V_RETURN(pd3dDevice->CreateDomainShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pDomainShader));

	}break;

	case t_GS:
	{
		V_RETURN(pd3dDevice->CreateGeometryShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pGeometryShader));

	}break;
	case t_PS:
	{
		V_RETURN(pd3dDevice->CreatePixelShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pPixelShader));

	}break;
	case t_CS:
	{
		V_RETURN(pd3dDevice->CreateComputeShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pComputeShader));

	}break;
	}

	return hr;
};
void TShader::ApplyVS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	else
		pd3dContext->VSSetShader(NULL, NULL, 0);
}
void TShader::ApplyHS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->HSSetShader(m_pHullShader, NULL, 0);
	else
		pd3dContext->HSSetShader(NULL, NULL, 0);
}
void TShader::ApplyDS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->DSSetShader(m_pDomainShader, NULL, 0);
	else
		pd3dContext->DSSetShader(NULL, NULL, 0);
}
void TShader::ApplyGS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->GSSetShader(m_pGeometryShader, NULL, 0);
	else
		pd3dContext->GSSetShader(NULL, NULL, 0);
}
void TShader::ApplyPS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	else
		pd3dContext->PSSetShader(NULL, NULL, 0);
}
void TShader::ApplyCS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->CSSetShader(m_pComputeShader, NULL, 0);
	else
		pd3dContext->CSSetShader(NULL, NULL, 0);
}
HRESULT TShader::CompileShaderFromFile(const WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// find the file
	WCHAR str[MAX_PATH];
	WCHAR workingPath[MAX_PATH], filePath[MAX_PATH];
	WCHAR *strLastSlash = NULL;
	bool  resetCurrentDir = false;

	// Get the current working directory so we can restore it later
	UINT nBytes = GetCurrentDirectory(MAX_PATH, workingPath);
	if (nBytes == MAX_PATH)
	{
		return E_FAIL;
	}

	wcscpy_s(str, MAX_PATH, szFileName);
	// Check we can find the file first
	// V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, szFileName ) );

	// Check if the file is in the current working directory
	wcscpy_s(filePath, MAX_PATH, szFileName);

	strLastSlash = wcsrchr(filePath, TEXT('\\'));
	if (strLastSlash)
	{
		// Chop the exe name from the exe path
		*strLastSlash = 0;

		SetCurrentDirectory(filePath);
		resetCurrentDir = true;
	}

	// open the file
	HANDLE hFile = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	// Get the file size
	LARGE_INTEGER FileSize;
	GetFileSizeEx(hFile, &FileSize);

	// create enough space for the file data
	BYTE* pFileData = new BYTE[FileSize.LowPart];
	if (!pFileData)
		return E_OUTOFMEMORY;

	// read the data in
	DWORD BytesRead;
	if (!ReadFile(hFile, pFileData, FileSize.LowPart, &BytesRead, NULL))
		return E_FAIL;

	CloseHandle(hFile);

	// Create an Include handler instance
	CIncludeHandler* pIncludeHandler = new CIncludeHandler;

	// Compile the shader using optional defines and an include handler for header processing
	ID3DBlob* pErrorBlob;
	hr = D3DCompile(pFileData, FileSize.LowPart, "none", NULL, static_cast< ID3DInclude* > (pIncludeHandler),
		szEntryPoint, szShaderModel, flags, D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS, ppBlobOut, &pErrorBlob);

	delete pIncludeHandler;
	delete[]pFileData;

	// Restore the current working directory if we need to 
	if (resetCurrentDir)
	{
		SetCurrentDirectory(workingPath);
	}


	if (FAILED(hr))
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return hr;
}
HRESULT TShader::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return S_OK;
}
HRESULT TShader::Load(ID3D11Device* pDevice, const TCHAR* strShaderPath,
	LPCSTR strVSEntryPoint,
	LPCSTR strPSEntryPoint,
	LPCSTR strShaderModel,
	UINT Flags)
{
	HRESULT hr;
	ID3DBlob* pBufferErrors = NULL;

	// Create the effect
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	if (strVSEntryPoint == NULL)
	{
		if (m_pBuffer == NULL)
		{
			V_RETURN(CompileShaderFromFile(strShaderPath, Flags, NULL, "fx_5_0", &m_pBuffer));
			V_RETURN(D3DX11CreateEffectFromMemory(m_pBuffer->GetBufferPointer(),
				m_pBuffer->GetBufferSize(),
				dwShaderFlags,
				pDevice,
				&m_pEffect));
			m_pTechnique = m_pEffect->GetTechniqueByName("DefaultRender");
			//SetInputLayout( pDevice, NULL );
		}
	}
	else
	{
		ID3DBlob* pPSBuffer = NULL;
		V_RETURN(CompileShaderFromFile(strShaderPath, strVSEntryPoint, "vs_5_0", &m_pBlobVS));
		V_RETURN(CompileShaderFromFile(strShaderPath, strPSEntryPoint, "ps_5_0", &pPSBuffer));
		V_RETURN(pDevice->CreateVertexShader(m_pBlobVS->GetBufferPointer(),
			m_pBlobVS->GetBufferSize(), NULL, &m_pVertexShader));
		V_RETURN(pDevice->CreatePixelShader(pPSBuffer->GetBufferPointer(),
			pPSBuffer->GetBufferSize(), NULL, &m_pPixelShader));
		pPSBuffer->Release();
		//SetInputLayout( pDevice, m_pBlobVS );
	}

	if (FAILED(hr))
	{
		TCHAR pMessage[256] = { L"NULL" };
		if (pBufferErrors)
		{
			mbstowcs(pMessage, (CHAR*)pBufferErrors->GetBufferPointer(), 256);
			MessageBox(NULL, pMessage, L"Error", MB_OK);
		}
		else
		{
			MessageBox(NULL, strShaderPath, L"Error", MB_OK);
		}
		return hr;
	}
	m_szName = strShaderPath;

	return S_OK;
}
ID3DX11Effect* TShader::GetEffect()
{
	return m_pEffect;
}
bool TShader::Release()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pHullShader);
	SAFE_RELEASE(m_pDomainShader);
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pComputeShader);
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pBlobVS);
	m_InputLayout.Release();
	m_TechMap.clear();
	m_VariMap.clear();
	return true;
}
ID3DX11EffectTechnique* TShader::AddTechnique(CHAR_STRING pWord)
{
	if (m_pEffect == NULL) return NULL;
	for (TechniqueMapItor itor = m_TechMap.begin(); itor != m_TechMap.end(); itor++)
	{
		CHAR_STRING pPoint = (CHAR_STRING)(*itor).first;
		if (pPoint == pWord)
		{
			return (ID3DX11EffectTechnique*)(*itor).second;
		}
	}
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByName(pWord.c_str());
	m_TechMap.insert(make_pair(pWord, pTechnique));
	return pTechnique;
};
ID3DX11EffectTechnique* TShader::GetTechniquePtr(CHAR_STRING pWord)
{
	ID3DX11EffectTechnique* pTechnique = m_TechMap[pWord.c_str()];
	return pTechnique;
};
ID3DX11EffectVariable* TShader::AddVariable(CHAR_STRING Word)
{
	if (m_pEffect == NULL) return NULL;
	for (VariableMapItor itor = m_VariMap.begin(); itor != m_VariMap.end(); itor++)
	{
		CHAR_STRING pPoint = (CHAR_STRING)(*itor).first;
		if (pPoint == Word)
		{
			return (ID3DX11EffectVariable*)(*itor).second;
		}
	}
	// 변수의 이름이 존재하는지 여부를 검증해야 한다.
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(Word.c_str());
	if (pVariable == NULL || pVariable->IsValid() == FALSE)
	{
		return NULL;
	}
	m_VariMap.insert(make_pair(Word, pVariable));
	return pVariable;
};
ID3DX11EffectVariable* TShader::GetVariablePtr(CHAR_STRING String)
{
	ID3DX11EffectVariable* pVariable = m_VariMap[String.c_str()];
	// 구문의 유효성 검사를 한다.
	if (pVariable == NULL || pVariable->IsValid() == FALSE)
	{
		return NULL;
	}
	return pVariable;
};
HRESULT	TShader::SetResource(CHAR_STRING string, ID3D11ShaderResourceView*	pSRV)
{
	HRESULT hr = S_OK;
	ID3DX11EffectVariable* pVari = GetVariablePtr(string);
	if (pVari != NULL)
	{
		ID3DX11EffectShaderResourceVariable* pESRV = pVari->AsShaderResource();
		if (pESRV != NULL)
		{
			hr = pESRV->SetResource(pSRV);
		}
	}
	return hr;
}
wchar_t* TShader::CharToWChar(const char* pString)
{
	assert(pString);
	size_t nLength = strlen(pString) + 1;
	wchar_t* pWString = (wchar_t *)malloc(nLength*sizeof(wchar_t));
	size_t ConvertedChars = 0;
	errno_t retval = mbstowcs_s(&ConvertedChars, pWString, nLength, pString, _TRUNCATE);
	assert(retval == 0 || retval == (size_t)-1);
	return pWString;
}

//std::wstring::size_type string2wstring(std::wstring& wstr,const std::string str)
//{
//    typedef std::string::size_type strsize;
//    typedef std::wstring::size_type wstrsize;
//
//    wchar_t* buff;
//
//    strsize size=str.size()+1;
//    buff=(wchar_t *)malloc(size*sizeof(wchar_t));
//    size=(size_t)mbstowcs(buff,str.c_str(),size);
//    if(size==(size_t)-1)
//        return 0;
//    buff=(wchar_t*)realloc(buff,(size+1)*sizeof(wchar_t));
//    
//    wstr=buff;
//
//    free(buff);
//
//    return wstr.size();
//}
/////////////////////////////////////////////////////////////////////

// wchar -> char
char* TShader::WCharToChar(const wchar_t* pwstrSrc)
{
	assert(pwstrSrc);

#if !defined _DEBUG
	int len = 0;
	len = (wcslen(pwstrSrc) + 1) * 2;
	char* pstr = (char*)malloc(sizeof(char) * len);

	WideCharToMultiByte(949, 0, pwstrSrc, -1, pstr, len, NULL, NULL);
#else

	int nLen = wcslen(pwstrSrc);
	size_t nLength = nLen + 1;
	char* pstr = (char*)malloc(sizeof(char) * nLen + 1);
	size_t numberOfConverted = 0;
	wcstombs_s(&numberOfConverted, pstr, nLength, pwstrSrc, wcslen(pwstrSrc));
#endif

	return pstr;
}
void TShader::ClearShaderResources(ID3D11DeviceContext*  pImmediateContext)
{
	// unbind resources
	ID3D11ShaderResourceView* pNULLViews[8] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	pImmediateContext->VSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->GSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(0, 8, pNULLViews);

}
TShader::TShader(void)
{
	m_pEffect = NULL;
	m_pBuffer = NULL;
	m_szPath = _T("../data/");
	m_pVertexShader = NULL;//정점쉐이더	
	m_pHullShader = NULL;//헐 쉐이더
	m_pDomainShader = NULL;//도메인쉐이더
	m_pGeometryShader = NULL;//기하쉐이더
	m_pPixelShader = NULL;//픽쉘쉐이더
	m_pComputeShader = NULL;//컴퓨터쉐이더
	m_pBlobVS = 0;
}

TShader::~TShader(void)
{
	Release();
}
INT	TShaderMgr::Add(TCHAR* pLoadShaderFile,
	CHAR* pEntryPoint,
	CHAR* pCompiler,
	SHADER_TYPE type)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pLoadShaderFile == NULL || pEntryPoint == NULL) return -1;
	ID3DBlob* pLoadBlob = NULL;

	// 중복 방지 
	if (pLoadShaderFile)
	{
		_tsplitpath_s(pLoadShaderFile, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TShader *pPoint = (TShader *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				pLoadBlob = pPoint->m_pBlobVS;
				break;
			}
		}
	}

	TShader *pPoint = NULL;
	SAFE_NEW(pPoint, TShader);
	pPoint->Load(m_pd3dDevice, pLoadShaderFile, pEntryPoint, pCompiler, type, pLoadBlob);
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	pPoint->m_iIndex = m_iCurIndex;
	pPoint->m_szName = szFileName;
	return m_iCurIndex;
};
INT TShaderMgr::Add(const TCHAR *pLoadShaderFile, LPCSTR strVSEntryPoint,
	LPCSTR strPSEntryPoint,
	LPCSTR strShaderModel)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pLoadShaderFile == NULL) return -1;
	_tsplitpath_s(pLoadShaderFile, Drive, Dir, FName, Ext);
	Ext[4] = 0;
	_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

	// 이펙트가 아닐때는 중복 로드 한다.
	if (strVSEntryPoint==NULL)
	{
		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TShader *pPoint = (TShader *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{				
				return (*itor).first;
			}
		}
	}


	TShader *pPoint = NULL;
	SAFE_NEW(pPoint, TShader);
	pPoint->Load(m_pd3dDevice, pLoadShaderFile, strVSEntryPoint, strPSEntryPoint, strShaderModel);
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	pPoint->m_iIndex = m_iCurIndex;
	pPoint->m_szName = szFileName;
	return m_iCurIndex;
}
bool TShaderMgr::Release()
{
	TShader *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TShader *)(*itor).second;
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
	return true;
}
TShader* TShaderMgr::GetPtr(INT iIndex)
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return NULL;
	TShader *pPoint = (*itor).second;
	return pPoint;
}
void TShaderMgr::SetDevice(ID3D11Device*	 pDevice)
{
	m_pd3dDevice = pDevice;
}
bool TShaderMgr::Delete(INT iDelete)
{
	TemplateMapItor itor = TMap.find(iDelete);
	if (itor == TMap.end()) return true;
	TShader *pPoint = (*itor).second;
	if (pPoint)
	{
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.erase(itor);
	return true;
}
TShaderMgr::TShaderMgr(void)
{
	m_pd3dDevice = NULL;
	m_iCurIndex = 0;
	TMap.clear();
}

TShaderMgr::~TShaderMgr(void)
{
	TShader *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TShader *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();

	m_iCurIndex = 0;
}
