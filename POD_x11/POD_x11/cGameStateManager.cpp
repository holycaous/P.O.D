#include "stdafx.h"
#include "cGameStateManager.h"

// �⺻ ���� ����
void cGameStateManager::Init()
{
	m_GameState = nullptr;
	ChangeState(new cMainState); //���ӻ��� ����
}
void cGameStateManager::ClearClass()
{
	//���� ���� ���ִٸ�
	if (m_GameState != nullptr)
	{
		m_GameState->ClearClass();
		delete m_GameState;

		m_GameState = nullptr;
	}
}

// ���ӻ��� ����
void cGameStateManager::ChangeState(cGameState* _NewState)
{
	ClearClass();

	m_GameState = _NewState;
	m_GameState->Init();
}

//-------------------------------------//
// ���ӻ��� ����
//-------------------------------------//
// �׸���
void cGameStateManager::Draw()
{
	//���������� �����ϴ���
	if (m_GameState)
		m_GameState->Draw();
}

// ������Ʈ
void cGameStateManager::Update(float& dt)
{
	//���������� ����Ǿ�����
	StageCheck();

	//���������� �����ϴ���
	if (m_GameState)
		m_GameState->Update(dt);
}

// Ű �̺�Ʈ
void cGameStateManager::KeyEvent(WPARAM& wParam)
{
	//���������� �����ϴ���
	if (m_GameState)
		m_GameState->KeyEvent(wParam);
}

// ���콺 �̺�Ʈ
void cGameStateManager::MouseEvent(UINT& msg, int _Xpos, int _Ypos, WPARAM& wParam)
{
	m_Xpos = _Xpos;
	m_Ypos = _Ypos;

	//���������� �����ϴ���
	if (m_GameState)
		m_GameState->MouseEvent(msg, _Xpos, _Ypos, wParam);
}

// ���콺 �̵� �̺�Ʈ
void cGameStateManager::MouseMoveEvent(int _Xpos, int _Ypos)
{
	//���������� �����ϴ���
	if (m_GameState)
		m_GameState->MouseMoveEvent(_Xpos, _Ypos);
}

// ���������� ����Ǿ����� üũ �� ����
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