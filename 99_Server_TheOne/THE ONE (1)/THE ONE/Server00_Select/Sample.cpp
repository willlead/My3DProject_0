#include "Select.h"

int main()
{
	I_Select.Set(10000);
	if (I_Select.Init() == false)
	{
		printf("윈속 초기화 오류\n");
		return 0;
	}
	if (I_Select.Run() == false)
	{
		printf("서버가 비정상 종료되었습니다.\n");
		return 0;
	}
	printf("서버가 정상 종료되었습니다.\n");
	I_Select.Release();

	return 0;
}