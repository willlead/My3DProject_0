#include "WCore.h"
bool     WCore::Init()
{
	return true;
}; // 초기화
bool     WCore::Frame() {
	return true;
};
bool     WCore::Render() {
	return true;
};
bool     WCore::Release() {
	return true;
}
void WCore::MsgEvent(MSG msg)
{
}
bool WCore::GameInit() {
	Init();
	return true;
}
bool WCore::GameRun() {
	GameFrame();
	GameRender();
	return true;
}
bool WCore::GameFrame() {
	Frame();
	return true;
}
bool WCore::GameRender() {
	Render();
	return true;
}
bool WCore::GameRelease() {
	Release();
	return true;
}
bool WCore::Run()
{
	GameInit();
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		// 메세지 큐에서 메세지 1개를 가져온다.
		// 메세지는 편집되지 못한 원시 정보.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// 키 해석
			TranslateMessage(&msg);
			// 가공된 메세지를 프로시져에 전달.
			DispatchMessage(&msg);

			MsgEvent(msg);
		}
		else {
			//Sleep(1);
			GameRun();
		}
	}
	GameRelease();
	return true;
}


WCore::WCore()
{
}


WCore::~WCore()
{
}
