//***************************************************************************************
 //
//	Protocol.cpp : 클라 -> 서버 데이터 전송 프로토콜 관련 소스 파일.
//
//  <<함수포인터배열로 구현>> 
//   *함수포인터 배열에 프로토콜 추가 할 시 프로토콜 순서 지킬것.
//
//***************************************************************************************

#include "stdafx.h"

namespace ClientProtocol_Func{
	Func g_Protocol_Func[Client_Protocol::CPROTOCOL_FULL] =
	{
		ClientProtocol_Func::Connect_Complete_Func,
		ClientProtocol_Func::DisConnect,
		ClientProtocol_Func::Loading_Start_Func,
		ClientProtocol_Func::Player_Count,
		ClientProtocol_Func::Player_Info,
		ClientProtocol_Func::Player_Pos,
		ClientProtocol_Func::Monster_Init,
		ClientProtocol_Func::Monster_Pos,
		ClientProtocol_Func::Monster_Move,
	};
	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length)
	{
		printf("서버에 접속 \n");
		Player_MGR->MyID = *_pPacket;
		Player_MGR->GetConnectUser(Player_MGR->MyID)->SetClientID(Player_MGR->MyID);//서버에서 받은 플레이어 ID 등록
		BYTE Byte = 1;
		Cla_ServerMGR->ClientNet()->WritePacket(SCONNET_COMPLETE, &Byte, 1); //서버에게 서버접속 패킷전송
		return;
	}
	void DisConnect(BYTE* _pPacket, DWORD& _Length)
	{
		BYTE ID;
		ID = Player_MGR->GetConnectUser(Player_MGR->MyID)->GetClientID();//서버에서 받은 플레이어 ID 가져오기
		Cla_ServerMGR->ClientNet()->WritePacket(SONIODISCONNECT, &ID, 1); //클라이언트에게 접속종료 패킷 전송
		printf("클라이언트 접속 종료 \n");

	}
	void Loading_Start_Func(BYTE* _pPacket, DWORD& _Length)
	{

		/*UINT Player_ID = *(UINT*)(_pPacket);
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT));
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));*/
		

		UINT Player_ID = *(UINT*)(_pPacket);
		UINT Unique_Code = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));


		cout << Player_ID<<" -- [플레이어 접속]  ";
		cout << "유니크 코드--" << Unique_Code << endl;
	
		if (Player_MGR->MyID == 0) Player_MGR->MyID = Player_ID; //나의 플레이어 ID 지정 
		       		
		//PlayerInfo[] 벡터에 플레이어 정보 입력
		Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);//플레이어 ID 입력
		Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);//플레이어 Code 입력
		Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);//플레이어 x좌표 입력
		Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);//플레이어 y좌표 입력
		Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);//플레이어 z좌표 입력
		Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);//플레이어 HP입력
	
		//Player_MGR->GetConnectUser(Player_ID)->SetObjectInitFlag(true); //플레이어패킷을 플레이어백터에 집어넣었다는 플래그 ON
		Player_MGR->GetConnectUser(Player_ID)->SetDrawFlag(true); //플레이어를 클라이언트에게 그리라고 명령하는 플래그 ON

		return;
	}
	void Player_Count(BYTE* _pPacket, DWORD& _Length)
	{
		Cla_ServerMGR->ClientNet()->m_nClientCnt = *_pPacket;
		printf("현재 접속인원 :%d명 \n", Cla_ServerMGR->ClientNet()->m_nClientCnt);
		
		return;
	}
	void Player_Info(BYTE* _pPacket, DWORD& _Length)
	{
		/*	UINT Player_ID = *(UINT*)(_pPacket);
			FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT));
			FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT));
			FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
			FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));*/
		
		    UINT Player_ID = *(UINT*)(_pPacket);
		    UINT Unique_Code = *(UINT*)(_pPacket + sizeof(UINT));
		    FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		    FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
	     	FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
	     	FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));
			
			//PlayerInfo[] 벡터에 플레이어 정보 입력
			Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);//플레이어 ID 입력
			Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);//플레이어 Code 입력
			Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);//플레이어 x좌표 입력
			Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);//플레이어 y좌표 입력
			Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);//플레이어 z좌표 입력
			Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);//플레이어 HP입력 
			
			if (Player_MGR->MyID != Player_ID)
			{
				cout << Player_ID << " -- [플레이어 정보]" << endl;
			}
			

	}
	void Player_Pos(BYTE* _pPacket, DWORD& _Length)
	{
		/*UINT Player_ID = *(UINT*)(_pPacket);
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)); 
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));*/

		UINT Player_ID = *(UINT*)(_pPacket);
		UINT Unique_Code = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Player_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Player_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Player_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Player_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));
		
		//cout << Player_ID << " -- [플레이어 정보]" << "X좌표 --" << Player_x << endl;
		if (Player_MGR->GetConnectUser(Player_ID)->GetObjectInitFlag() != TRUE) //아직 플레이어 벡터에 집어넣기 전이라면
		{
			cout << Player_ID<< "번째 플레이어 데이터 벡터에 입력" << endl;
			Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);
			Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);
			Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);

			//Player_MGR->GetConnectUser(Player_ID)->SetObjectInitFlag(true); //플레이어패킷을 플레이어백터에 집어넣었다는 플래그 ON
			Player_MGR->GetConnectUser(Player_ID)->SetDrawFlag(true); //플레이어를 클라이언트에게 그리라고 명령하는 플래그 ON
		}
		else //플레이어백터에 있는 플레이어라면 데이터를 입력해준다.
		{
			Player_MGR->GetConnectUser(Player_ID)->SetClientID(Player_ID);
			Player_MGR->GetConnectUser(Player_ID)->Set_UniqueCode(Unique_Code);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posx(Player_x);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posy(Player_y);
			Player_MGR->GetConnectUser(Player_ID)->Set_Posz(Player_z);
			Player_MGR->GetConnectUser(Player_ID)->Set_HP(Player_HP);
		}


		
	}
	void Monster_Init(BYTE* _pPacket, DWORD& _Length)
	{
		cout << "몬스터 이닛" << endl;
		UINT Monster_Type = *(UINT*)(_pPacket);
		UINT Monster_Num = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Monster_ID_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Monster_ID_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Monster_ID_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Monster_ID_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));
		//cout << "몬스터 초기화  정보데이터-- " << Monsyer_Num << " 번째 몬스터--" << endl;
		//Monster_MGR->Init(Monster_Type, Monster_Num, Monster_ID_x, Monster_ID_y,Monster_ID_z, Monster_ID_HP);

	}
	void Monster_Pos(BYTE* _pPacket, DWORD& _Length)
	{
		UINT Monster_Type = *(UINT*)(_pPacket);
		UINT Monster_Num = *(UINT*)(_pPacket + sizeof(UINT));
		FLOAT Monster_ID_x = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT));
		FLOAT Monster_ID_y = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT));
		FLOAT Monster_ID_z = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT));
		FLOAT Monster_ID_HP = *(FLOAT*)(_pPacket + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT));

		//몬스터 정보 입력
		Monster_MGR->Init(Monster_Type, Monster_Num, Monster_ID_x, Monster_ID_y, Monster_ID_z, Monster_ID_HP);
	
		
	}
	void Monster_Move(BYTE* _pPacket, DWORD& _Length)
	{
		//cout << "몬스터 무브"<<endl;
		UINT Player_Num = *(UINT*)(_pPacket); //따라갈 플레이어 번호
		UINT Monster_Num = *(UINT*)(_pPacket + sizeof(UINT)); //몬스터 번호

		Monster_MGR->GetConnectMonster(Monster_Num)->SetMonsterState(MonsterState::MOVE, Player_Num);
		
	}

	
}





/****************************** End of File "CSProcess.cpp" ******************************/