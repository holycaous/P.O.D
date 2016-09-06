#include "StdAfx.h"

cMonster_Info::cMonster_Info(UINT Type, float HP, float posX, float posY, float posZ)
:CObject()
{
	m_Type = Type;
	m_HP = HP;
	m_posx = posX;
	m_posy = posY;
	m_posz = posZ;


}
cMonster_Info::~cMonster_Info(void)
{
}

