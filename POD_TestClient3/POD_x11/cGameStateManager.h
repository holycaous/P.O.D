#pragma once
#include "cGameStateSave.h"
#include "cMapTool.h"
#include "cMainState.h"
#include "cPlayState.h"

class cGameStateManager : public cSingleton<cGameStateManager>
{
	//현재 게임상태
	cGameState *m_GameState;

	// 마우스 상태 저장
	int m_Xpos;
	int m_Ypos;

public:
	// 기본 게임 상태
	void Init();
	void ClearClass();

	// 게임상태 변경
	void ChangeState(cGameState* _NewState);

	//-------------------------------------//
	// 게임상태 변수
	//-------------------------------------//
	// 그리다
	void Draw();

	// 업데이트
	void Update(float& dt);

	// 키 이벤트
	void KeyEvent(WPARAM& wParam);

	// 마우스 이벤트
	void MouseEvent(UINT& msg, int _Xpos, int _Ypos);

	// 마우스 이동 이벤트
	void MouseMoveEvent(int _Xpos, int _Ypos);

	// 스테이지가 변경되었는지 체크 후 변경
	void StageCheck();
};
