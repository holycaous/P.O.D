#include "stdafx.h"
#include "Main.h"

// 전역 인스턴스
cCam gCam;

// 윈 메인
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// 런타임 메모리 체크(디버그 모드)
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//네트워크 초기화 
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 메모리 누수 검색
	//_CrtSetBreakAlloc(154);

	// 엔진 클래스 인스턴스 생성
	cEngine Application(hInstance);

	// 엔진 클래스 초기화
	if (!Application.Init())
		return 0;

	// 엔진 클래스 실행
	return Application.Run();
}
