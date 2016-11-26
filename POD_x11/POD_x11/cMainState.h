#pragma once
#include "cGameState.h"

extern cCam gCam;

class cMainState : public cGameState
{
public:
	// �ʱ�ȭ
	void Init()
	{
		// ���� �� ������ ���� ( ��Ȯ�����ϸ� ���� ��Ʈ���� )
		mModelManager->ClearModel();

		// ��ũ�� �߰� (Ǯ ��ũ������)
		mModelManager->AddScreen(0.0f, 0.0f, 140.0f);

		// �� �׽�Ʈ��
		mModelManager->AddMap(0, "Map1", 200, 100, 200, 5.0f);

		//// �� �߰�
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
		

		// ���� ��ü �׽�Ʈ��
		//mModelManager->AddModel("Model1", 100.0f, 100.0f, 200.0f );
		//mModelManager->AddModel("Model2", 100.0f, 100.0f, 250.0f ); // <-- ��� ���� ����ũ�ڵ�� 0������ ����
		//mModelManager->AddModel("Model3", 100.0f, 100.0f, 470.0f ); //     ������ ���� �� ����� ����ũ �ڵ尡 1�� ������ 
		//mModelManager->AddModel(0, "Model4", 100.0f, 100.0f, 470.0f, e_Idle);

		// �׽�Ʈ��
		//mModelManager->AddModel(0, "Model4", 100.0f, 100.0f, 300.0f, e_Idle);
		mModelManager->AddModel(0, "Model2", 100.0f, 100.0f, 200.0f, e_Idle);
		mModelManager->AddModel(1, "Model3", 100.0f, 100.0f, 400.0f, e_Idle);
		//mModelManager->AddModel(0, "Model5", 100.0f, 100.0f, 700.0f, e_Idle);
		
		//// �� �׷����� �߰�
		//for (int i = 0; i < 101; ++i)
		//	mModelManager->DrawBone("Model1", "Idle", 200.0f, 100.0f, 50 * i, i); // �������� Ű ��ȣ (����� ����Ǳ� ���ؼ�, ��Ų �ؽ�ó�� ����� ��)

		//for (int i = 0; i < 21; ++i)
		//	mModelManager->DrawBone("Model5", "Idle", 300.0f, 100.0f, 50 * i, i); // �������� Ű ��ȣ (����� ����Ǳ� ���ؼ�, ��Ų �ؽ�ó�� ����� ��)


		//--------------------------------------------------------------------------------------------------------------------//
		// �� �׽�Ʈ
		//--------------------------------------------------------------------------------------------------------------------//
		mModelManager->AddModel(0, "Model1", 100.0f, 100.0f, 500.0f, e_Idle);
		//mModelManager->DrawBone("Model1", "Idle", 100.0f, 100.0f, 500.0f, 0);  // �ִ�Ű ��ȣ

		//mModelManager->AddModel(0, "Model4", 100.0f, 100.0f, 500.0f, e_Idle);
		//mModelManager->DrawBone("Model4", "Idle", 100.0f, 100.0f, 500.0f, 0);  // �ִ�Ű ��ȣ

		mModelManager->AddModel(0, "Model5", 100.0f, 100.0f, 700.0f, e_Idle);
		//mModelManager->DrawBone("Model5", "Idle", 100.0f, 100.0f, 700.0f, 0);  // �ִ�Ű ��ȣ
		
		//--------------------------------------------------------------------------------------------------------------------//


		// �ش� �ν��Ͻ� ���۸� ����ڴ�..
		mModelManager->MakeInsbuf();

		// �÷��̾� �� ī�޶� ����
		mModelManager->IniPlayer(0, "Model3", 200.0f, 100.0f, 600.0f, e_Idle);
		mModelManager->SetScale (0, "Model3", 0.001f, 0.001f, 0.001f);

		// FSM ���¹ٲٱ�
		//mModelManager->SetFSM(0, "Model3", e_Idle);

		// ü�� ����
		//mModelManager->SetHP(0, "Mode1", 100.0f);
	}	

	// ����
	void ClearClass()
	{



	}

