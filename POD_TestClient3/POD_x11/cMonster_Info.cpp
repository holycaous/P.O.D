#include "StdAfx.h"

cMonster_Info::cMonster_Info(UINT Type, float HP, float posX, float posY, float posZ)
:CObject()
{
	m_Type = Type;
	m_HP = HP;
	m_Pos_x = posX;
	m_Pos_y = posY;
	m_Pos_z = posZ;

}
cMonster_Info::~cMonster_Info(void)
{
}

