#pragma once
#include "cGameState.h"

class cMainState : public cGameState
{
public:
	// �ʱ�ȭ
	void Init()
	{
		// ���� �� ������ ���� ( ��Ȯ�����ϸ� ���� ��Ʈ���� )
		mModelManager->ClearModel();

		//// �� �߰�
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

		
		// ���� ��ü �׽�Ʈ��
		mModelManager->AddModel("Model0", 100.0f, 100.0f, 100.0f );
		mModelManager->AddModel("Model1", 100.0f, 100.0f, 200.0f );
		mModelManager->AddModel("Model2", 100.0f, 100.0f, 250.0f );
		mModelManager->AddModel("Model3", 100.0f, 100.0f, 350.0f ); // <-- ��� ���� ����ũ�ڵ�� 0������ ����
		mModelManager->AddModel("Model3", 100.0f, 100.0f, 470.0f ); //     ������ ���� �� ����� ����ũ �ڵ尡 1�� ������ 
		mModelManager->AddModel("Model4", 100.0f, 100.0f, 470.0f );
		
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
		// �� �׸��� ( �ڽ� )
		mDrawManager->DrawIns(DXGI_FORMAT_R32_UINT);
	}

	// Ű �̺�Ʈ
	void KeyEvent(WPARAM& wParam)
	{
		// ������ ����
		switch (wParam)
		{
		case 'W':
			mModelManager->SetPos(1, "Model3", 100.0f, 100.0f, 100.0f + 100.0f);
			break;

		case 'A':
			mModelManager->SetPos(1, "Model3", 100.0f + 100.0f, 100.0f, 100.0f);
			break;

		case 'S':
			mModelManager->SetPos(1, "Model3", 100.0f, 100.0f, 100.0f - 100.0f);
			break;

		case 'D':
			mModelManager->SetPos(1, "Model3", 100.0f - 100.0f, 100.0f, 100.0f);
			break;

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