	// ������Ʈ
	void Update(float& dt)
	{
		//---------------------------------------------------------------------------------------//
		// �ʼ� ������Ʈ
		//---------------------------------------------------------------------------------------//
		// ������ �Ŵ��� ������Ʈ
		mDrawManager->Update(dt);
		
		// �� �Ŵ��� ������Ʈ
		mModelManager->Update(dt);

		// �÷��̾� �׼�
		PlayerAction(dt);





		//---------------------------------------------------------------------------------------//
		// ���� ������Ʈ
		//---------------------------------------------------------------------------------------//

		//mModelManager->MoveToPlayer(0, "Model2", 10.0f * dt);						    // �÷��̾�� �̵�
		//mModelManager->MovePoint   (0, "Model5", 100.0f, 100.0f, 100.0f ,10.0f * dt); // �ش� �������� �̵�










	}

	// �÷��̾� �׼�
	void PlayerAction(float& dt)
	{
		// ī�޶� ��Ʈ��
		if (GetAsyncKeyState('W') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//




			//----------------------------------//
			// Ŭ�󿡼� ����� �÷��̾� ��ġ
			XMFLOAT3 mPos = mModelManager->PlayerWalk(100.0f * dt);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//




			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerWalk(-100.0f * dt);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//




			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerStrafe(-100.0f * dt);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//





			//----------------------------------//
			XMFLOAT3 mPos = mModelManager->PlayerStrafe(100.0f * dt);
		}
	}

	// �׸���
	void Draw()
	{
		// �� �׸��� ( �ڽ� )
		mDrawManager->DrawIns(DXGI_FORMAT_R32_UINT);
	}

	// Ű �̺�Ʈ
	void KeyEvent(WPARAM& wParam)
	{
		// ������ ����
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

			// �׳� �� �߰��ϱ�
		case 'G':
			mModelManager->AddUpdateModel("Model5", float(rand() % 500) + 100.0f, 100.0f, float(rand() % 500) + 100.0f);
			break;

		// ����� �׽�Ʈ
		case 'H':
			{
				static int i = 0;
				mModelManager->EraseUpdateModel(++i, "Model5");
			}
			break;

			// ��ȣ ����, �߰��ϱ�
		case 'O':
			mModelManager->AddUpdateModel(1, "Model5", 100, 100, 500);
			break;
		
		// ��Ÿ �׽�Ʈ
		case 'I':
			{
				//--------------------------------------------------------//
				// �׽�Ʈ 1
				//--------------------------------------------------------//
			    // �ش� ��ġ�� ���� ���⺤�� ��� (0�� ��3�� �ش� ��ġ�� ���� ���⺤�� ���)
				XMFLOAT3 dir = mModelManager->GetPointDir(0, "Model3", 100.0f, 100.0f, 300.0f);


				//--------------------------------------------------------//
				// �׽�Ʈ 2
				//--------------------------------------------------------//
				// �����͸� �ӽ÷� ������ ����
				vector<ObjData> _testOut;

				//--------------------------------------------------------//
				// <1> 0�� ��3�� ������ ������.
				//--------------------------------------------------------//
				mModelManager->getModelData(0, "Model3", _testOut);

				for (unsigned int i = 0; i < _testOut.size(); ++i)
				{
					// ���ϴ� ������ ������.
					_testOut[i].getLookDir(); // �� ����
					_testOut[i].getWdMtx();   // ��� ���� ��Ʈ����
				}
				_testOut.clear(); // ����Ŀ��� �ݵ�� Ŭ����

				//--------------------------------------------------------//
				// <2> Model3�� ����, ��� ������ ������.
				//--------------------------------------------------------//
				mModelManager->getAllModelData("Model3", _testOut);

				for (unsigned int i = 0; i < _testOut.size(); ++i)
				{
					// ���ϴ� ������ ������.
					_testOut[i].getLookDir(); // �� ����
					_testOut[i].getWdMtx();   // ��� ���� ��Ʈ����
				}
				_testOut.clear(); // ����Ŀ��� �ݵ�� Ŭ����
				//--------------------------------------------------------//
			}
			break;
		}
	}

	// ���콺 �̺�Ʈ
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

	// ���콺 �̵� �̺�Ʈ
	void MouseMoveEvent(int& _Xpos, int& _Ypos)
	{
		if (RbtDown)
		{
			// ���ο� ���콺 ��ġ ���� ( ȸ�� �� )
			float dx = XMConvertToRadians(0.25f * static_cast<float>(_Xpos - mLastMousePos.x));
			float dy = XMConvertToRadians(0.25f * static_cast<float>(_Ypos - mLastMousePos.y));

			// ���ο� ���콺 ��ġ ����
			gCam.Pitch(dy);
			gCam.RotateY(dx);
		}

		// ���� ���콺 ��ġ
		mLastMousePos.x = _Xpos;
		mLastMousePos.y = _Ypos;
	}
};

