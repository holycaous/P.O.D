#include "stdafx.h"

extern CIocpServer IocpServer; 
extern CTerrain_Server* g_pTerrain;


void cPlayer_Info::Update(float _fDTime)
{
	if (m_HP <= 0)
		m_blive = FALSE;
	// ������ ����.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT)); //�÷��̾� ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //�÷��̾� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //�÷��̾� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //�÷��̾� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //�÷��̾� Hp
	
	// ����.
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
	
	 //������ ����.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT)); //�÷��̾� ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //�÷��̾� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //�÷��̾� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //�÷��̾� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //�÷��̾� Hp

	IocpServer.GetConnectUserManager()->WriteSOLO(CSTART_INFO, pData, size, m_nClientID);
	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CSTART_INFO, pData, size);
}

void cPlayer_Info::PlayerInfo()
{
	if (m_HP <= 0) 
		m_blive = FALSE;

	// ������ ����.
	int size = sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[20] = { 0, };

	memcpy(pData, &m_nClientID, sizeof(UINT)); //�÷��̾� ID
	memcpy(pData + sizeof(UINT), &m_posx, sizeof(FLOAT));  //�÷��̾� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //�÷��̾� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //�÷��̾� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //�÷��̾� Hp

	// ����.
	IocpServer.GetConnectUserManager()->WriteGameClientAll(CPLAYER_INFO, pData, size);
}
VOID	cPlayer_Info::Move(int state)
{
	switch (state)
	{
	case 0:
		cout << "��Ű" << endl;
		m_posx = m_posx + 20;
		break;
	case 1:
		cout << "�ٿ�Ű" << endl;
		break;
	case 2:
		cout << "����ƮŰ" << endl;
		break;
	case 3:
		cout << "����ƮŰ" << endl;
		break;
	}
}