#include "stdafx.h"

cMonster_1::cMonster_1(UINT Type, float HP, float posX, float posY, float posZ)
:cMonster_Info()
{
	m_Type = Type;
	m_HP = HP;
	m_Pos_x = posX;
	m_Pos_y = posY;
	m_Pos_z = posZ;

}
void cMonster_1::Monster_Init(UINT Type, float HP, float posX, float posY, float posZ)
{
	m_Type = Type;
	m_HP = HP;
	m_Pos_x = posX;
	m_Pos_y = posY;
	m_Pos_z = posZ;
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
VOID cMonster_1::Update(float dt)
{


}
VOID cMonster_1::AI_Update(float dt)
{

}