//***************************************************************************************
//
//	Protocol.cpp : 서버 -> 클라 데이터 전송 프로토콜 관련 소스 파일.
//
//  <<함수포인터배열로 구현>> 
//   *함수포인터 배열에 프로토콜 추가 할 시 프로토콜 순서 지킬것.
//
//***************************************************************************************

#include "stdafx.h"
extern CTerrain_Server* g_pTerrain;
extern CIocpServer IocpServer;
cPlayer_Info info;
UINT ClientID; //접속한 클라이언트의 아이디
namespace ServerProtocol_Func{
	Func g_Protocol_Func[Server_Protocol::SPROTOCOL_FULL] =
	{
		ServerProtocol_Func::Connect_Complete_Func,
		ServerProtocol_Func::DisConnect,
		ServerProtocol_Func::Player_Push_W,
		ServerProtocol_Func::Player_Push_S,
		ServerProtocol_Func::Player_Push_A,
		ServerProtocol_Func::Player_Push_D
	}; 

	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{

		printf("클라이언트 로딩 완료 \n");
		/*ServerMGR->TerrainWidth = *(float*)(_pPacket);
		ServerMGR->TerrainDepth = *(float*)(_pPacket+sizeof(float));
		printf("Map Size : %f, %f\n", ServerMGR->TerrainWidth, ServerMGR->TerrainDepth);*/

		for (INT i = MAX_SESSION - 1; i >= 0; --i){
			if (IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetConnected()&&IocpServer.GetConnectUserManager()->GetConnectUser(i)->GetUserStaus() == GAME_END)  //아직 캐릭터 생성이 안되어있으면
			{
				IocpServer.GetConnectUserManager()->GetConnectUser(i)->Player_Init();//플레이어 생성 및 전송
				IocpServer.GetConnectUserManager()->GetConnectUser(i)->SetUserStaus(GAME_ING); //게임상태로 만들어준다.
				//ServerMGR->bGame = true; // 게임 시작 토큰
			}
		}
		//MonsterMGR->INIT(); //몬스터 전송
		//IocpServer.GetConnectUserManager()->WriteAll(CPlAYER_INFO, (BYTE*)ClientID, sizeof(UINT)); //모든 클라이언트에게 접속한 클라의 아이디(클라이언트 수)를 보내준다.

	}
	void DisConnect(BYTE* _pPacket, DWORD& _Length)
	{
		int PlayerID = 0;
		PlayerID = (*_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(PlayerID)->SetUserStaus(GAME_END); //게임종료상태로 만들어준다.
		delete IocpServer.GetConnectUserManager()->GetConnectUser(PlayerID); //플레이어 오브젝트 제거
		printf("클라이언트 접속 종료 \n");

	}
	void Player_Push_W(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::UP);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::UP);
	}
	void Player_Push_S(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::DOWN);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::DOWN);
	}
	void Player_Push_A(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::LEFT);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::LEFT);
	}
	void Player_Push_D(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Player_ID = *(UINT*)(_pPacket);
		IocpServer.GetConnectUserManager()->GetConnectUser(Player_ID)->Get_Playerinfo()->Move(PlayerMoveType::RIGHT);
		//PlayerMGR->GetPlayerInfo(Player_ID)->Move(PlayerMoveType::RIGHT);
	}
}





/****************************** End of File "CSProcess.cpp" ******************************/