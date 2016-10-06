#pragma once

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <list>

using namespace std;

// 悼利 且寸 积己, 秦力
#define SAFE_ZERO(T) T=0;
#define SAFE_NEW(T, tyTe) if(!T) {T=new tyTe;}
#define SAFE_NEW_ARRAY(T, tyTe, size) if(!T) {T=new tyTe[size];}
#define SAFE_DEL(T) {if(!T) delete T; T=NULL;}
#define SAFE_DEL_ARRAY(T) if(!T) {delete []T; T=NULL;}

#define RadianToDegree(Angle) (Angle * (3.14192f / 180.0))

#ifndef singleton
#define singleton

template <class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleton;
		return theSingleton;
	}
};

#endif


