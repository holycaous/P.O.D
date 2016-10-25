// POD_Server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <ctime>
#define S_FPS 1.0f/60.0f

CIocpServer IocpServer;

int _tmain(int argc, _TCHAR* argv[])
{
	// winsock을 사용하기 위한 DLL 로드

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	ServerMGR->TerrainInit(&(ServerMGR->Terrain_info)); //맵 초기화
	MonsterMGR->Begin(); //몬스터 초기화

	DWORD ET = timeGetTime(); //현재시간
	DWORD DT = timeGetTime();//경과시간
	float fDt = 0.f;
	float fAccdt = 0.f;
	srand((unsigned)time(NULL));
	// 서버 시작.
	if (IocpServer.Begin())
	{ 
		ServerMGR->Init();//서버 초기화
		while (1)
		{
			DT = timeGetTime() - ET; //나중시간- 현재시간 =경과시간
			ET = timeGetTime();  //현재시간 초기화
			fDt = DT / 1000.0f; 
			fAccdt += fDt; //시간 누적량

			if (fAccdt > S_FPS){ 				
			/*	if (ServerMGR->bGame){
					for (int i = MAX_SESSION - 1; i >= 0; --i) {
						if (IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetConnected())
							IocpServer.GetConnectUserManager()->GetConnectUser(i)->Update(fAccdt);
					}*/
					for (int i = MAX_SESSION - 1; i >= 0; --i) {
						if (IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetUserStaus() == GAME_ING)
							IocpServer.GetConnectUserManager()->GetConnectUser(i)->Update(fAccdt);
					}
					MonsterMGR->Update(fAccdt); //몬스터 업데이트
				 
				fAccdt = 0.0f; 
			}
		}
		getchar();
		IocpServer.End();
	}
		ServerMGR->MemoryDelete(); //동적할당 해제
		MonsterMGR->End(); //몬스터 동적할당 해제

		// 종료시 DLL을 언로드 시킵니다.
		WSACleanup();

		CoUninitialize();
		return 0;
	

}
