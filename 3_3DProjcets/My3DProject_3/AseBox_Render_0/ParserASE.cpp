#include "ParserASE.h"

bool ParserASE::ConvertToModel(TAseObject* pModel)
{
	int iFace;      // 페이스 반복
	int iTriangle;   // 삼각형 반복(페이스의 3개 정점)
	int iIndex;      // 모든 정점마다의 고유한 인덱스 (페이스 * 삼각형(3))

	pModel->m_IndexList.resize(m_ASE.ObjectList[0].FaceList.size() * 3);   // 실제 랜더링될 정점들의 인덱스여야 하므로 최대값 (페이스 * 3)
	pModel->m_VertexList.resize(m_ASE.ObjectList[0].FaceList.size() * 3);   // 실제 버텍스 정보이므로... 중복 제거 필요함!

	for (iFace = 0; iFace < m_ASE.ObjectList[0].FaceList.size(); iFace++)   // 루프 한번에 페이스 하나(정점3개)에 대한 처리
	{
		for (iTriangle = 0; iTriangle < 3; iTriangle++)
		{
			// 인덱스 버퍼용 값 저장
			iIndex = iFace * 3 + iTriangle;
			pModel->m_IndexList[iIndex] = m_ASE.ObjectList[0].FaceList[iFace].i[iTriangle];

			// 버텍스 버퍼용 값 저장
			PNCT_VERTEX vertexTemp;
			vertexTemp.p = D3DXVECTOR3(m_ASE.ObjectList[0].VertexList[m_ASE.ObjectList[0].FaceList[iFace].i[iTriangle]]);
			vertexTemp.c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);// D3DXVECTOR4(m_ASE.ObjectList[0].ColVertexList[m_ASE.ObjectList[0].ColFaceList[iFace].i[iTriangle]], 1.0f);
			vertexTemp.t = D3DXVECTOR2(m_ASE.ObjectList[0].TexVertexList[m_ASE.ObjectList[0].TexFaceList[iFace].i[iTriangle]].x,
				m_ASE.ObjectList[0].TexVertexList[m_ASE.ObjectList[0].TexFaceList[iFace].i[iTriangle]].y);
			vertexTemp.n = D3DXVECTOR3(m_ASE.ObjectList[0].NormVertexList[iIndex]);
			pModel->m_VertexList[iIndex] = vertexTemp;
		}
	}
	return true;
}

