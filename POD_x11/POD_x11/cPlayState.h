#pragma once
#include "cGameState.h"

class cPlayState : public cGameState
{
public:
	// 초기화
	void Init()
	{
		// 이전 모델 데이터 삭제 ( 정확히말하면 월드 매트릭스 )
		mModelManager->ClearModel();

		// 모델 추가
		int size  = 550;
		int Count = 5500;
		for (int x = 0; x < Count; ++x)
		{
			switch (rand() % 3)
			{
			case 0:
				mModelManager->AddModel("BOX1", float(rand() % size), float(rand() % size), float(rand() % size));
				break;
			case 1:
				mModelManager->AddModel("BOX2", float(rand() % size), float(rand() % size), float(rand() % size));
				break;
			case 2:
				mModelManager->AddModel("BOX3", float(rand() % size), float(rand() % size), float(rand() % size));
				break;
			}
		}

		// 스크린 추가 (풀 스크린쿼드)
		mModelManager->AddScreen(0.0f, 0.0f, 140.0f);

		// 해당 인스턴스 버퍼를 만들겠당..
		mModelManager->MakeInsbuf();
	}

	// 제거
	void ClearClass()
	{
		
	}

	// 업데이트
	void Update(float& dt)
	{
		// 렌더링 매니저 업데이트
		mDrawManager->Update(dt);
	}

	// 그리기
	void Draw()
	{
		// 모델 그리기
		mDrawManager->DrawIns(DXGI_FORMAT_R32_UINT);
	}

	// 키 이벤트
	void KeyEvent(WPARAM& wParam)
	{
		// 렌더링 제어
		switch (wParam)
		{
		case 'Z':
			mDrawManager->SetRes(e_Solid);
			break;

		case 'X':
			mDrawManager->SetRes(e_Wire);
			break;
		}

	}

	// 마우스 이벤트
	void MouseEvent(UINT& msg, int& _Xpos, int& _Ypos)
	{
		switch (msg)
		{
		case WM_RBUTTONDOWN:
			RbtDown = true;

			mLastMousePos.x = _Xpos;
			mLastMousePos.y = _Ypos;

			SetCapture(*cCoreStorage::GetInstance()->mhWnd);
			break;

		case WM_RBUTTONUP:
			RbtDown = false;

			ReleaseCapture();
			break;

		}
	}

	// 마우스 이동 이벤트
	void MouseMoveEvent(int& _Xpos, int& _Ypos)
	{
		if (RbtDown)
		{
			// 새로운 마우스 위치 생성 ( 회전 후 )
			float dx = XMConvertToRadians(0.25f * static_cast<float>(_Xpos - mLastMousePos.x));
			float dy = XMConvertToRadians(0.25f * static_cast<float>(_Ypos - mLastMousePos.y));

			// 새로운 마우스 위치 갱신
			gCam.Pitch(dy);
			gCam.RotateY(dx);
		}

		// 이전 마우스 위치
		mLastMousePos.x = _Xpos;
		mLastMousePos.y = _Ypos;
	}
};

