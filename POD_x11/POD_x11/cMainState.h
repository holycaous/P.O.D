#pragma once
#include "cGameState.h"

class cMainState : public cGameState
{
public:
	// 초기화
	void Init()
	{
		// 이전 모델 데이터 삭제 ( 정확히말하면 월드 매트릭스 )
		mModelManager->ClearModel();

		// 맵 테스트용
		mModelManager->AddModel("Map1", 0.0f, 100.0f, 0.0f);
		mModelManager->SetScale(0, "Map1", 5.0f, 1.0f, 5.0f);


		//// 모델 추가
		//int size = 1250;
		//int Count = 2500;
		//
		//float Offset = 100.0f;
		//for (int x = 0; x < Count; ++x)
		//{
		//	switch (rand() % 7)
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
		//		mModelManager->AddModel("BOX4", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 4:
		//		mModelManager->AddModel("Model2", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 5:
		//		mModelManager->AddModel("Model3", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	case 6:
		//		mModelManager->AddModel("Model4", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//		break;
		//	}
		//}
		

		// 단일 객체 테스트용
		//mModelManager->AddModel("Model0", 100.0f, 100.0f, 100.0f );
		//mModelManager->AddModel("Model1", 100.0f, 100.0f, 200.0f );
		//mModelManager->AddModel("Model2", 100.0f, 100.0f, 250.0f );
		mModelManager  ->AddModel("Model3", 100.0f, 100.0f, 350.0f ); // <-- 모든 모델의 유니크코드는 0번부터 시작
		//mModelManager->AddModel("Model3", 100.0f, 100.0f, 470.0f ); //     동일한 모델을 또 만들면 유니크 코드가 1씩 증가함 
		//mModelManager->AddModel("Model4", 100.0f, 100.0f, 470.0f );


		// 테스트용
		mModelManager->AddModel("Model4", -100.0f, 100.0f, -100.0f);
		mModelManager->AddModel("Model2",    0.0f, 100.0f,    0.0f);
		mModelManager->AddModel("Model1",  100.0f, 100.0f,  100.0f);
		
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
		// 모델 그리기 ( 박스 )
		mDrawManager->DrawIns(DXGI_FORMAT_R32_UINT);
	}

	// 키 이벤트
	void KeyEvent(WPARAM& wParam)
	{
		// 렌더링 제어
		switch (wParam)
		{
		// 이동 테스트
		case 'W':
			mModelManager->SetPos(0, "Model3", 100.0f, 100.0f, 100.0f + 100.0f);
			break;

		case 'A':
			mModelManager->SetPos(0, "Model3", 100.0f + 100.0f, 100.0f, 100.0f);
			break;

		case 'S':
			mModelManager->SetPos(0, "Model3", 100.0f, 100.0f, 100.0f - 100.0f);
			break;

		case 'D':
			mModelManager->SetPos(0, "Model3", 100.0f - 100.0f, 100.0f, 100.0f);
			break;

		case 'Z':
			mDrawManager->SetRes(e_Solid);
			mDrawManager->mSolidDraw = true;
			break;

		case 'X':
			mDrawManager->mSolidDraw = false;
			break;

		// 회전 테스트
		case 'Q':
			mModelManager->SetRotate(0, "Model3", -100.0f, 100.0f, -100.0f);
			break;

		case 'E':
			mModelManager->SetRotate(0, "Model3", 100.0f, 100.0f, 100.0f);
			break;

		// 스케일 테스트
		case 'T':
			break;

		case 'G':
			break;
		
		// 기타 테스트
		case 'H':
			{
				// 데이터를 임시로 저장할 공간
				vector<ObjData> _testOut;

				//--------------------------------------------------------//
				// <1> 1개의 오브젝트 정보만 빼낸다.
				mModelManager->getModelData(1, "Model3", _testOut);

				for (unsigned int i = 0; i < _testOut.size(); ++i)
				{
					// 원하는 정보를 뺴낸다.
					_testOut[i].getLookDir(); // 룩 벡터
					_testOut[i].getWdMtx();   // 요건 월드 매트릭스
				}
				_testOut.clear(); // 사용후에는 반드시 클리어

				//--------------------------------------------------------//
				// <2> 모든 오브젝트의 정보를 빼낸다.
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

