#pragma once
#include "cGameStateSave.h"
#include "cMapTool.h"
#include "cMainState.h"
#include "cPlayState.h"

class cGameStateManager : public cSingleton<cGameStateManager>
{
	//���� ���ӻ���
	cGameState *m_GameState;

	// ���콺 ���� ����
	int m_Xpos;
	int m_Ypos;

public:
	// �⺻ ���� ����
	void Init();
	void ClearClass();

	// ���ӻ��� ����
	void ChangeState(cGameState* _NewState);

	//-------------------------------------//
	// ���ӻ��� ����
	//-------------------------------------//
	// �׸���
	void Draw();

	// ������Ʈ
	void Update(float& dt);

	// Ű �̺�Ʈ
	void KeyEvent(WPARAM& wParam);

	// ���콺 �̺�Ʈ
	void MouseEvent(UINT& msg, int _Xpos, int _Ypos);

	// ���콺 �̵� �̺�Ʈ
	void MouseMoveEvent(int _Xpos, int _Ypos);

	// ���������� ����Ǿ����� üũ �� ����
	void StageCheck();
};
