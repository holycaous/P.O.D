#pragma once
#include "cGameState.h"

class cPlayState : public cGameState
{
public:
	// �ʱ�ȭ
	void Init()
	{
		// ���� �� ������ ���� ( ��Ȯ�����ϸ� ���� ��Ʈ���� )
		mModelManager->ClearModel();

		// �� �߰�
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

		// ��ũ�� �߰� (Ǯ ��ũ������)
		mModelManager->AddScreen(0.0f, 0.0f, 140.0f);

		// �ش� �ν��Ͻ� ���۸� ����ڴ�..
		mModelManager->MakeInsbuf();
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

