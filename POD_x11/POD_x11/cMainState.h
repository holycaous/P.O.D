#pragma once
#include "cGameState.h"

extern cCam gCam;

class cMainState : public cGameState
{
	// �÷��̾� ��ġ
	XMFLOAT3 mPlayerPos;

public:
	// �ʱ�ȭ
	void Init()
	{
		// ���� �� ������ ���� ( ��Ȯ�����ϸ� ���� ��Ʈ���� )
		mModelManager->ClearModel();

		// �� �׽�Ʈ��
		float tSzie = 10.0f;
		float tItp  = (100.0f * tSzie / 2.0f) + 100.0f;  
		mModelManager->AddModel("Map1", tItp, 100.0f, tItp);
		mModelManager->SetScale(0, "Map1", tSzie, 1.0f, tSzie);


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
		//mModelManager->AddModel("Model0", 100.0f, 100.0f, 100.0f );
		//mModelManager->AddModel("Model1", 100.0f, 100.0f, 200.0f );
		//mModelManager->AddModel("Model2", 100.0f, 100.0f, 250.0f );
		mModelManager  ->AddModel("Model3", 100.0f, 100.0f, 350.0f ); // <-- ��� ���� ����ũ�ڵ�� 0������ ����
		//mModelManager->AddModel("Model3", 100.0f, 100.0f, 470.0f ); //     ������ ���� �� ����� ����ũ �ڵ尡 1�� ������ 
		//mModelManager->AddModel("Model4", 100.0f, 100.0f, 470.0f );

		// �׽�Ʈ��
		//mModelManager->AddModel("Model4",  100.0f, 0.0f,  300.0f);
		mModelManager->AddModel("Model2",  100.0f, 100.0f,  200.0f);
		mModelManager->AddModel("Model1",  100.0f, 100.0f,  500.0f);
		mModelManager->AddModel("Model5",  100.0f, 100.0f,  700.0f);
		
		// �� �׷����� �߰�
		mModelManager->DrawBone("Model1", "Idle", 100.0f, 100.0f, 500.0f);

		// ��ũ�� �߰� (Ǯ ��ũ������)
		mModelManager->AddScreen(0.0f, 0.0f, 140.0f);

		// �ش� �ν��Ͻ� ���۸� ����ڴ�..
		mModelManager->MakeInsbuf();

		// ī�޶� ����
		initCam(0, "Model3", 275.0f, 125.0f, 200.0f);
	}

	// ����
	void ClearClass()
	{



	}

	// ������Ʈ
	void Update(float& dt)
	{
		// ������ �Ŵ��� ������Ʈ
		mDrawManager->Update(dt);

		mModelManager->MovePoint(0, "Model2", mPlayerPos, 10.0f * dt);
		mModelManager->MovePoint(0, "Model5", mPlayerPos, 10.0f * dt);

		// �÷��̾� �׼�
		PlayerAction(dt);
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
			//mPlayerPos <-- Ŭ�󿡼� ����� �÷��̾� ��ġ



			//----------------------------------//
			PlayerWalk(0, "Model3", 100.0f * dt);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//




			//----------------------------------//
			PlayerWalk(0, "Model3", -100.0f * dt);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//




			//----------------------------------//
			PlayerStrafe(0, "Model3", -100.0f * dt);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//





			//----------------------------------//
			PlayerStrafe(0, "Model3", 100.0f * dt);
		}
	}

	// �÷��̾� ����, ����
	void PlayerWalk(int _key, string _model, float _speed)
	{
		gCam.Walk(_speed);

		mPlayerPos = gCam.GetThirdPosition();
		mModelManager->SetRotate(_key, _model, mPlayerPos);
		mModelManager->SetPos   (_key, _model, mPlayerPos);
	}

	// �÷��̾� ����, ����
	void PlayerStrafe(int _key, string _model, float _speed)
	{
		gCam.Strafe(_speed);

		mPlayerPos = gCam.GetThirdPosition();
		mModelManager->SetRotate(_key, _model, mPlayerPos);
		mModelManager->SetPos   (_key, _model, mPlayerPos);
	}

	// ī�޶� �ʱ�ȭ
	void initCam(int _key, string _model, float _x, float _y, float _z)
	{
		gCam.SetPosition(_x, _y, _z);
		gCam.Change3PersonCam();
		mPlayerPos = gCam.GetThirdPosition();
		mModelManager->SetRotate(_key, _model, mPlayerPos);
		mModelManager->SetPos   (_key, _model, mPlayerPos);
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

