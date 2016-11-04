#pragma once
#include "stdafx.h"
enum MonsterState
{
	MOVE,
	ATTACK,
	DEATH,
	DAMAGED,
	IDLE,
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
	UINT m_State; //몬스터 상태
	UINT m_ColisonPlayer; //충돌플레이어

public:
	virtual void Monster_Init(UINT Type, float HP, float posX, float posY, float posZ) = 0;
	virtual void AI_Update(float dt) = 0;
	void SetMonsterType(UINT _mType) { m_Type = _mType; }
	UINT GetMonsterType() { return m_Type; }

	void SetMonsterState(UINT _mState, int ClientID) { m_State = _mState; m_ColisonPlayer = ClientID;}
	UINT GetMonsterState() { return m_State; }
	
	UINT GetCollisonPlayer() { return m_ColisonPlayer; }


};
