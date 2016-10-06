#include "bgParserASE.h"

TCHAR*	szWord[] =
{
	_T("}"),		// ASE_END_OF_SECTION
	_T("*SCENE"),
	_T("*SCENE_FIRSTFRAME"),
	_T("*SCENE_LASTFRAME"),
	_T("*SCENE_FRAMESPEED"),
	_T("*SCENE_TICKSPERFRAME"),
	_T("*MATERIAL_LIST"),
	_T("*MATERIAL_COUNT"),
	_T("*MATERIAL"),
	_T("*MATERIAL_NAME"),
	_T("*NUMSUBMTLS"),
	_T("*SUBMATERIAL"),
	_T("*MAP_DIFFUSE"),
	_T("*MAP_SUBNO"),
	_T("*BITMAP"),
	_T("*GEOMOBJECT"),
	_T("*NODE_TM"),
	_T("*TM_ROW0"),
	_T("*TM_ROW1"),
	_T("*TM_ROW2"),
	_T("*TM_ROW3"),
	_T("*MESH"),
	_T("*MESH_NUMVERTEX"),
	_T("*MESH_NUMFACES"),
	_T("*MESH_VERTEX_LIST"),
	_T("*MESH_VERTEX"),
	_T("*MESH_FACE_LIST"),
	_T("*MESH_FACE"),
	_T("*MESH_NUMTVERTEX"),
	_T("*MESH_TVERTLIST"),
	_T("*MESH_TVERT"),
	_T("*MESH_NUMTVFACES"),
	_T("*MESH_TFACELIST"),
	_T("*MESH_TFACE"),
	_T("*MESH_NUMCVERTEX"),
	_T("*MESH_CVERTLIST"),
	_T("*MESH_VERTCOL"),
	_T("*MESH_NUMCVFACES"),
	_T("*MESH_CFACELIST"),
	_T("*MESH_CFACE"),
	_T("*MESH_NORMALS"),
	_T("*MESH_FACENORMAL"),
	_T("*MESH_VERTEXNORMAL"),
	_T("*MATERIAL_REF"),
};

bgParserASE::bgParserASE()
{
}

bgParserASE::~bgParserASE()
{
}

bool bgParserASE::Init()
{
	return true;
}

