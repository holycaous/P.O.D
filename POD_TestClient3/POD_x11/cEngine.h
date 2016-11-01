#pragma once
#include "cInitD3D.h"

// ���� Ŭ����
class cEngine : public cInitD3D
{
	cGameStateManager* mGameStateManager;
public:
	cEngine(HINSTANCE hInstance);
	~cEngine();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
};