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

		
		// ���� ��ü �׽�Ʈ��
		mModelManager->AddModel("Model0", 100.0f, 100.0f, 100.0f );
		mModelManager->AddModel("Model1", 100.0f, 100.0f, 200.0f );
		mModelManager->AddModel("Model2", 100.0f, 100.0f, 250.0f );

		mModelManager->AddModel("Model3", 100.0f, 100.0f, 350.0f ); // <-- ��� ���� ����ũ�ڵ�� 0������ ����
		mModelManager->AddModel("Model3", 100.0f, 100.0f, 470.0f ); //     ������ ���� �� ����� ����ũ �ڵ尡 1�� ������ 
		mModelManager->AddModel("Model4", 100.0f, 100.0f, 470.0f );
		

		
		// ��ũ�� �߰�
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
		for (int i = 0; i < MAX_SESSION; i++)
		{
			if (Player_MGR->GetConnectUser(i)->GetDrawFlag()==true) //�÷��̾ �������� ��� 
			{
				//������ �÷��̾� ĳ���͸� ���� 
				mModelManager->AddModel("Model1", Player_MGR->GetConnectUser(i)->Get_Posx(),
					Player_MGR->GetConnectUser(i)->Get_Posy(),
					Player_MGR->GetConnectUser(i)->Get_Posz());
				cout << "�÷��̾� ����--" << "x��ǥ" << Player_MGR->GetConnectUser(i)->Get_Posx() <<
					"  Y��ǥ " << Player_MGR->GetConnectUser(i)->Get_Posy() <<
					"  Z��ǥ " << Player_MGR->GetConnectUser(i)->Get_Posz() << endl;
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
				cout << "���� ����--" << i << endl;
				mModelManager->AddModel("Model2", Monster_MGR->GetConnectMonster(i)->Get_Posx(), Monster_MGR->GetConnectMonster(i)->Get_Posy(),
								Monster_MGR->GetConnectMonster(i)->Get_Posz());
				cout << "���� ����--" << "x��ǥ" << Monster_MGR->GetConnectMonster(i)->Get_Posx() <<
					"  Y��ǥ " << Monster_MGR->GetConnectMonster(i)->Get_Posy() <<
					"  Z��ǥ " << Monster_MGR->GetConnectMonster(i)->Get_Posz() << endl;

				Monster_MGR->GetConnectMonster(i)->SetDrawFlag(false); //�׷����� false�� �ٲ� �ٽô� �׸��� �ʰ� �����.
			}
		}

		
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
		UINT ID = Player_MGR->MyID; //���� ID
		// ������ ����.
		BYTE pData[4] = { 0, };
		memcpy(pData, &ID, sizeof(UINT)); //ID��  ��Ŷ���ۿ� ����ִ´� 

		// ������ ����
		switch (wParam)
		{
		case 'W':
			Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_W, pData, sizeof(UINT)); //�������� ��Ŷ����
			//mModelManager->SetPos(1, "Model3", 1500.0f, 100.0f, 100.0f + 100.0f);
			break;

		case 'A':
			//Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_A, pData, sizeof(UINT)); //�������� ��Ŷ����
			//mModelManager->SetPos(1, "Model3", 100.0f + 100.0f, 100.0f, 100.0f);
			break;

		case 'S':
			//Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_S, pData, sizeof(UINT)); //�������� ��Ŷ����
			//mModelManager->SetPos(1, "Model3", 100.0f, 100.0f, 100.0f - 100.0f);
			break;

		case 'D':
			//Cla_ServerMGR->ClientNet()->WritePacket(SPUSH_D, pData, sizeof(UINT)); //�������� ��Ŷ����
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