bool bgParserASE::Read()
{
	int iLoop, iLoopSub, iLoopMax, iLoopSubMax, iData;
	FaceInfo i4Data;
	D3DXVECTOR3 v3Data;

	///// SCENE - �� ������
	if (!FindWord(szWord[ASE_SCENE])) return false;						//	*SCENE {
	if (!FindWord(szWord[ASE_SCENE_FIRSTFRAME])) return false;			//		*SCENE_FIRSTFRAME
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &m_ASE.Scene.iFirstFrame);
	if (!FindWord(szWord[ASE_SCENE_LASTFRAME])) return false;			//		*SCENE_LASTFRAME
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &m_ASE.Scene.iLastFrame);
	if (!FindWord(szWord[ASE_SCENE_FRAMESPEED])) return false;			//		*SCENE_FRAMESPEED
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &m_ASE.Scene.iFrameSpeed);
	if (!FindWord(szWord[ASE_SCENE_TICKSPERFRAME])) return false;		//		*SCENE_TICKSPERFRAME
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &m_ASE.Scene.iTicksPerFrame);
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//	}

	///// MATERIAL_LIST - ���͸��� ����Ʈ
	if (!FindWord(szWord[ASE_MATERIAL_LIST])) return false;				//	*MATERIAL_LIST {
	if (!FindWord(szWord[ASE_MATERIAL_COUNT])) return false;			//		*MATERIAL_COUNT
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &iData);
	m_ASE.MaterialList.resize(iData);

	TCHAR* pszToken;
	iLoopMax = m_ASE.MaterialList.size();
	for (iLoop = 0; iLoop < iLoopMax; iLoop++)
	{
		if (!FindWord(szWord[ASE_MATERIAL])) return false;				//		*MATERIAL {
		if (!FindWord(szWord[ASE_MATERIAL_NAME])) return false;			//			*MATERIAL_NAME
		ZeroMemory(m_ASE.MaterialList[iLoop].szName, MAX_PATH);
		pszToken = _tcstok(m_szLine, _T("\""));
		pszToken = _tcstok(NULL, _T("\""));
		_tcscpy(m_ASE.MaterialList[iLoop].szName, pszToken);

		if (!FindWord(szWord[ASE_MAP_DIFFUSE])) return false;			//			*MAP_DIFFUSE {
		if (!FindWord(szWord[ASE_BITMAP])) return false;				//				*BITMAP
		ZeroMemory(m_ASE.MaterialList[iLoop].szBitmap, MAX_PATH);
		pszToken = _tcstok(m_szLine, _T("\""));
		pszToken = _tcstok(NULL, _T("\""));
		_tcscpy(m_ASE.MaterialList[iLoop].szBitmap, pszToken);

		if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;		//			}
		if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;		//		}
	}
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//	}

	///// GEOMOBJECT - ������Ʈ�� ������Ʈ
	m_ASE.ObjectList.resize(1);
	if (!FindWord(szWord[ASE_GEOMOBJECT])) return false;				//	*GEOMOBJECT {
	if (!FindWord(szWord[ASE_NODE_TM])) return false;					//		*NODE_TM {
	if (!FindWord(szWord[ASE_TM_ROW0])) return false;					//			*TM_ROW0
	_stscanf(m_szLine, _T("%s%f%f%f"), m_szWord, &v3Data.x, &v3Data.z, &v3Data.y);
	m_ASE.ObjectList[0].matWorld._11 = v3Data.x;
	m_ASE.ObjectList[0].matWorld._12 = v3Data.y;
	m_ASE.ObjectList[0].matWorld._13 = v3Data.z;
	m_ASE.ObjectList[0].matWorld._14 = 0.0f;

	if (!FindWord(szWord[ASE_TM_ROW1])) return false;					//			*TM_ROW1
	_stscanf(m_szLine, _T("%s%f%f%f"), m_szWord, &v3Data.x, &v3Data.z, &v3Data.y);
	m_ASE.ObjectList[0].matWorld._31 = v3Data.x;
	m_ASE.ObjectList[0].matWorld._32 = v3Data.y;
	m_ASE.ObjectList[0].matWorld._33 = v3Data.z;
	m_ASE.ObjectList[0].matWorld._34 = 0.0f;

	if (!FindWord(szWord[ASE_TM_ROW2])) return false;					//			*TM_ROW2
	_stscanf(m_szLine, _T("%s%f%f%f"), m_szWord, &v3Data.x, &v3Data.z, &v3Data.y);
	m_ASE.ObjectList[0].matWorld._21 = v3Data.x;
	m_ASE.ObjectList[0].matWorld._22 = v3Data.y;
	m_ASE.ObjectList[0].matWorld._23 = v3Data.z;
	m_ASE.ObjectList[0].matWorld._24 = 0.0f;

	if (!FindWord(szWord[ASE_TM_ROW3])) return false;					//			*TM_ROW3
	_stscanf(m_szLine, _T("%s%f%f%f"), m_szWord, &v3Data.x, &v3Data.z, &v3Data.y);
	m_ASE.ObjectList[0].matWorld._41 = v3Data.x;
	m_ASE.ObjectList[0].matWorld._42 = v3Data.y;
	m_ASE.ObjectList[0].matWorld._43 = v3Data.z;
	m_ASE.ObjectList[0].matWorld._44 = 1.0f;

	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//		}

	///// MESH - �޽� ������
	if (!FindWord(szWord[ASE_MESH])) return false;						//		*MESH {
	if (!FindWord(szWord[ASE_MESH_NUMVERTEX])) return false;			//			*MESH_NUMVERTEX
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &iData);
	m_ASE.ObjectList[0].VertexList.resize(iData);

	if (!FindWord(szWord[ASE_MESH_NUMFACES])) return false;				//			*MESH_NUMFACES
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &iData);
	m_ASE.ObjectList[0].FaceList.resize(iData);

	///// VERTEX_LIST - ���ؽ� ����Ʈ
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse, NULL, &m_ASE.ObjectList[0].matWorld);

	if (!FindWord(szWord[ASE_MESH_VERTEX_LIST])) return false;			//			*MESH_VERTEX_LIST {
	iLoopMax = m_ASE.ObjectList[0].VertexList.size();
	for (iLoop = 0; iLoop < iLoopMax; iLoop++)
	{
		if (!FindWord(szWord[ASE_MESH_VERTEX])) return false;			//				*MESH_VERTEX
		_stscanf(m_szLine, _T("%s%d%f%f%f"), m_szWord, &iData, &v3Data.x, &v3Data.z, &v3Data.y);
		D3DXVec3TransformCoord(&v3Data, &v3Data, &matInverse);
		m_ASE.ObjectList[0].VertexList[iLoop] = v3Data;
	}
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//			}

	///// FACE_LIST - ���̽� ����Ʈ
	if (!FindWord(szWord[ASE_MESH_FACE_LIST])) return false;			//			*MESH_FACE_LIST {
	iLoopMax = m_ASE.ObjectList[0].FaceList.size();
	for (iLoop = 0; iLoop < iLoopMax; iLoop++)
	{
		if (!FindWord(szWord[ASE_MESH_FACE])) return false;				//				*MESH_FACE
		_stscanf(m_szLine, _T("%s%s %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d"),
			m_szWord, m_szWord,			// *MESH_FACE  0:
			m_szWord, &i4Data.i[0],
			m_szWord, &i4Data.i[2],
			m_szWord, &i4Data.i[1],		// A:  B:  C:
			m_szWord, &iData,
			m_szWord, &iData,
			m_szWord, &iData,			// AB: BC: CA:
			m_szWord, &iData,			// *MESH_SMOOTHING
			m_szWord, &i4Data.i[3]);	// *MESH_MTLID
		m_ASE.ObjectList[0].FaceList[iLoop] = i4Data;
	}
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//			}

	if (!FindWord(szWord[ASE_MESH_NUMTVERTEX])) return false;			//			*MESH_NUMTVERTEX
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &iData);
	m_ASE.ObjectList[0].TexVertexList.resize(iData);

	///// TVERTLIST - �ؽ��� ���ؽ� ����Ʈ
	if (!FindWord(szWord[ASE_MESH_TVERTLIST])) return false;			//			*MESH_TVERTLIST {
	iLoopMax = m_ASE.ObjectList[0].TexVertexList.size();
	for (iLoop = 0; iLoop < iLoopMax; iLoop++)
	{
		if (!FindWord(szWord[ASE_MESH_TVERT])) return false;			//				*MESH_TVERT
		_stscanf(m_szLine, _T("%s%d%f%f%f"), m_szWord, &iData, &v3Data.x, &v3Data.y, &v3Data.z);
		m_ASE.ObjectList[0].TexVertexList[iLoop] = v3Data;
	}
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//			}

	if (!FindWord(szWord[ASE_MESH_NUMTVFACES])) return false;			//			*MESH_NUMTVFACES
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &iData);
	m_ASE.ObjectList[0].TexFaceList.resize(iData);

	///// TFACELIST - �ؽ��� ���̽� ����Ʈ
	if (!FindWord(szWord[ASE_MESH_TFACELIST])) return false;			//			*MESH_TFACELIST {
	iLoopMax = m_ASE.ObjectList[0].TexFaceList.size();
	for (iLoop = 0; iLoop < iLoopMax; iLoop++)
	{
		if (!FindWord(szWord[ASE_MESH_TFACE])) return false;			//				*MESH_TFACE
		_stscanf(m_szLine, _T("%s%d%d%d%d"), m_szWord, &iData, &i4Data.i[0], &i4Data.i[2], &i4Data.i[1]);
		i4Data.i[3] = 0;
		m_ASE.ObjectList[0].TexFaceList[iLoop] = i4Data;
	}
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//			}

	///// CVERTLIST - �÷� ���ؽ�&���̽� ����Ʈ
	if (!FindWord(szWord[ASE_MESH_NUMCVERTEX])) return false;			//			*MESH_NUMCVERTEX
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &iData);
	if (iData == 0) // ASE ���Ͽ� ���� ������ ���ٸ� ���ǻ��� �߰�
	{
		// ���ؽ� �÷�
		iLoopMax = m_ASE.ObjectList[0].VertexList.size();
		m_ASE.ObjectList[0].ColVertexList.resize(iLoopMax);
		for (iLoop = 0; iLoop < iLoopMax; iLoop++)
		{
			m_ASE.ObjectList[0].ColVertexList[iLoop] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}

		// �÷��� �ε���
		iLoopMax = m_ASE.ObjectList[0].FaceList.size();
		m_ASE.ObjectList[0].ColFaceList.resize(iLoopMax);
		for (iLoop = 0; iLoop < iLoopMax; iLoop++)
		{
			m_ASE.ObjectList[0].ColFaceList[iLoop] = m_ASE.ObjectList[0].FaceList[iLoop];
		}
	}
	else // ASE ���Ͽ� ���� ������ �ִٸ� �ش���� ����
	{
		// ���ؽ� �÷�
		iLoopMax = iData;
		m_ASE.ObjectList[0].ColVertexList.resize(iLoopMax);
		if (!FindWord(szWord[ASE_MESH_CVERTLIST])) return false;		//			*MESH_CVERTLIST {
		for (iLoop = 0; iLoop < iLoopMax; iLoop++)
		{
			if (!FindWord(szWord[ASE_MESH_VERTCOL])) return false;		//				*MESH_VERTCOL
			_stscanf(m_szLine, _T("%s%d%f%f%f"), m_szWord, &iData, &v3Data.x, &v3Data.y, &v3Data.z);
			m_ASE.ObjectList[0].ColVertexList[iLoop] = v3Data;
		}
		if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;		//			}

		// �÷��� �ε���
		if (!FindWord(szWord[ASE_MESH_NUMCVFACES])) return false;		//			*MESH_NUMCVFACES
		_stscanf(m_szLine, _T("%s%d"), m_szWord, &iData);
		iLoopMax = iData;
		m_ASE.ObjectList[0].ColFaceList.resize(iLoopMax);
		if (!FindWord(szWord[ASE_MESH_CFACELIST])) return false;		//			*MESH_CFACELIST {
		for (iLoop = 0; iLoop < iLoopMax; iLoop++)
		{
			if (!FindWord(szWord[ASE_MESH_CFACE])) return false;		//				*MESH_CFACE
			_stscanf(m_szLine, _T("%s%d%d%d%d"), m_szWord, &iData, &i4Data.i[0], &i4Data.i[2], &i4Data.i[1]);
			i4Data.i[3] = 0;
			m_ASE.ObjectList[0].ColFaceList[iLoop] = i4Data;
		}
		if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;		//			}
	}

	///// NORMALS - �븻 ���ؽ�&���̽� ����Ʈ
	if (!FindWord(szWord[ASE_MESH_NORMALS])) return false;				//			*MESH_NORMALS {
	iLoopMax = m_ASE.ObjectList[0].FaceList.size();
	m_ASE.ObjectList[0].NorFaceList.resize(iLoopMax);
	m_ASE.ObjectList[0].NorVertexList.resize(iLoopMax * 3);
	for (iLoop = 0; iLoop < iLoopMax; iLoop++)
	{
		if (!FindWord(szWord[ASE_MESH_FACENORMAL])) return false;		//				*MESH_FACENORMAL
		_stscanf(m_szLine, _T("%s%d%f%f%f"), m_szWord, &iData, &v3Data.x, &v3Data.z, &v3Data.y);
		m_ASE.ObjectList[0].NorFaceList[iLoop] = v3Data;

		if (!FindWord(szWord[ASE_MESH_VERTEXNORMAL])) return false;		//					*MESH_VERTEXNORMAL
		_stscanf(m_szLine, _T("%s%d%f%f%f"), m_szWord, &iData, &v3Data.x, &v3Data.z, &v3Data.y);
		m_ASE.ObjectList[0].NorVertexList[iLoop * 3 + 0] = v3Data;

		if (!FindWord(szWord[ASE_MESH_VERTEXNORMAL])) return false;		//					*MESH_VERTEXNORMAL
		_stscanf(m_szLine, _T("%s%d%f%f%f"), m_szWord, &iData, &v3Data.x, &v3Data.z, &v3Data.y);
		m_ASE.ObjectList[0].NorVertexList[iLoop * 3 + 2] = v3Data;

		if (!FindWord(szWord[ASE_MESH_VERTEXNORMAL])) return false;		//					*MESH_VERTEXNORMAL
		_stscanf(m_szLine, _T("%s%d%f%f%f"), m_szWord, &iData, &v3Data.x, &v3Data.z, &v3Data.y);
		m_ASE.ObjectList[0].NorVertexList[iLoop * 3 + 1] = v3Data;
	}
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//			}
	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//		}

	if (!FindWord(szWord[ASE_MATERIAL_REF])) return false;				//		*MATERIAL_REF
	_stscanf(m_szLine, _T("%s%d"), m_szWord, &m_ASE.ObjectList[0].iMaterialRef);

	if (!FindWord(szWord[ASE_END_OF_SECTION])) return false;			//	}

	return true;
}

