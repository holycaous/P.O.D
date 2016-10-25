#include "stdafx.h"

extern CIocpServer IocpServer; 
extern CTerrain_Server* g_pTerrain;


void cPlayer_Info::Update(float _fDTime)
{
	if (m_HP <= 0)
		m_blive = FALSE;
	// 데이터 조립.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, }; 

	memcpy(pData, &m_nClientID, sizeof(UINT)); //플레이어 ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //플레이어 위치 x좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //플레이어 위치 y좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //플레이어 위치 z좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //플레이어 Hp
	
	// 전송.
	for (int i = 0; i < ServerMGR->GetTerrainServer()->GetTree()->Get_Index(); i++)
	{  
		cout << ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_nNum << endl;
		IocpServer.GetConnectUserManager()->WriteCulling(CPLAYER_POS, pData, size, 
														 ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_nNum);
	}
	ServerMGR->GetTerrainServer()->GetTree()->Reset_QudeIndex(); //트리노드인덱스 초기화
	//IocpServer.GetConnectUserManager()->WriteCulling(CPLAYER_POS, pData, size, m_NodeNum);
	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CPLAYER_POS, pData, size);

}
void cPlayer_Info::PlayerInit(int ID, float pos_x, float pos_y, float pos_z, float hp)
{
	m_posx = pos_x;
	//m_posx = pos_x + (rand() % 100);
	m_posy = pos_y;
	m_posz = pos_z;
	//m_posz = pos_z +(rand() % 200);
	m_HP = hp;
	m_nClientID = ID;
	
	 //데이터 조립.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT)); //플레이어 ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //플레이어 위치 x좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //플레이어 위치 y좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //플레이어 위치 z좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //플레이어 Hp

	IocpServer.GetConnectUserManager()->WriteSOLO(CSTART_INFO, pData, size, m_nClientID);
	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CSTART_INFO, pData, size);
}

void cPlayer_Info::PlayerInfo()
{
	if (m_HP <= 0) 
		m_blive = FALSE;

 	// 데이터 조립.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT)); //플레이어 ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //플레이어 위치 x좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //플레이어 위치 y좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //플레이어 위치 z좌표
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //플레이어 Hp

	// 전송.
	IocpServer.GetConnectUserManager()->WriteGameClientAll(CPLAYER_INFO, pData, size);
}
VOID	cPlayer_Info::Move(int state)
{
	switch (state)
	{
	case 0:
		cout << "업키" << endl;
		m_posx = m_posx + 20;
		break;
	case 1:
		cout << "다운키" << endl;
		break;
	case 2:
		cout << "레프트키" << endl;
		break;
	case 3:
		cout << "라이트키" << endl;
		break;
	}
}
void	cPlayer_Info::PlayerCollison()
{
	float Monster_x;
	float Monster_y;
	float Monster_z;

	float SerchRange = 10; //몬스터탐색범위
	float AttackRange = 2; //몬스터공격범위


	//*1.몬스터가 플레이어와의  감지거리에 들어올 경우는  MOVE상태로 변경  2.몬스터가 플레이어를 공격할수있는 거리일 경우는 ATTACK상태로 변경
	for (int i = 0; i < MAX_MONSTER; i++)
	{
		Monster_x=MonsterMGR->GetMonsterInfo(i)->GetPositionX()-m_posx;
		Monster_y=MonsterMGR->GetMonsterInfo(i)->GetPositionY()-m_posy; 
		Monster_z=MonsterMGR->GetMonsterInfo(i)->GetPositionZ()-m_posz;

		float Distance = sqrt((Monster_x*Monster_x) + (Monster_y*Monster_y) + (Monster_z*Monster_z)); //플레이어와 몬스터간의 거리 
		
		 if (AttackRange >= Distance)
		{
			//몬스터의 상태를 ATTACK 상태로 변경하여 플레이어를 공격하게 만든다.
			MonsterMGR->GetMonsterInfo(i)->SetMonsterState(MonsterState::ATTACK);
		}
		 else if (SerchRange >= Distance)
		 {
			 cout << "몬스터 감지" << endl;
			 //몬스터의 상태를 MOVE로 변경하여 플레이어를 쫒아가게 만든다.
			 MonsterMGR->GetMonsterInfo(i)->SetMonsterState(MonsterState::MOVE);
		 }
		else
		{
			//몬스터의 상태를 IDLE 상태로 변경
			MonsterMGR->GetMonsterInfo(i)->SetMonsterState(MonsterState::IDLE);
		}
		
	}
} 