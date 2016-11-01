#pragma once
#include "UtilD3D.h"
#include <D3Dcompiler.h>

extern cCam gCam;

class cGameState
{
protected:
	cCoreStorage*     mCoreStorage     = cCoreStorage	 ::GetInstance(); // 다이렉트 COM 저장소
	cModelManager*    mModelManager    = cModelManager   ::GetInstance(); // 각 스테이트에 모델을 사용 가능하게끔
	cDrawManager*     mDrawManager     = cDrawManager    ::GetInstance(); // 각 스테이트에 렌더링이 가능하게끔

	//cShaderManager*   mShaderManager   = cShaderManager  ::GetInstance();
	//cMaterialManager* mMaterialManager = cMaterialManager::GetInstance();

	// 마우스
	POINT mLastMousePos;

	// 마우스
	bool MbtDown = false;
	bool LbtDown = false;
	bool RbtDown = false;
public:
	cGameState()
	{
		mLastMousePos.x = 0;
		mLastMousePos.y = 0;
	}

	virtual void Init()		      = 0;
	virtual void ClearClass()     = 0;
	virtual void Draw()		      = 0;
	virtual void Update(float& dt) = 0;
	virtual void KeyEvent(WPARAM& wParam) = 0;
	virtual void MouseEvent(UINT& msg, int& _Xpos, int& _Ypos) = 0;
	virtual void MouseMoveEvent(int& _Xpos, int& _Ypos) = 0;
};

