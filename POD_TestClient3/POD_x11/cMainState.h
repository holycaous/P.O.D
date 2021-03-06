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

		//// 모델 추가
		//int size = 1250;
		//int Count = 2500;
		//
		//float Offset = 100.0f;
		//for (int x = 0; x < Count; ++x)
		//{
		//	switch (1)
		//	{
		//	case 0:
		//		switch (rand() % 7)
		//		{
		//		case 0:
		//			mModelManager->AddModel("BOX1", float(rand() % size) + Offset, 100.0f, float(rand() % size) + Offset);
		//			break;
		//		case 1:
		//			mModelManager->AddModel("BOX2", float(rand() % size) + Offset, 100.0f, float(rand() % size) + Offset);
		//			break;
		//		case 2:
		//			mModelManager->AddModel("BOX3", float(rand() % size) + Offset, 100.0f, float(rand() % size) + Offset);
		//			break;
		//		case 3:
		//			mModelManager->AddModel("BOX4", float(rand() % size) + Offset, 100.0f, float(rand() % size) + Offset);
		//			break;
		//		case 4:
		//			mModelManager->AddModel("Model2", float(rand() % size) + Offset, 100.0f, float(rand() % size) + Offset);
		//			break;
		//		case 5:
		//			mModelManager->AddModel("Model3", float(rand() % size) + Offset, 100.0f, float(rand() % size) + Offset);
		//			break;
		//		case 6:
		//			mModelManager->AddModel("Model4", float(rand() % size) + Offset, 100.0f, float(rand() % size) + Offset);
		//			break;
		//		}
		//		break;
		//	case 1:
		//		switch (rand() % 7)
		//		{
		//		case 0:
		//			mModelManager->AddModel("BOX1", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//			break;
		//		case 1:
		//			mModelManager->AddModel("BOX2", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//			break;
		//		case 2:
		//			mModelManager->AddModel("BOX3", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//			break;
		//		case 3:
		//			mModelManager->AddModel("BOX4", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//			break;
		//		case 4:
		//			mModelManager->AddModel("Model2", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//			break;
		//		case 5:
		//			mModelManager->AddModel("Model3", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//			break;
		//		case 6:
		//			mModelManager->AddModel("Model4", float(rand() % size) + Offset, float(rand() % size) + Offset, float(rand() % size) + Offset);
		//			break;
		//		}
		//		break;
		//	default:
		//		break;
		//	}
		//}

		
		// 단일 객체 테스트용
		mModelManager->AddModel("Model0", 100.0f, 100.0f, 100.0f );
		mModelManager->AddModel("Model1", 100.0f, 100.0f, 200.0f );
		mModelManager->AddModel("Model2", 100.0f, 100.0f, 250.0f );

		mModelManager->AddModel("Model3", 100.0f, 100.0f, 350.0f ); // <-- 모든 모델의 유니크코드는 0번부터 시작
		mModelManager->AddModel("Model3", 100.0f, 100.0f, 470.0f ); //     동일한 모델을 또 만들면 유니크 코드가 1씩 증가함 
		mModelManager->AddModel("Model4", 100.0f, 100.0f, 470.0f );
		

		
		// 스크린 추가
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
		for (int i = 0; i < MAX_SESSION; i++)
		{
			if (Player_MGR->GetConnectUser(i)->GetDrawFlag()==true) //플레이어가 접속했을 경우 
			{
				//접속한 플레이어 캐릭터를 생성 
				mModelManager->AddModel("Model1", Player_MGR->GetConnectUser(i)->Get_Posx(),
					Player_MGR->GetConnectUser(i)->Get_Posy(),
					Player_MGR->GetConnectUser(i)->Get_Posz());
				cout << "플레이어 생성--" << "x좌표" << Player_MGR->GetConnectUser(i)->Get_Posx() <<
					"  Y좌표 " << Player_MGR->GetConnectUser(i)->Get_Posy() <<
					"  Z좌표 " << Player_MGR->GetConnectUser(i)->Get_Posz() << endl;
				Player_MGR->GetConnectUser(i)->SetDrawFlag(false);
			}
			if (Player_MGR->GetConnectUser(i)->GetObjectInitFlag() == true)
			{
				mModelManager->SetPos(0, "Model1", Player_MGR->GetConnectUser(i)->Get_Posx(),
					Player_MGR->GetConnectUser(i)->Get_Posy(),
					Player_MGR->GetConnectUser(i)->Get_Posz());

			}
		}

		for (int i = 0; i<MAX_MONSTER; i++)
		{
			if (Monster_MGR->GetConnectMonster(i)->GetDrawFlag() == true)
			{
				cout << "몬스터 생성--" << i << endl;
				mModelManager->AddModel("Model2", Monster_MGR->GetConnectMonster(i)->Get_Posx(), Monster_MGR->GetConnectMonster(i)->Get_Posy(),
								Monster_MGR->GetConnectMonster(i)->Get_Posz());
				cout << "몬스터 생성--" << "x좌표" << Monster_MGR->GetConnectMonster(i)->Get_Posx() <<
					"  Y좌표 " << Monster_MGR->GetConnectMonster(i)->Get_Posy() <<
					"  Z좌표 " << Monster_MGR->GetConnectMonster(i)->Get_Posz() << endl;

				Monster_MGR->GetConnectMonster(i)->SetDrawFlag(false); //그렸으니 false로 바꿔 다시는 그리지 않게 만든다.
			}
		}

		
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
		UINT ID = Player_MGR->MyID; //나의 ID
		// 데이터 조립.
		BYTE pData[4] = { 0, };
		memcpy(pData, &ID, sizeof(UINT)); //ID를  패킷버퍼에 집어넣는다 

		// 렌더링 제어
		switch (wParam)
		{
		case 'W':
			Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_W, pData, sizeof(UINT)); //서버에게 패킷전송
			//mModelManager->SetPos(1, "Model3", 1500.0f, 100.0f, 100.0f + 100.0f);
			break;

		case 'A':
			//Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_A, pData, sizeof(UINT)); //서버에게 패킷전송
			//mModelManager->SetPos(1, "Model3", 100.0f + 100.0f, 100.0f, 100.0f);
			break;

		case 'S':
			//Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_S, pData, sizeof(UINT)); //서버에게 패킷전송
			//mModelManager->SetPos(1, "Model3", 100.0f, 100.0f, 100.0f - 100.0f);
			break;

		case 'D':
			//Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_D, pData, sizeof(UINT)); //서버에게 패킷전송
			//mModelManager->SetPos(1, "Model3", 100.0f - 100.0f, 100.0f, 100.0f);
			break;

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

