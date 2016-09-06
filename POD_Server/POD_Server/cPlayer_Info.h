#pragma once
#include "stdafx.h"

enum{
	CHARECTOR_STOP =0,
	CHARECTOR_MOVE,
	CHARECTOR_ATTACK,
};
enum PlayerMoveType
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class cPlayer_Info : public CObject
{
public:
	cPlayer_Info() : CObject()
	{
		m_blive = true;
		m_iState = CHARECTOR_STOP;

	};
	virtual ~cPlayer_Info(){};

protected:

	char *m_PlayerID;
	int m_iState;

	float m_fSearchRange;
	float m_fAttackRange;

	cPlayer_Info* m_pTarget;

public:
	virtual void Update(float _fDTime);
	void PlayerInit(int ID,float pos_x,float pos_y,float pos_z,float hp);
	void PlayerInfo();


	VOID Move(int state);

};