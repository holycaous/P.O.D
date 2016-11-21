#pragma once
#include "UtilD3D.h"
#include <D3Dcompiler.h>

extern cCam gCam;

class cGameState
{
protected:
	cCoreStorage*     mCoreStorage     = cCoreStorage	 ::GetInstance(); // ���̷�Ʈ COM �����
	cModelManager*    mModelManager    = cModelManager   ::GetInstance(); // �� ������Ʈ�� ���� ��� �����ϰԲ�
	cDrawManager*     mDrawManager     = cDrawManager    ::GetInstance(); // �� ������Ʈ�� �������� �����ϰԲ�

	//cShaderManager*   mShaderManager   = cShaderManager  ::GetInstance();
	//cMaterialManager* mMaterialManager = cMaterialManager::GetInstance();

	// ���콺
	POINT mLastMousePos;

	// ���콺
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

