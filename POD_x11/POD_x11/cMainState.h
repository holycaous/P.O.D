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
		mModelManager->AddScreen(0.0f, 0.0f, 0.0f);

		// �� �׽�Ʈ��
		mModelManager->AddMap(0, "Map1", 0.0f, 0.0f, 0.0f);


		//--------------------------------------------------------------------------------------------------------------------//
		// �� �׽�Ʈ
		//--------------------------------------------------------------------------------------------------------------------//
		mModelManager->AddModel(1, "Model2", 100.0f, 0.0f, 100.0f, e_Idle);
		//mModelManager->DrawBone(   "Model2", "Run" , 100.0f, 100.0f, 600.0f, 10);  // �ִ�Ű ��ȣ
		
		// �� �Ϸķ� �����
		//for (int i = 0; i < 17; ++i)
		//	mModelManager->DrawBone("Model2", "Run", 50.0f, 100.0f, 100.0f * i, i);  // �ִ�Ű ��ȣ

		//--------------------------------------------------------------------------------------------------------------------//
		// �ش� �ν��Ͻ� ���۸� ����ڴ�..
		mModelManager->MakeInsbuf();



		// �÷��̾� �� ī�޶� ����
		mModelManager->IniPlayer(0, "Model1", 0.0f, 0.0f, 0.0f, e_Run);
	
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









		//// ����Ʈ ����Ʈ
		//XMFLOAT3 _PlayerPos     = gCam.GetThirdPosition();
		//XMFLOAT3 _PlayerLookDir = mModelManager->PlayerLookDir();
		//_PlayerPos.x = _PlayerPos.x + _PlayerLookDir.x * 15.0f;
		//_PlayerPos.z = _PlayerPos.z + _PlayerLookDir.z * 15.0f;
		//_PlayerPos.y = _PlayerPos.y + 20.0f;
		//mShaderManager->GetLightManager()->mPointLight.Position = _PlayerPos;

	}

	// �÷��̾� �׼�
	void PlayerAction(float& dt)
	{
		//-------------------------------------------------------------------//
		// Ű �ٿ�
		//-------------------------------------------------------------------//
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

		// ����
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			mModelManager->Jump(0, "Model1");
		}

		//-------------------------------------------------------------------//
		// Ű ��
		//-------------------------------------------------------------------//
		// ī�޶� ��Ʈ��
		if (GetAsyncKeyState('W') & 0x0001)
		{
			//----------------------------------//
			// �� Ű�� �������� ������ ������
			//----------------------------------//




			//----------------------------------//
		}

		if (GetAsyncKeyState('S') & 0x0001)
		{
			//----------------------------------//
			// �� Ű�� �������� ������ ������
			//----------------------------------//




			//----------------------------------//
		}

		if (GetAsyncKeyState('A') & 0x0001)
		{
			//----------------------------------//
			// �� Ű�� �������� ������ ������
			//----------------------------------//




			//----------------------------------//
		}

		if (GetAsyncKeyState('D') & 0x0001)
		{
			//----------------------------------//
			// �� Ű�� �������� ������ ������
			//----------------------------------//





			//----------------------------------//
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

			// �׳� �� �߰��ϱ�
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

				cout << "����: " << i1 + i2 << endl;
			}
			break;

		// ����� �׽�Ʈ
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
				cout << "����: " << i1 + i2 << endl;
			}
			break;
		}
	}

	// ���콺 �̺�Ʈ
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

