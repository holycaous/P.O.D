#include "stdafx.h"

extern CIocpServer IocpServer;


cMonster_1::cMonster_1(UINT Type, float HP, float posX, float posY, float posZ)
:cMonster_Info()
{
	m_Type = Type;
	m_HP = HP;
	m_posx = posX;
	m_posy = posY;
	m_posz = posZ;

}
void cMonster_1::Monster_Init(int num)
{
	m_posx = m_posx + (rand() % 100);
	m_posy = m_posy;
	m_posz = m_posz + (rand() % 200);


	// 데이터 조립.
	//int size = sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	//BYTE pData[24] = { 0, };

	//memcpy(pData, &m_Type, sizeof(UINT)); //몬스터 Type
	//memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //몬스터 배열넘버
	//memcpy(pData + sizeof(UINT)+sizeof(UINT), &m_posx, sizeof(FLOAT));  //몬스터 위치 x좌표
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //몬스터 위치 y좌표
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //몬스터 위치 z좌표
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //몬스터 Hp

	// 전송.
	//IocpServer.GetConnectUserManager()->WriteSOLO(CMONSTER_INIT, pData, size, ServerMGR->ClientCnt);
}
cMonster_1::~cMonster_1(void)
{

}
VOID cMonster_1::Skill1(float dt)
{
	//cout << "스킬1 발동" << endl;
}

VOID cMonster_1::Skill2(float dt)
{ 
	cout << "스킬2 발동" << endl;
}
VOID cMonster_1::Update(float dt, int num)
{
	//cout << "몬스터 업데이트" << endl;
	if (m_HP < 50)
	{ 
		DT = timeGetTime() - ET; //경과시간을 구한다
		ET = timeGetTime(); // 현재시간 초기화
		fDt = DT / 1000.0f;
		fAccdt += fDt; //시간 누적량
		if (fAccdt>1.0f / 300.0f) //5초당 한번 부른다.
			Skill1(fAccdt);
	}
	//데이터 조립.
	int size = sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[24] = { 0, };

	memcpy(pData, &m_Type, sizeof(UINT)); //몬스터 Type
	memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //몬스터 배열넘버
	memcpy(pData + sizeof(UINT)+sizeof(UINT), &m_posx, sizeof(FLOAT));  //몬스터 위치 x좌표
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //몬스터 위치 y좌표
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //몬스터 위치 z좌표
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //몬스터 Hp

	// 전송.
	IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_POS, pData, size, m_NodeNum);
	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CMONSTER_POS, pData, size);

}
VOID cMonster_1::AI_Update(float dt)
{
	switch (rand() % 4)
	{
	case 0:cout << "m_posx++" << endl; m_posx++; break;
	case 1:cout << "m_posx--" << endl; m_posx--; break;
	case 2:cout << "m_posz++" << endl; m_posz++; break;
	case 3:cout << "m_posz--" << endl; m_posz--; break;
	}
}