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
	//IocpServer.GetConnectUserManager()->WriteCulling(CPLAYER_POS, pData, size, m_NodeNum);
	IocpServer.GetConnectUserManager()->WriteGameClientAll(CPLAYER_POS, pData, size);

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