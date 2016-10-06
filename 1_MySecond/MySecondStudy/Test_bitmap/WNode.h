#pragma once
#include "WStd.h"
#include "WBitmap.h"
#include <vector>
#include <list>
#define NodeCnt 10

class WNode
{
public:

	std::vector<NodeType*> vData;
	std::list<NodeType*> lData;

	NodeType	   m_iNodeType[NodeCnt];
	
	WBitmap    m_NodeBitmap[3];
	WBitmap    m_SNodeBitmap[3];

	WRect m_rtCollide;
	WRect m_rt;

public:
	void CreateNode();
	//void NodeSwap(std::vector<NodeType*> vData,int i);
	void findNode(int num);
	void DrawNode();

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	WNode();
	virtual ~WNode();
};

