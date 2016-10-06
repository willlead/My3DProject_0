#pragma once
#include "Parser.h"

struct WScene
{
	int	iFirstFrame;    // ���� ������
	int	iLastFrame;     // ������ ������
	int	iFrameSpeed;    // 1�ʴ� ������ ����(30)
	int	iTickPerFrame;  // 1�������� ƽ ��(160)
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

	int		i[4];	// *MESH_FACE_LIST ������ >> A:, B:, C:, *MESH_MTLID �̰�, *MESH_TFACELIST ������ i[3] ��� ������� ����
};

struct TColInfo
{

	int		i[4];	// *MESH_FACE_LIST ������ >> A:, B:, C:, *MESH_MTLID �̰�, *MESH_TFACELIST ������ i[3] ��� ������� ����
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
	
	vector<D3DXVECTOR3>		VertexList;		// *MESH_VERTEX_LIST												
	vector<TFaceInfo>		FaceList;		// *MESH_FACE_LIST >> i[0] = A:, i[1] = B:, i[2] = C:, i[3] = *MESH_MTLID
	
	vector<D3DXVECTOR3>		NormVertexList;	// *MESH_NORMALS >> *MESH_VERTEXNORMAL
	vector<D3DXVECTOR3>		NormFaceList;	// *MESH_NORMALS >> *MESH_FACENORMAL

	vector<D3DXVECTOR4>		ColVertexList;	// 0�� ���� 
	vector<TColInfo>		ColFaceList;

	vector<D3DXVECTOR2>		TexVertexList;	// *MESH_TVERTLIST
	vector<TFaceInfo>		TexFaceList;	// *MESH_TFACELIST >> i[3] ��� ������� ����

	int						iMaterialRef;	// *MATERIAL_REF
};

struct PNCTBuffer
{
	vector<PNCT_VERTEX>			m_VertexList;
	ID3D11Buffer*               m_pVertexBuffer = NULL;
	ID3D11Buffer*               m_pIndexBuffer = NULL;
};

struct AnimTrack
{
	int    iTick; //  �ð�(ƽ ����) 
	D3DXQUATERNION  qRotate; ;// ������ �� �� ���͸� ���Ͽ� ������� ��ȯ
	D3DXVECTOR3  vVector; // ��ġ ���� �� ������ ������ Ȱ�� 
	TAnimTrack*  pNext; // ���� Ʈ�� ����Ʈ �ּ� 
	TAnimTrack*  pPrev;// ���� Ʈ�� ����Ʈ �ּ� 
};

struct TAseObject
{
	vector<int>				m_IndexList;
	vector<PNCT_VERTEX>			m_VertexList;
	vector<PNCT_VERTEX>			m_SubVertexList;
	 
	WScene				Scene;
	//

	vector<TMaterialList>	MaterialList;
	//vector<TMaterialList>	SubMaterialList;
	//

	vector<TGeomObject>		ObjectList;
	//

	D3DXMATRIX    m_matWorldTrans;  // �����̵���� 
	D3DXMATRIX    m_matWorldRotate;  // ����ȸ����� 
	D3DXMATRIX    m_matWorldScale;  // ���������� 

	vector<AnimTrack> m_PosTrack;// �̵�Ʈ�� 
	vector<AnimTrack> m_RotTrack;// ȸ��Ʈ�� 
	vector<AnimTrack> m_SclTrack;// ����Ʈ�� 

};




class ParserASE : public Parser
{
public:
	ParserASE();
	virtual ~ParserASE();
public:
	TAseObject	m_ASE;
	vector<PNCTBuffer>  m_pnctBuf;
	int GeomCount = 0;

	DWORD		m_dwTokenIndex;
	DWORD		m_dwMaxStringTable;
	T_STR		m_pwcTokenData;
	vector<T_STR>	m_ListTokens;
public:
	bool	Init();
	bool	Release();
	bool	Read();		// virtual bool bgParser::Read() = 0;
	bool	LoadScene();
	bool	LoadMaterial();
	bool	LoadGeomObject();
	bool	LoadHelperObject();


	//bool	ConvertToModel(TAseObject* pModel);
	//bool ConvertToModel(TAseObject* pModel);

};