bool ParserASE::LoadScene()
{

	// SCENE_FIRSTFRAME
	if (!FindWord(L"*SCENE_FIRSTFRAME")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &m_ASE.Scene.iFirstFrame);
	//SCENE_LASTFRAME
	if (!FindWord(L"*SCENE_LASTFRAME")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &m_ASE.Scene.iLastFrame);
	//*SCENE_FRAMESPEED 
	if (!FindWord(L"*SCENE_FRAMESPEED")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &m_ASE.Scene.iFrameSpeed);
	//*SCENE_TICKSPERFRAME 
	if (!FindWord(L"*SCENE_TICKSPERFRAME")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &m_ASE.Scene.iTickPerFrame);
	// } END_SECTION
	//if (!FindWord(L"}"))return false;

	return true;

}
bool ParserASE::LoadMaterial()
{
	int i, iMax, iTemp;
	if (!FindWord(L"*MATERIAL_COUNT")) false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &iTemp);
	//L 은 유니코드 _T 시스템 설정에 ?? L로 사용해도된다.
	m_ASE.MaterialList.resize(iTemp);
	iMax = m_ASE.MaterialList.size();
	for (i = 0; i < iMax; i++)
	{
		if (!FindWord(L"*MATERIAL")) return false;
		if (!FindWord(L"*MATERIAL_NAME")) return false; // class ? name?
		ZeroMemory(m_ASE.MaterialList[i].szName, MAX_PATH);
		_stscanf(m_szBuffer, _T("%s%s"), m_szWord, m_ASE.MaterialList[i].szName);

		if (!FindWord(L"*MAP_DIFFUSE")) return false;
		if (!FindWord(L"*BITMAP")) return false;
		ZeroMemory(m_ASE.MaterialList[i].szBitmap, MAX_PATH);
		_stscanf(m_szBuffer, _T("%s%s"), m_szWord, m_ASE.MaterialList[i].szBitmap);

		//if (!FindWord(L"}"))return false;
		//if (!FindWord(L"}"))return false;
	}

	return true;
}
bool ParserASE::LoadGeomObject()
{
	int iLoop, iMax, iTemp;
	TFaceInfo fiTemp;
	D3DXVECTOR3 v3Temp;

	if (!FindWord(L"*NODE_TM")) return false;
	D3DXMatrixIdentity(&m_ASE.ObjectList[GeomCount].matWorld);
	if (!FindWord(L"*TM_ROW0")) return false;
	_stscanf(m_szBuffer, _T("%s%f%f%f"), m_szWord, &v3Temp.x, &v3Temp.y, &v3Temp.z);
	m_ASE.ObjectList[GeomCount].matWorld._11 = v3Temp.x;
	m_ASE.ObjectList[GeomCount].matWorld._12 = v3Temp.z;
	m_ASE.ObjectList[GeomCount].matWorld._13 = v3Temp.y;
	//m_ASE.ObjectList[0].matWorld._14 = 0.0f;

	if (!FindWord(L"*TM_ROW1")) return false;
	_stscanf(m_szBuffer, _T("%s%f%f%f"), m_szWord, &v3Temp.x, &v3Temp.y, &v3Temp.z);
	m_ASE.ObjectList[GeomCount].matWorld._31 = v3Temp.x;
	m_ASE.ObjectList[GeomCount].matWorld._32 = v3Temp.z;
	m_ASE.ObjectList[GeomCount].matWorld._33 = v3Temp.y;
	//m_ASE.ObjectList[0].matWorld._44 = 0.0f;

	if (!FindWord(L"*TM_ROW2")) return false;
	_stscanf(m_szBuffer, _T("%s%f%f%f"), m_szWord, &v3Temp.x, &v3Temp.y, &v3Temp.z);
	m_ASE.ObjectList[GeomCount].matWorld._21 = v3Temp.x;
	m_ASE.ObjectList[GeomCount].matWorld._22 = v3Temp.z;
	m_ASE.ObjectList[GeomCount].matWorld._23 = v3Temp.y;
	//m_ASE.ObjectList[0].matWorld._34 = 0.0f;

	if (!FindWord(L"*TM_ROW3")) return false;
	_stscanf(m_szBuffer, _T("%s%f%f%f"), m_szWord, &v3Temp.x, &v3Temp.y, &v3Temp.z);
	m_ASE.ObjectList[GeomCount].matWorld._41 = v3Temp.x;
	m_ASE.ObjectList[GeomCount].matWorld._42 = v3Temp.z;
	m_ASE.ObjectList[GeomCount].matWorld._43 = v3Temp.y;
	//m_ASE.ObjectList[0].matWorld._44 = 1.0f;	

	if (!FindWord(L"*MESH")) return false;

	if (!FindWord(L"*MESH_NUMVERTEX")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &iTemp);
	m_ASE.ObjectList[GeomCount].VertexList.resize(iTemp);
	////m_ASE.ObjectList[GeomCount].iNumVertex = iTemp;

	if (!FindWord(L"*MESH_NUMFACES")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &iTemp);
	m_ASE.ObjectList[GeomCount].FaceList.resize(iTemp);
	////m_ASE.ObjectList[GeomCount].iNumFace = iTemp;
	{
		if (!FindWord(L"*MESH_VERTEX_LIST")) return false;
		iMax = m_ASE.ObjectList[GeomCount].VertexList.size();
		for (iLoop = 0; iLoop < iMax; iLoop++)
		{
			if (!FindWord(L"*MESH_VERTEX"))return false;
			_stscanf(m_szBuffer, _T("%s%d%f%f%f"), m_szWord, &iTemp, &v3Temp.x, &v3Temp.y, &v3Temp.z);
			m_ASE.ObjectList[GeomCount].VertexList[iLoop].x = v3Temp.x;
			m_ASE.ObjectList[GeomCount].VertexList[iLoop].y = v3Temp.z;
			m_ASE.ObjectList[GeomCount].VertexList[iLoop].z = v3Temp.y;
		}
		//if (!FindWord(L"}"))return false; //EndSection;
	}//End Mesh

	if (!FindWord(L"*MESH_FACE_LIST")) return false;
	{
		iMax = m_ASE.ObjectList[GeomCount].FaceList.size();
		for (iLoop = 0; iLoop < iMax; iLoop++)
		{
			if (!FindWord(L"*MESH_FACE")) return false;
			//fiTemp.i[3] = 0;
			_stscanf(m_szBuffer, _T("%s%s %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d"),
				m_szWord, m_szWord,
				m_szWord, &fiTemp.i[0],
				m_szWord, &fiTemp.i[2],
				m_szWord, &fiTemp.i[1],
				m_szWord, &iTemp,
				m_szWord, &iTemp,
				m_szWord, &iTemp,
				m_szWord, &iTemp,
				m_szWord, &fiTemp.i[3]);
			m_ASE.ObjectList[GeomCount].FaceList[iLoop] = fiTemp;
		}
	}//end for faceList
	 //if (!FindWord(L"}")) return false;

	 //TVERLIST
	if (!FindWord(L"*MESH_NUMTVERTEX")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &iTemp);
	m_ASE.ObjectList[GeomCount].TexVertexList.resize(iTemp);

	if (!FindWord(L"*MESH_TVERTLIST")) return false;
	iMax = m_ASE.ObjectList[GeomCount].TexVertexList.size();
	for (iLoop = 0; iLoop < iMax; iLoop++)
	{
		if (!FindWord(L"*MESH_TVERT")) return false;
		_stscanf(m_szBuffer, _T("%s%d%f%f%f"), m_szWord, &iTemp, &v3Temp.x, &v3Temp.y, &v3Temp.z); 
		
		m_ASE.ObjectList[GeomCount].TexVertexList[iLoop] = v3Temp;
	}// end for TVerlist
	 //if (!FindWord(L"}")) return false;

	 //*MESH_NUMTVFACES 
	if (!FindWord(L"*MESH_NUMTVFACES")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &iTemp);
	m_ASE.ObjectList[GeomCount].TexFaceList.resize(iTemp);

	if (!FindWord(L"*MESH_TFACELIST")) return false;
	iMax = m_ASE.ObjectList[GeomCount].TexFaceList.size();
	for (iLoop = 0; iLoop < iMax; iLoop++)
	{
		if (!FindWord(L"*MESH_TFACE")) return false;
		_stscanf(m_szBuffer, _T("%s%d%d%d%d"), m_szWord, &iTemp,
			&fiTemp.i[0], &fiTemp.i[2], &fiTemp.i[1]);
		fiTemp.i[3] = 0;
		m_ASE.ObjectList[GeomCount].TexFaceList[iLoop] = fiTemp;
	}// end for FaceList;
	 //if (!FindWord(L")")) return false;


	 // CVerTex
	if (!FindWord(L"*MESH_NUMCVERTEX")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &iTemp);
	m_ASE.ObjectList[GeomCount].ColVertexList.resize(iTemp);

	//Normals
	if (!FindWord(L"*MESH_NORMALS")) return false;
	iMax = m_ASE.ObjectList[GeomCount].FaceList.size();
	m_ASE.ObjectList[GeomCount].NormFaceList.resize(iMax);
	m_ASE.ObjectList[GeomCount].NormVertexList.resize(iMax * 3);
	for (iLoop = 0; iLoop < iMax; iLoop++)
	{
		if (!FindWord(L"*MESH_FACENORMAL")) return false;
		_stscanf(m_szBuffer, _T("%s%d%f%f%f"), m_szWord, &iTemp, &v3Temp.x, &v3Temp.z, &v3Temp.y);
		m_ASE.ObjectList[GeomCount].NormFaceList[iLoop] = v3Temp;

		if (!FindWord(L"*MESH_VERTEXNORMAL")) return false;
		_stscanf(m_szBuffer, _T("%s%d%f%f%f"), m_szWord, &iTemp, &v3Temp.x, &v3Temp.z, &v3Temp.y);
		m_ASE.ObjectList[GeomCount].NormVertexList[iLoop * 3 + 0] = v3Temp;

		if (!FindWord(L"*MESH_VERTEXNORMAL")) return false;
		_stscanf(m_szBuffer, _T("%s%d%f%f%f"), m_szWord, &iTemp, &v3Temp.x, &v3Temp.z, &v3Temp.y);
		m_ASE.ObjectList[GeomCount].NormVertexList[iLoop * 3 + 1] = v3Temp;

		if (!FindWord(L"*MESH_VERTEXNORMAL")) return false;
		_stscanf(m_szBuffer, _T("%s%d%f%f%f"), m_szWord, &iTemp, &v3Temp.x, &v3Temp.z, &v3Temp.y);
		m_ASE.ObjectList[GeomCount].NormVertexList[iLoop * 3 + 2] = v3Temp;

	}// end for Normals

	 //if (!FindWord(L"}")) return false;
	 //if (!FindWord(L"}")) return false;

	return true;
}

ParserASE::ParserASE()
{
	//SAFE_ZERO(m_ParserASE);
}


ParserASE::~ParserASE()
{
}
bool ParserASE::Init()
{
	return true;
}

bool ParserASE::Read()
{
	//////SCENE
	if (FindWord(L"*SCENE")) 
		LoadScene();
	
	//*MATERIAL_COUNT
	if (FindWord(L"*MATERIAL_LIST"))
		LoadMaterial();


	m_ASE.ObjectList.resize(1);
	if (FindWord(L"*GEOMOBJECT"))
		LoadGeomObject();


	if (!FindWord(L"*MATERIAL_REF ")) return false;
	_stscanf(m_szBuffer, _T("%s%d"), m_szWord, &m_ASE.ObjectList[GeomCount].iMaterialRef);

	//if (!FindWord(L"}")) return false;
	return true;

}
