#pragma once
#include "cGameState.h"

extern cCam gCam;

class cPlayState : public cGameState
{
public:
	// �ʱ�ȭ
	void Init()
	{
		// ���� �� ������ ���� ( ��Ȯ�����ϸ� ���� ��Ʈ���� )
		mModelManager->ClearModel();

		// ��ũ�� �߰� (Ǯ ��ũ������)
		mModelManager->AddScreen(0.0f, 0.0f, 0.0f);

#ifdef POSTEFFECT_ON
		// HDR ��ũ�� �߰� (Ǯ ��ũ������)
		mModelManager->AddHdrScreen(0.0f, 0.0f, 0.0f);
#endif
		// ������� ��豸 ������ �߰�
		mShadowMap->SetmSceneBoundsRadius(mMapManager->mData["Map1"].GetWidth() / 1.8f);

		// ť��� ����
		mModelManager->AddCubeMap(0, "CubeMap1", 0.0f, 0.0f, 0.0f);

		//----------------------------------------------------------------------------------------------------//
		// �� �߰�
		//----------------------------------------------------------------------------------------------------//
		mModelManager->AddMap(0, "Map1", 0.0f, 0.0f, 0.0f);


		//----------------------------------------------------------------------------------------------------//
		// ��� �� �߰�
		//----------------------------------------------------------------------------------------------------//
		mModelManager->AddModel(1, "Model2", 100.0f, 0.0f, 100.0f, e_Idle);


		//----------------------------------------------------------------------------------------------------//
		// �ش� �ν��Ͻ� ���۸� ����ڴ�..
		mModelManager->MakeInsbuf();

		//----------------------------------------------------------------------------------------------------//
		// �÷��̾� �� ī�޶� ����
		//----------------------------------------------------------------------------------------------------//
		mModelManager->IniPlayer(0, "Model1", 0.0f, 0.0f, 0.0f, e_Run);
		gCam.Change1PersonCam();

		//----------------------------------------------------------------------------------------------------//
		// BGM
		//----------------------------------------------------------------------------------------------------//
		mSoundManager->PlayBGM(e_PlayState);

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

		// �� �Ŵ��� ������Ʈ
		mModelManager->Update(dt);

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


			//----------------------------------//
			gCam.Walk(100.0f*dt);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//


			//----------------------------------//
			gCam.Walk(-100.0f*dt);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//

			//----------------------------------//
			gCam.Strafe(-100.0f*dt);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			//----------------------------------//
			// �� Ű �������� ������ ������
			//----------------------------------//


			// �������� ��ġ�� �޾�, �÷��̾� ��ġ�� �����ϴ� �Լ�.
			// �ش� �κ� �̰��� �ƴ�, �ٸ� ������ �����ؾ���.
			// �������� ������ �޴� ��
			//mModelManager->SetPlayerPos(_x, _y, _z);
			//----------------------------------//
			gCam.Strafe(100.0f*dt);
		}
	}


	// �׸���
	void Draw()
	{
		// �� �׸���
		mDrawManager->DrawIns(DXGI_FORMAT_R32_UINT);
	}

	// Ű �̺�Ʈ
	void KeyEvent(WPARAM& wParam)
	{
		// ������ ����
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

