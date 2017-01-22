#pragma once
#include "cGameState.h"

extern cCam gCam;

class cPlayState : public cGameState
{
public:
	// 초기화
	void Init()
	{
		// 이전 모델 데이터 삭제 ( 정확히말하면 월드 매트릭스 )
		mModelManager->ClearModel();

		// 스크린 추가 (풀 스크린쿼드)
		mModelManager->AddScreen(0.0f, 0.0f, 0.0f);

#ifdef POSTEFFECT_ON
		// HDR 스크린 추가 (풀 스크린쿼드)
		mModelManager->AddHdrScreen(0.0f, 0.0f, 0.0f);
#endif
		// 쉐도우맵 경계구 반지름 추가
		mShadowMap->SetmSceneBoundsRadius(mMapManager->mData["Map1"].GetWidth() / 1.8f);

		// 큐브맵 적용
		mModelManager->AddCubeMap(0, "CubeMap1", 0.0f, 0.0f, 0.0f);

		//----------------------------------------------------------------------------------------------------//
		// 맵 추가
		//----------------------------------------------------------------------------------------------------//
		mModelManager->AddMap(0, "Map1", 0.0f, 0.0f, 0.0f);


		//----------------------------------------------------------------------------------------------------//
		// 배경 모델 추가
		//----------------------------------------------------------------------------------------------------//
		mModelManager->AddModel(1, "Model2", 100.0f, 0.0f, 100.0f, e_Idle);


		//----------------------------------------------------------------------------------------------------//
		// 해당 인스턴스 버퍼를 만들겠당..
		mModelManager->MakeInsbuf();

		//----------------------------------------------------------------------------------------------------//
		// 플레이어 및 카메라 세팅
		//----------------------------------------------------------------------------------------------------//
		mModelManager->IniPlayer(0, "Model1", 0.0f, 0.0f, 0.0f, e_Run);
		gCam.Change1PersonCam();

		//----------------------------------------------------------------------------------------------------//
		// BGM
		//----------------------------------------------------------------------------------------------------//
		mSoundManager->PlayBGM(e_PlayState);

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

		// 모델 매니저 업데이트
		mModelManager->Update(dt);

		// 플레이어 액션
		PlayerAction(dt);
	}

	// 플레이어 액션
	void PlayerAction(float& dt)
	{
		// 카메라 컨트롤
		if (GetAsyncKeyState('W') & 0x8000)
		{
			//----------------------------------//
			// 상 키 눌렀음을 서버에 보내기
			//----------------------------------//


			//----------------------------------//
			gCam.Walk(100.0f*dt);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// 히 키 눌렀음을 서버에 보내기
			//----------------------------------//


			//----------------------------------//
			gCam.Walk(-100.0f*dt);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// 좌 키 눌렀음을 서버에 보내기
			//----------------------------------//

			//----------------------------------//
			gCam.Strafe(-100.0f*dt);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// 우 키 눌렀음을 서버에 보내기
			//----------------------------------//


			// 서버에서 위치를 받아, 플레이어 위치를 갱신하는 함수.
			// 해당 부분 이곳이 아닌, 다른 곳에서 구현해야함.
			// 서버에서 데이터 받는 곳
			//mModelManager->SetPlayerPos(_x, _y, _z);
			//----------------------------------//
			gCam.Strafe(100.0f*dt);
		}
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

		case 'R':
			gCam.Change3PersonCam();
			break;

		case 'T':
			gCam.Change1PersonCam();
			break;
		}

	}

	// 마우스 이벤트
	void MouseEvent(UINT& msg, int& _Xpos, int& _Ypos, WPARAM& wParam)
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

