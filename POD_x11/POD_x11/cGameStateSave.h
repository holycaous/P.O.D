#pragma once
class cGameStateSave : public cSingleton<cGameStateSave>
{
public:
	bool        m_StateChangeCheck = false;
	GAME_STATE_TYPE  m_StateChaneNumber = e_MainState;
};
