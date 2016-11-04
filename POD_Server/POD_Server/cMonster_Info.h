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
public :
	cMonster_Info(){};
	cMonster_Info(UINT Type, float HP, float posX, float posY, float posZ);
	virtual ~cMonster_Info();
	
protected :
	UINT m_Type;
	UINT m_State;
	//UINT m_MonsterCode; //몬스터 번호
	UINT m_ColisonPlayer;
public :
	virtual void Monster_Init(int num) = 0;
	virtual void Update(float dt,int num) = 0;
	virtual void AI_Update(float dt, int num, QUAD* p) = 0;
	void SetMonsterType(UINT _mType) { m_Type = _mType; }
	UINT GetMonsterType() { return m_Type; }
	void SetMonsterState(UINT _mState, int ClientID) { m_State = _mState; m_ColisonPlayer = ClientID; }
	UINT GetMonsterState() { return m_State; }


};
