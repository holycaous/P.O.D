#include "stdafx.h"

extern CIocpServer IocpServer;


cMonster_1::cMonster_1(UINT Type, float HP, float posX, float posY, float posZ ,float dirX, float dirY, float dirZ)
:cMonster_Info()
{
	m_Type = Type;
	m_HP = HP;
	m_posx = posX;
	m_posy = posY;
	m_posz = posZ;
	m_dirX = dirX;
	m_dirY = dirY;
	m_dirZ = dirZ;

}
void cMonster_1::Monster_Init(int num)
{
	/*m_posx = m_posx;
	m_posy = m_posy;
	m_posz = m_posz;*/
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
	for (int i = 0; i < ServerMGR->GetTerrainServer()->GetTree()->Get_Index(); i++)
	{
		//cout << ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_NodeNum << endl;
		IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_POS, pData, size,
														 ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_NodeNum);
	}
	// 전송.
	//cout << "몬스터 넘버--" <<m_NodeNum <<endl;
	IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_POS, pData, size, m_NodeNum);

	ServerMGR->GetTerrainServer()->GetTree()->Reset_QudeIndex(); //트리노드인덱스 초기화 

	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CMONSTER_POS, pData, size);
}

VOID cMonster_1::AI_Update(float dt, int num, QUAD* p)
{
	//POINT p;
	//p.x = m_posx;
	//p.y = m_posz;
	//QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //쿼드트리에 속한 트리노드를 알아낸다.
	//cout << pQuad->s_P->x <<"----"<< pQuad->s_P->y << endl;


	if (GetMonsterState() == ATTACK) //몬스터가 플레이어에게 공격할수 있는 범위에 들어와 공격.
	{
		BYTE pData[4] = { 0, };
		int size = sizeof(UINT);
		// 클라이언트에게 공격하라는 포지션 전송.
		//IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_ATTACK, pData, size, m_NodeNum);
	}

	if (GetMonsterState() == MOVE) //몬스터가 플레이어를 식별하여 이동
	{
		cout << "몬스터가  " << m_ColisonPlayer<<"-플레이어를 따라감"<<endl;
		
		BYTE pData[8] = { 0, };
		memcpy(pData, &m_ColisonPlayer, sizeof(UINT)); //따라갈 플레이어 넘버
		memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //몬스터 배열넘버
		int size = sizeof(UINT)+sizeof(UINT);
		// 전송. 
		IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_MOVE, pData, size, m_NodeNum);
	}
	if (GetMonsterState() == IDLE) //몬스터 주변에 플레이어가 없을경우 랜덤으로 이동.
	{

		switch (rand() % 4)
		{
		case 0:
		{
				  if (m_posx >p->s_P[1].x || m_posx <p->s_P[2].x)
				  //if (m_posx > pQuad->s_P[1].x || m_posx < pQuad->s_P[2].x)
				  //if (m_posx > 0 || m_posx < 256)
				  {
					  //cout << "m_posx++" << endl;
					  //m_posx++;
				  }
				  break;
		}
		case 1:
		{
				  if (m_posx >p->s_P[1].x || m_posx <p->s_P[2].x)
				  //if (m_posx > pQuad->s_P[1].x || m_posx < pQuad->s_P[2].x)
				 //if (m_posx > 0 || m_posx < 256)
				  {
					  //cout << "m_posx--" << endl;
					  //m_posx--;
				  }
				  break;
		}
		case 2:
		{

				  if (m_posz >p->s_P[1].x || m_posz <p->s_P[2].x)
				 //if (m_posz > pQuad->s_P[1].x || m_posz < pQuad->s_P[2].x)
				  //if (m_posx > 0 || m_posx < 256)
				  {
					 // cout << "m_posz++" << endl;
					  //m_posz++;
				  }
				  break;
		}
		case 3:
		{
				  if (m_posz >p->s_P[1].x || m_posz <p->s_P[2].x)
				 //if (m_posz > pQuad->s_P[1].x || m_posz < pQuad->s_P[2].x)
				  //if (m_posx > 0 || m_posx < 256)
				  {
					 // cout << "m_posz--" << endl;
					  //m_posz--;
				  }
				  break;
		}
		}
	}
}
