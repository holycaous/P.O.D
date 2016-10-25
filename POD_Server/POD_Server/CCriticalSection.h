#pragma once
#include "stdafx.h"

class CCriticalSection
{
private:
	CRITICAL_SECTION m_CS; // CRITICAL_SECTION °´Ã¼
public:
	CCriticalSection()
	{
		InitializeCriticalSection(&m_CS);
	}

	~CCriticalSection()
	{
		DeleteCriticalSection(&m_CS);
	}

	void Enter()
	{
		EnterCriticalSection(&m_CS);
	}

	void Leave()
	{
		LeaveCriticalSection(&m_CS);
	}
};