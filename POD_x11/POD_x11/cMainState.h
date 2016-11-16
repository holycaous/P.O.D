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
		mModelManager->AddScreen(0.0f, 0.0f, 140.0f);

		// 맵 테스트용
		mModelManager->AddMap(0, "Map1", 200, 100, 200, 5.0f);

		//// 모델 추가
		//int size = 1250;
		//int Count = 2500;
		//
		//float Offset = 100.0f;
		//for (int x = 0; x < Count; ++x)
		//{
		//	switch (rand() % 5)
		//	{
		//	case 0:
		//		mModelManager->AddModel("BOX1", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 1:
		//		mModelManager->AddModel("BOX2", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 2:
		//		mModelManager->AddModel("BOX3", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 3:
		//		mModelManager->AddModel("Model2", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 4:
		//		mModelManager->AddModel("Model3", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 5:
		//		//mModelManager->AddModel("Model5", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	}
		//}
		

		// 단일 객체 테스트용
		//mModelManager->AddModel("Model0", 100.0f, 100.0f, 100.0f );
		//mModelManager->AddModel("Model1", 100.0f, 100.0f, 200.0f );
		//mModelManager->AddModel("Model2", 100.0f, 100.0f, 250.0f ); // <-- 모든 모델의 유니크코드는 0번부터 시작
		//mModelManager->AddModel("Model3", 100.0f, 100.0f, 470.0f ); //     동일한 모델을 또 만들면 유니크 코드가 1씩 증가함 
		//mModelManager->AddModel("Model4", 100.0f, 100.0f, 470.0f );

		// 테스트용
		//mModelManager->AddModel("Model4",  100.0f, 0.0f,  300.0f);
		mModelManager->AddModel("Model2",  100.0f, 100.0f,  200.0f);
		mModelManager->AddModel("Model1",  100.0f, 100.0f,  500.0f);
		mModelManager->AddModel("Model5",  100.0f, 100.0f,  700.0f);
		
		// 본 그려보기 추가
		mModelManager->DrawBone("Model1", "Idle", 100.0f, 100.0f, 500.0f);

		// 해당 인스턴스 버퍼를 만들겠당..
		mModelManager->MakeInsbuf();

		// 플레이어 및 카메라 세팅
		mModelManager->IniPlayer(0, "Model3", 200.0f, 100.0f, 600.0f);
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

		mModelManager->MoveToPlayer(0, "Model2", 10.0f * dt);						  // 플레이어에게 이동
		mModelManager->MovePoint   (0, "Model5", 100.0f, 100.0f, 100.0f ,10.0f * dt); // 해당 지점으로 이동

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
			// 클라에서 계산한 플레이어 위치
			XMFLOAT3 mPos = mModelManager->PlayerWalk(100.0f * dt);
			cout << "mPos: " << mPos.x << ", " << mPos.y << ", " << mPos.z << endl;
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// 히 키 눌렀음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerWalk(-100.0f * dt);
			cout << "mPos: " << mPos.x << ", " << mPos.y << ", " << mPos.z << endl;
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// 좌 키 눌렀음을 서버에 보내기
			//----------------------------------//




			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerStrafe(-100.0f * dt);
			cout << "mPos: " << mPos.x << ", " << mPos.y << ", " << mPos.z << endl;
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// 우 키 눌렀음을 서버에 보내기
			//----------------------------------//





			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerStrafe(100.0f * dt);
			cout << "mPos: " << mPos.x << ", " << mPos.y << ", " << mPos.z << endl;
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
			
			break;

		case 'E':

			break;

			// 그냥 모델 추가하기
		case 'G':
			mModelManager->AddUpdateModel("Model5", float(rand() % 500) + 100.0f, 100.0f, float(rand() % 500) + 100.0f);
			break;

		// 지우기 테스트
		case 'H':
			{
				static int i = 0;
				mModelManager->EraseUpdateModel(++i, "Model5");
			}
			break;

			// 번호 선택, 추가하기
		case 'O':
			mModelManager->AddUpdateModel(1, "Model5", 100, 100, 500);
			break;
		
		// 기타 테스트
		case 'I':
			{
				//--------------------------------------------------------//
				// 테스트 1
				//--------------------------------------------------------//
			    // 해당 위치로 가는 방향벡터 얻기 (0번 모델3이 해당 위치로 가는 방향벡터 얻기)
				XMFLOAT3 dir = mModelManager->GetPointDir(0, "Model3", 100.0f, 100.0f, 300.0f);


				//--------------------------------------------------------//
				// 테스트 2
				//--------------------------------------------------------//
				// 데이터를 임시로 저장할 공간
				vector<ObjData> _testOut;

				//--------------------------------------------------------//
				// <1> 0번 모델3의 정보를 빼낸다.
				//--------------------------------------------------------//
				mModelManager->getModelData(0, "Model3", _testOut);

				for (unsigned int i = 0; i < _testOut.size(); ++i)
				{
					// 원하는 정보를 뺴낸다.
					_testOut[i].getLookDir(); // 룩 벡터
					_testOut[i].getWdMtx();   // 요건 월드 매트릭스
				}
				_testOut.clear(); // 사용후에는 반드시 클리어

				//--------------------------------------------------------//
				// <2> Model3에 속한, 모든 정보를 빼낸다.
				//--------------------------------------------------------//
				mModelManager->getAllModelData("Model3", _testOut);

				for (unsigned int i = 0; i < _testOut.size(); ++i)
				{
					// 원하는 정보를 뺴낸다.
					_testOut[i].getLookDir(); // 룩 벡터
					_testOut[i].getWdMtx();   // 요건 월드 매트릭스
				}
				_testOut.clear(); // 사용후에는 반드시 클리어
				//--------------------------------------------------------//
			}
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

