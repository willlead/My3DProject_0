// RTTI.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include <typeinfo.h>
using namespace std;

#define DECLARE_CLASSNAME static char lpszClassName[]
#define IMPLEMENT_CLASSNAME(s) char s##::lpszClassName[] = (#s);
#define RUNTIME_CLASS(s) (&s::class##s)
// CAlpha::classCAlpha

class CObject;

struct CRuntimeClass
{
	char m_lpszClassName[20];
	int m_iObjectSize;
	CObject* (*pfnCreateObject)();
	CObject* CreateObject();
};
CObject* CRuntimeClass::CreateObject()
{
	return (*pfnCreateObject)();
}

class CObject {
public:
	virtual CRuntimeClass* GetRuntimeClass() const { return NULL; }
	static CRuntimeClass classCObject;
	virtual char* GetClassName() const	{		return NULL;	} //�ڿ� �ܽ�Ʈ�� ������������

public:
	virtual ~CObject(){}
protected:
	CObject() {	}	
};
CRuntimeClass CObject::classCObject = { "CObject",sizeof(CObject), NULL }; // �ʱ�ȭ


class  CAlpha : public CObject
{
public:	
	virtual CRuntimeClass* GetRuntimeClass() const
	{
		return &classCAlpha;
	}
	static CRuntimeClass classCAlpha;
	DECLARE_CLASSNAME;//static char lpszClassName[];
	static CObject* CreateObject();
	//char* GetClassName()const { return m_lpszClassName; }

protected:
	CAlpha() {};
};
CRuntimeClass CAlpha::classCAlpha = { "CAlpha",sizeof(CAlpha),CAlpha::CreateObject };
CObject* CAlpha::CreateObject()
{
	return new CAlpha;
}


IMPLEMENT_CLASSNAME(CAlpha); //char CAlpha::lpszClassName[] = "CAlpha";
class CBase {};
class CChild :public CBase {  };
int main()
{

	CRuntimeClass* pRT = RUNTIME_CLASS(CAlpha);
	CObject* pObj = pRT->CreateObject();
	cout << pObj->GetRuntimeClass()->m_lpszClassName << endl;
	delete pObj;


	return 0;
}

