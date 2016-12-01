#pragma once
#include "cGameState.h"

extern cCam gCam;

class cMainState : public cGameState
{
public:
	// 초기화
	void Init()
	{
		// 이전 모델 데이터 삭제 ( 정확히말하면 월드 매트릭스 )
		mModelManager->ClearModel();

		// 스크린 추가 (풀 스크린쿼드)
		mModelManager->AddScreen(0.0f, 0.0f, 0.0f);

		// 맵 테스트용
		mModelManager->AddMap(0, "Map1", 0.0f, 0.0f, 0.0f);


		//--------------------------------------------------------------------------------------------------------------------//
		// 본 테스트
		//--------------------------------------------------------------------------------------------------------------------//
		mModelManager->AddModel(1, "Model2", 100.0f, 0.0f, 100.0f, e_Idle);
		//mModelManager->DrawBone(   "Model2", "Run" , 100.0f, 100.0f, 600.0f, 10);  // 애니키 번호
		
		// 본 일렬로 세우기
		//for (int i = 0; i < 17; ++i)
		//	mModelManager->DrawBone("Model2", "Run", 50.0f, 100.0f, 100.0f * i, i);  // 애니키 번호

		//--------------------------------------------------------------------------------------------------------------------//
		// 해당 인스턴스 버퍼를 만들겠당..
		mModelManager->MakeInsbuf();



		// 플레이어 및 카메라 세팅
		mModelManager->IniPlayer(0, "Model1", 0.0f, 0.0f, 0.0f, e_Run);
	
		// 체력 제어
		//mModelManager->SetHP(0, "Mode1", 100.0f);
	}	

	// 제거
	void ClearClass()
	{



	}

	// 업데이트
	void Update(float& dt)
	{
		//---------------------------------------------------------------------------------------//
		// 필수 업데이트
		//---------------------------------------------------------------------------------------//
		// 렌더링 매니저 업데이트
		mDrawManager->Update(dt);
		
		// 모델 매니저 업데이트
		mModelManager->Update(dt);

		// 플레이어 액션
		PlayerAction(dt);





		//---------------------------------------------------------------------------------------//
		// 선택 업데이트
		//---------------------------------------------------------------------------------------//

		//mModelManager->MoveToPlayer(0, "Model2", 10.0f * dt);						    // 플레이어에게 이동
		//mModelManager->MovePoint   (0, "Model5", 100.0f, 100.0f, 100.0f ,10.0f * dt); // 해당 지점으로 이동









		//// 포인트 라이트
		//XMFLOAT3 _PlayerPos     = gCam.GetThirdPosition();
		//XMFLOAT3 _PlayerLookDir = mModelManager->PlayerLookDir();
		//_PlayerPos.x = _PlayerPos.x + _PlayerLookDir.x * 15.0f;
		//_PlayerPos.z = _PlayerPos.z + _PlayerLookDir.z * 15.0f;
		//_PlayerPos.y = _PlayerPos.y + 20.0f;
		//mShaderManager->GetLightManager()->mPointLight.Position = _PlayerPos;

	}

	// 플레이어 액션
	void PlayerAction(float& dt)
	{
		//-------------------------------------------------------------------//
		// 키 다운
		//-------------------------------------------------------------------//
		// 카메라 컨트롤
		if (GetAsyncKeyState('W') & 0x8000)
		{
			//----------------------------------//
			// 상 키 눌렀음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
			// 클라에서 계산한 플레이어 위치
			XMFLOAT3 mPos = mModelManager->PlayerWalk(100.0f * dt);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// 히 키 눌렀음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerWalk(-100.0f * dt);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// 좌 키 눌렀음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerStrafe(-100.0f * dt);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// 우 키 눌렀음을 서버에 보내기
			//----------------------------------//





			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerStrafe(100.0f * dt);
		}

		// 점프
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			mModelManager->Jump(0, "Model1");
		}

		//-------------------------------------------------------------------//
		// 키 업
		//-------------------------------------------------------------------//
		// 카메라 컨트롤
		if (GetAsyncKeyState('W') & 0x0001)
		{
			//----------------------------------//
			// 상 키가 때졌음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
		}

		if (GetAsyncKeyState('S') & 0x0001)
		{
			//----------------------------------//
			// 히 키가 때졌음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
		}

		if (GetAsyncKeyState('A') & 0x0001)
		{
			//----------------------------------//
			// 좌 키가 때졌음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
		}

		if (GetAsyncKeyState('D') & 0x0001)
		{
			//----------------------------------//
			// 우 키가 때졌음을 서버에 보내기
			//----------------------------------//





			//----------------------------------//
		}
	}

	// 그리기
	void Draw()
	{
		// 모델 그리기 ( 박스 )
		mDrawManager->DrawIns(DXGI_FORMAT_R32_UINT);
	}

	// 키 이벤트
	void KeyEvent(WPARAM& wParam)
	{
		// 렌더링 제어
		switch (wParam)
		{
		case 'R':
			gCam.Change3PersonCam();
			break;

		case 'T':
			gCam.Change1PersonCam();
			break;

		case 'Z':
			mDrawManager->SetRes(e_Solid);
			mDrawManager->mSolidDraw = true;
			break;

		case 'X':
			mDrawManager->mSolidDraw = false;
			break;

		case 'Q':
			{
				static int i = 0;
				if (i <= 0)
					i = 5;
				else
					--i;
				mModelManager->SetPlayerFSM(FSM_TYPE(i));			
			}
			break;

		case 'E':
			{
				static int i = 0;
				if (i >= 5)
					i = 0;
				else
					++i;
				mModelManager->SetPlayerFSM(FSM_TYPE(i));			
			}
			break;

			// 그냥 모델 추가하기
		case 'G':
			{
				static int i1 = 1;
				static int i2 = 1;
				switch (/*rand() % 2*/1)
				{
				case 0:
					mModelManager->AddUpdateModel(++i1, "Model1", float(rand() % 4000) - 2000.0f, 200.0f + float(rand() % 500), float(rand() % 4000) - 2000.0f, e_Idle);
					mModelManager->SetFSM        (  i1, "Model1", FSM_TYPE(rand() % 6));
					break;
				default:
				case 1:
					mModelManager->AddUpdateModel(++i2, "Model2", float(rand() % 4000) - 2000.0f, 200.0f + float(rand() % 500), float(rand() % 4000) - 2000.0f, e_Idle);
					mModelManager->SetFSM        (  i2, "Model2", FSM_TYPE(rand() % 6));
					break;
				}

				cout << "생성: " << i1 + i2 << endl;
			}
			break;

		// 지우기 테스트
		case 'H':
			{
				static int i1 = 1;
				static int i2 = 1;
				switch (/*rand() % 2*/1)
				{
				case 0:
					mModelManager->EraseUpdateModel(++i1, "Model1");
					break;
				default:
				case 1:
					mModelManager->EraseUpdateModel(++i2, "Model2");
					break;
				}
				cout << "삭제: " << i1 + i2 << endl;
			}
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

		case WM_MOUSEWHEEL:
		{
			  if (((short)HIWORD(wParam)) > 0)
			  {
				  gCam.Setm3PersonLength(10.0f);
			  }
			  else
			  {
				  gCam.Setm3PersonLength(-10.0f);
			  }
			  break;
		}
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

