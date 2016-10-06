#pragma once
#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T>*		m_pHead;
	Node<T>*		m_pTail;

public:
	void			InsertLink(T*);
	void			DeleteLink(Node<T>*);
	T*		    	Find(TCHAR*);
	void			Free();

public:
	LinkedList();
	~LinkedList();
};

