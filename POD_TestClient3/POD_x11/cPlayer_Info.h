#pragma once
#include "stdafx.h"

class cPlayer_Info : public CObject
{
public:
	cPlayer_Info() : CObject()
	{
		m_blive = true;
	};
	virtual ~cPlayer_Info(){};

protected:
	cPlayer_Info* m_pTarget;


public:
	//virtual void Update(float _fDTime);

};