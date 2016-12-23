#include "stdafx.h"
#include "cGameStateManager.h"

// 기본 게임 상태
void cGameStateManager::Init()
{
	m_GameState = nullptr;
	ChangeState(new cMainState); //게임상태 변경
}
void cGameStateManager::ClearClass()
{
	//무언가 값이 들어가있다면
	if (m_GameState != nullptr)
	{
		m_GameState->ClearClass();
		delete m_GameState;

		m_GameState = nullptr;
	}
}

// 게임상태 변경
void cGameStateManager::ChangeState(cGameState* _NewState)
{
	ClearClass();

	m_GameState = _NewState;
	m_GameState->Init();
}

//-------------------------------------//
// 게임상태 변수
//-------------------------------------//
// 그리다
void cGameStateManager::Draw()
{
	//스테이지가 존재하는지
	if (m_GameState)
		m_GameState->Draw();
}

// 업데이트
void cGameStateManager::Update(float& dt)
{
	//스테이지가 변경되었는지
	StageCheck();

	//스테이지가 존재하는지
	if (m_GameState)
		m_GameState->Update(dt);
}

// 키 이벤트
void cGameStateManager::KeyEvent(WPARAM& wParam)
{
	//스테이지가 존재하는지
	if (m_GameState)
		m_GameState->KeyEvent(wParam);
}

// 마우스 이벤트
void cGameStateManager::MouseEvent(UINT& msg, int _Xpos, int _Ypos, WPARAM& wParam)
{
	m_Xpos = _Xpos;
	m_Ypos = _Ypos;

	//스테이지가 존재하는지
	if (m_GameState)
		m_GameState->MouseEvent(msg, _Xpos, _Ypos, wParam);
}

// 마우스 이동 이벤트
void cGameStateManager::MouseMoveEvent(int _Xpos, int _Ypos)
{
	//스테이지가 존재하는지
	if (m_GameState)
		m_GameState->MouseMoveEvent(_Xpos, _Ypos);
}

// 스테이지가 변경되었는지 체크 후 변경
void cGameStateManager::StageCheck()
{
	if (cGameStateSave::GetInstance()->m_StateChangeCheck)
	{
		switch (cGameStateSave::GetInstance()->m_StateChaneNumber)
		{
		case e_MapTool:
			ChangeState(new cMapTool);
			break;

		case e_MainState:
			ChangeState(new cMainState);
			break;

		case e_PlayState:
			ChangeState(new cPlayState);
			break;

		default:
			break;
		}
		cGameStateSave::GetInstance()->m_StateChangeCheck = false;
	}
}