bool bgParserASE::ConvertToModel(bgModel* pModel)
{
	int iFace;			// ���̽� �ݺ�
	int iTriangle;		// �ﰢ�� �ݺ�(���̽��� 3�� ����)
	int iIndex;			// ��� ���������� ������ �ε��� (���̽� * �ﰢ��(3))

	pModel->m_IndexList.resize(m_ASE.ObjectList[0].FaceList.size() * 3);	// ���� �������� �������� �ε������� �ϹǷ� �ִ밪 (���̽� * 3)
	pModel->m_VertexList.resize(m_ASE.ObjectList[0].FaceList.size() * 3);	// ���� ���ؽ� �����̹Ƿ�... �ߺ� ���� �ʿ���!

	for (iFace = 0; iFace < m_ASE.ObjectList[0].FaceList.size(); iFace++)	// ���� �ѹ��� ���̽� �ϳ�(����3��)�� ���� ó��
	{
		for (iTriangle = 0; iTriangle < 3; iTriangle++)
		{
			// �ε��� ���ۿ� �� ����
			iIndex = iFace * 3 + iTriangle;
			pModel->m_IndexList[iIndex] = iIndex;

			// ���ؽ� ���ۿ� �� ����
			VertexPNCT vertexTemp;
			vertexTemp.pos = D3DXVECTOR3(m_ASE.ObjectList[0].VertexList[m_ASE.ObjectList[0].FaceList[iFace].i[iTriangle]]);
			vertexTemp.col = D3DXVECTOR4(m_ASE.ObjectList[0].ColVertexList[m_ASE.ObjectList[0].ColFaceList[iFace].i[iTriangle]], 1.0f);
			vertexTemp.tex = D3DXVECTOR2(m_ASE.ObjectList[0].TexVertexList[m_ASE.ObjectList[0].TexFaceList[iFace].i[iTriangle]].x,
								  1.0f - m_ASE.ObjectList[0].TexVertexList[m_ASE.ObjectList[0].TexFaceList[iFace].i[iTriangle]].y);
			vertexTemp.nor = D3DXVECTOR3(m_ASE.ObjectList[0].NorVertexList[iIndex]);
			pModel->m_VertexList[iIndex] = vertexTemp;
		}
	}

	pModel->m_TextureIDList.resize(m_ASE.MaterialList.size());
	for (int iBitmap = 0; iBitmap < m_ASE.MaterialList.size(); iBitmap++)
		pModel->m_TextureIDList[iBitmap] = I_TextureMgr.Add(m_ASE.MaterialList[0].szBitmap);

	/* �ߺ����� �ڵ�... ���� �۾�
	for (iFace = 0; iFace < m_ASE.ObjectList[0].FaceList.size(); iFace++) // ���� �ѹ��� ���̽� �ϳ�(����3��)�� ���� ó��
	{
		for (iTriangle = 0; iTriangle < 3; iTriangle++)
		{
			// iIndexTemp�� �ӽ÷� *MESH_FACE�� A: B: C: ���� �ֱ�
			int iIndexTemp = m_ASE.ObjectList[0].FaceList[iFace].i[iTriangle];
			pModel->m_IndexList[iFace * 3 + iTriangle] = iIndexTemp;

			// ���ؽ�����Ʈ�� ���� �ڿ������� ���ʴ�� Ž��
			// (iFace�� ���� Ŀ������ �迭�� �޺κп� ������ ���ɼ��� ũ�Ƿ�...)
			for (iVertex = pModel->m_VertexList.size() - 1; iVertex >= 0; iVertex--)
			{
				// �� ������ PCTN�� ������ �߰����� �ʰ� �ѱ�
				if ()
				{

				}
				// �� ������ PCTN�� ���� �ٸ��� ���ؽ��� �߰�
				else
				{
					VertexPNCT vertexTemp;
					vertexTemp.pos = m_ASE.ObjectList[0].VertexList[iIndexTemp];
					vertexTemp.col = m_ASE.ObjectList[0].ColVertexList[iIndexTemp];
					vertexTemp.tex.x = m_ASE.ObjectList[0].TexVertexList[].x;
					vertexTemp.tex.y = m_ASE.ObjectList[0].TexVertexList[].y;
					vertexTemp.nor = m_ASE.ObjectList[0].NorVertexList[iFace * 3 + iTriangle];
					pModel->m_VertexList.push_back(vertexTemp);
				}
			}
		}
	}
	*/
	return true;
}

/*
ship.ase

*MESH_NUM     VERTEX 2656
*MESH_NUM     FACES  4158
*MESH_NUM Tex VERTEX 3803
*MESH_NUM Tex VFACES 4158
*MESH_NUM Col VERTEX 2656
*MESH_NUM Col VFACES 4158
*MESH_  FACE  NORMAL 4158
*MESH_ VERTEX NORMAL 4158*3
*/
