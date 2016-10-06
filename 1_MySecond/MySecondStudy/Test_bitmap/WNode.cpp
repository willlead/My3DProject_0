#include "WNode.h"
#include "WSys.h"

#define g_NodeCount 0
#define NODE_MAX_SIZE 10

//bool Cmp(int& num)
//{
//
//}
//
//void WNode::NodeSwap(std::vector<NodeType*> vData, int i)
//{
//	
//		vData[i] = NULL;
//}

void WNode::findNode(int num)
{
	std::vector<NodeType*>::iterator _S = vData.begin();
	std::vector<NodeType*>::iterator _L = vData.end();


	//if ( )	{

	//}

	int FindData = vData[num]->data;	
	{
		for (;_S!=_L ;_S++)
		{
			if ((*_S)->data == FindData)
			{
				//vData[i]->
				delete(*_S);
				*_S = NULL;
			}
		}
		_S = vData.begin();
		while (_S != vData.end())
		{
			if (*_S == NULL)
			{
				_S = vData.erase(_S);
			}
			else
			{
				_S++;
			}

		}
	}
	

}
void WNode::CreateNode()
{

	//for (int i = 0; i < NodeCnt; i++)
	//{
	//	m_iNodeType[i].data = rand() % 3;
	//	m_iNodeType[i].rect.x = 140+i * 100;
	//	m_iNodeType[i].rect.y = 590;
	//	m_iNodeType[i].rect.w = 98;
	//	m_iNodeType[i].rect.h = 98;
	//}

	for (int i = 0; i < NODE_MAX_SIZE ; i++)
	{
		NodeType* temp = new NodeType;
		//temp->index = i;
		temp->data = rand() % 3;
		//temp->rect.x = 140 + (i * 100);
		temp->rect.y = 590;
		temp->rect.w = 98;
		temp->rect.h = 98;

		vData.push_back(temp);

		//vData.clear();
	}

	//for (int i = 0; i < NodeCnt; i++)
	//{
	//	NodeType* temp = new NodeType;
	//	temp->index = i;
	//	temp->data=rand() % 3;
	//	temp->rect.x = 140 + (temp->index * 100);
	//	temp->rect.y = 590;
	//	temp->rect.w = 98;
	//	temp->rect.h = 98;

	//	lData.push_back(temp);

	////	vData.clear();
	//}

}

void WNode::DrawNode()
{
	for (int i = 0; i < vData.size(); i++)
	{
		//BitBlt(g_hOffScreenDC,
		//	lData._Get_data(lData);
		//	lData[i]->rect.x,
		//	lData[i]->rect.y,
		//	lData[i]->rect.w,
		//	lData[i]->rect.h,
		//	m_NodeBitmap[lData[i]->data].m_hMemDC,
		//	0, 0, SRCCOPY);

		vData[i]->index = i;
		vData[i]->rect.x = 140 + (vData[i]->index * 100);

		BitBlt(g_hOffScreenDC,
			vData[i]->rect.x,
			vData[i]->rect.y,
			vData[i]->rect.w,
			vData[i]->rect.h,
			m_NodeBitmap[vData[i]->data].m_hMemDC,
			0, 0, SRCCOPY);

		//BitBlt(g_hOffScreenDC,
		//	m_iNodeType[i].rect.x,
		//	m_iNodeType[i].rect.y,
		//	m_iNodeType[i].rect.w,
		//	m_iNodeType[i].rect.h,
		//	m_NodeBitmap[m_iNodeType[i].data].m_hMemDC,
		//	0, 0, SRCCOPY);
	}
}
bool WNode::Init()
{
	//m_NodeBitmap;
	if (!m_NodeBitmap[0].Load(_T("../../data/NH_Node_1.bmp")))
	{
		return false;
	}
	if (!m_NodeBitmap[1].Load(_T("../../data/NH_Node_2.bmp")))
	{
		return false;
	}
	if (!m_NodeBitmap[2].Load(_T("../../data/NH_Node_3.bmp")))
	{
		return false;
	}

	//   m_SNodeBitmap;
	if (!m_SNodeBitmap[0].Load(_T("../../data/NH_Node_1.bmp")))
	{
		return false;
	}
	if (!m_SNodeBitmap[1].Load(_T("../../data/NH_SNode_2.bmp")))
	{
		return false;
	}
	if (!m_SNodeBitmap[2].Load(_T("../../data/NH_SNode_3.bmp")))
	{
		return false;
	}

	CreateNode();
	//NodeCollision();
	return true;
}
bool WNode::Frame()
{
	return true;
}
bool WNode::Render()
{
	DrawNode();
	return true;
}
bool WNode::Release()
{
	return true;
}
WNode::WNode()
{

}
WNode::~WNode()
{
}
