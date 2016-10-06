#pragma once
#include "Parser.h"

struct WScene
{
	int	iFirstFrame;    // 시작 프레임
	int	iLastFrame;     // 마지막 프레임
	int	iFrameSpeed;    // 1초당 프레임 개수(30)
	int	iTickPerFrame;  // 1프레임의 틱 값(160)
};

struct TMaterialList
{
	//int MaterialCount;
	//vector<wstring> MaterialFileFromPath;
	TCHAR	szName[MAX_PATH];		// *MATERIAL_NAME // class
	int		iMaterialType;
	TCHAR	szBitmap[MAX_PATH];		// *MAP_DIFFUSE >> *BITMAP
	vector<TMaterialList> subMaterial;

};

struct TFaceInfo
{

	int		i[4];	// *MESH_FACE_LIST 에서는 >> A:, B:, C:, *MESH_MTLID 이고, *MESH_TFACELIST 에서는 i[3] 요소 사용하지 않음
};

struct TGeomObject
{
	//D3DXMATRIX matWorld;				//*NODE_TM
	//vector<D3DXVECTOR3> vVertexList;	//*MESH_VERTEX_LIST;
	//vector<D3DXVECTOR3> vFaceList;		//*MESH_FACE_LIST;
	//vector<D3DXVECTOR3> TVertList;		//*MESH_TVERTLIST;
	//vector<D3DXVECTOR3> TFaceList;		//*MESH_TFACELIST;
	//vector<TNormals>	TNormalList;		//*MESH_NORMALS;
	//int					MATERIAL_REF;	//*MATERIAL_REF;	

	D3DXMATRIX				matWorld;		// *TM_ROW0~3
											//int						iNumVertex;
	vector<D3DXVECTOR3>		VertexList;		// *MESH_VERTEX_LIST	
											//int						iNumFace;
	vector<TFaceInfo>		FaceList;		// *MESH_FACE_LIST >> i[0] = A:, i[1] = B:, i[2] = C:, i[3] = *MESH_MTLID
	vector<D3DXVECTOR2>		TexVertexList;	// *MESH_TVERTLIST
	vector<TFaceInfo>		TexFaceList;	// *MESH_TFACELIST >> i[3] 요소 사용하지 않음
	vector<D3DXVECTOR4>		ColVertexList;	// 0이 고정 
	vector<D3DXVECTOR3>		NormVertexList;	// *MESH_NORMALS >> *MESH_VERTEXNORMAL
	vector<D3DXVECTOR3>		NormFaceList;	// *MESH_NORMALS >> *MESH_FACENORMAL
	int						iMaterialRef;	// *MATERIAL_REF
};

struct TAseObject
{
	vector<int>				m_IndexList;
	vector<PNCT_VERTEX>			m_VertexList;
	WScene				Scene;
	//

	vector<TMaterialList>	MaterialList;
	vector<TMaterialList>	SubMaterialList;
	//

	vector<TGeomObject>		ObjectList;
	//
};


class ParserASE : public Parser
{
public:
	ParserASE();
	virtual ~ParserASE();
public:
	TAseObject	m_ASE;
	int GeomCount = 0;
public:
	bool	Init();
	bool	Release();
	bool	Read();		// virtual bool bgParser::Read() = 0;
	bool	LoadScene();
	bool	LoadMaterial();
	bool	LoadGeomObject();
	bool	ConvertToModel(TAseObject* pModel);
	//bool ConvertToModel(TAseObject* pModel);
};