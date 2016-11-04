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

	// ������ ����.
	//int size = sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	//BYTE pData[24] = { 0, };

	//memcpy(pData, &m_Type, sizeof(UINT)); //���� Type
	//memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //���� �迭�ѹ�
	//memcpy(pData + sizeof(UINT)+sizeof(UINT), &m_posx, sizeof(FLOAT));  //���� ��ġ x��ǥ
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //���� ��ġ y��ǥ
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //���� ��ġ z��ǥ
	//memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //���� Hp

	// ����.
	//IocpServer.GetConnectUserManager()->WriteSOLO(CMONSTER_INIT, pData, size, ServerMGR->ClientCnt);
}
cMonster_1::~cMonster_1(void)
{

}
VOID cMonster_1::Skill1(float dt)
{
	//cout << "��ų1 �ߵ�" << endl;
}

VOID cMonster_1::Skill2(float dt)
{ 
	cout << "��ų2 �ߵ�" << endl;
}
VOID cMonster_1::Update(float dt, int num)
{
	//cout << "���� ������Ʈ" << endl;
	if (m_HP < 50)
	{
		DT = timeGetTime() - ET; //����ð��� ���Ѵ�
		ET = timeGetTime(); // ����ð� �ʱ�ȭ
		fDt = DT / 1000.0f;
		fAccdt += fDt; //�ð� ������
		if (fAccdt>1.0f / 300.0f) //5�ʴ� �ѹ� �θ���.
			Skill1(fAccdt);
	}
	//������ ����.
	int size = sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT);
	BYTE pData[24] = { 0, };

	memcpy(pData, &m_Type, sizeof(UINT)); //���� Type
	memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //���� �迭�ѹ�
	memcpy(pData + sizeof(UINT)+sizeof(UINT), &m_posx, sizeof(FLOAT));  //���� ��ġ x��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT), &m_posy, sizeof(FLOAT));  //���� ��ġ y��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT), &m_posz, sizeof(FLOAT)); //���� ��ġ z��ǥ
	memcpy(pData + sizeof(UINT)+sizeof(UINT)+sizeof(FLOAT)+sizeof(FLOAT)+sizeof(FLOAT), &m_HP, sizeof(FLOAT)); //���� Hp

	// ����.
	for (int i = 0; i < ServerMGR->GetTerrainServer()->GetTree()->Get_Index(); i++)
	{
		//cout << ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_NodeNum << endl;
		IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_POS, pData, size,
														 ServerMGR->GetTerrainServer()->GetTree()->GetQudeIndex(i)->s_NodeNum);
	}
	// ����.
	//cout << "���� �ѹ�--" <<m_NodeNum <<endl;
	IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_POS, pData, size, m_NodeNum);

	ServerMGR->GetTerrainServer()->GetTree()->Reset_QudeIndex(); //Ʈ������ε��� �ʱ�ȭ 

	//IocpServer.GetConnectUserManager()->WriteGameClientAll(CMONSTER_POS, pData, size);
}

VOID cMonster_1::AI_Update(float dt, int num, QUAD* p)
{
	//POINT p;
	//p.x = m_posx;
	//p.y = m_posz;
	//QUAD *pQuad = ServerMGR->GetTerrainServer()->GetTree()->GetTopNode()->IsIn(p); //����Ʈ���� ���� Ʈ����带 �˾Ƴ���.
	//cout << pQuad->s_P->x <<"----"<< pQuad->s_P->y << endl;


	if (GetMonsterState() == ATTACK) //���Ͱ� �÷��̾�� �����Ҽ� �ִ� ������ ���� ����.
	{
		BYTE pData[4] = { 0, };
		int size = sizeof(UINT);
		// Ŭ���̾�Ʈ���� �����϶�� ������ ����.
		//IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_ATTACK, pData, size, m_NodeNum);
	}

	if (GetMonsterState() == MOVE) //���Ͱ� �÷��̾ �ĺ��Ͽ� �̵�
	{
		cout << "���Ͱ�  " << m_ColisonPlayer<<"-�÷��̾ ����"<<endl;
		
		BYTE pData[8] = { 0, };
		memcpy(pData, &m_ColisonPlayer, sizeof(UINT)); //���� �÷��̾� �ѹ�
		memcpy(pData + sizeof(UINT), &num, sizeof(UINT)); //���� �迭�ѹ�
		int size = sizeof(UINT)+sizeof(UINT);
		// ����. 
		IocpServer.GetConnectUserManager()->WriteCulling(CMONSTER_MOVE, pData, size, m_NodeNum);
	}
	if (GetMonsterState() == IDLE) //���� �ֺ��� �÷��̾ ������� �������� �̵�.
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
