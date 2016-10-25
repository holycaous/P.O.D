#pragma once
#include "stdafx.h"
enum MonsterState
{
	MOVE,
	ATTACK,
	DEATH,
	DAMAGED,
	STAND,
};

enum MonsterType
{
	MONSTERZERO,
	MONSTER_1,
	MONSTER_2,
};
class cMonster_Info : public CObject
{
public:
	cMonster_Info(){};
	cMonster_Info(UINT Type, float HP, float posX, float posY, float posZ);
	virtual ~cMonster_Info();

protected:
	UINT m_Type;


public:
	virtual void Monster_Init(UINT Type, float HP, float posX, float posY, float posZ) = 0;
	virtual void AI_Update(float dt) = 0;
	void SetMonsterType(UINT _mType) { m_Type = _mType; }
	UINT GetMonsterType() { return m_Type; }


};
