#include "stdafx.h"
#include "cEngine.h"

extern cCam gCam;

DWORD Protocol; //��������
BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };  //��Ŷ����ũ��
DWORD PacketLength;             //��Ŷ����

// ���� Ŭ���� ������
cEngine::cEngine(HINSTANCE hInstance)
: cInitD3D(hInstance)
{
}

// ���� Ŭ���� �Ҹ���
cEngine::~cEngine()
{
	//------------------------------------------------------//
	// �̱��� Ŭ������ ����
	//------------------------------------------------------//
	// ������Ʈ ���� ���� ����
	cGameStateSave::DestroyInstance();

	// ���� ���� �Ŵ��� ����
	cGameStateManager::GetInstance()->ClearClass();
	cGameStateManager::DestroyInstance();

	// �Ŵ��� Ŭ������ ����
	cModelManager   ::GetInstance()->ClearClass();
	cModelManager   ::DestroyInstance();
	cAniManager     ::GetInstance()->ClearClass();
	cAniManager     ::DestroyInstance();
	cShaderManager  ::GetInstance()->ClearClass();
	cShaderManager  ::DestroyInstance();
	cMaterialManager::GetInstance()->ClearClass();
	cMaterialManager::DestroyInstance();
	cLightManager   ::GetInstance()->ClearClass();
	cLightManager   ::DestroyInstance();
	cDrawManager    ::GetInstance()->ClearClass();
	cDrawManager    ::DestroyInstance();

	//�÷��̾�Ŵ��� ����
	Player_MGR->Destroy();

	//���� �Ŵ��� ����
	Monster_MGR->Destroy();

	//�����Ŵ��� ����
	Cla_ServerMGR->Release();

}

// ȭ�� �ٽ� �����
void cEngine::OnResize()
{
	// ���̷�Ʈ ���۵� �����
	cInitD3D::OnResize();
	
	// ī�޶� �ٽ� �����
	gCam.SetLens(0.25f * cMathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	// ī�޶� �� �������� ������Ʈ
	//cCam::GetInstance()->FrustumProjection();

}

// ���� Ŭ���� �ʱ�ȭ
bool cEngine::Init()
{
	// ī�޶� �ʱ�ȭ
	gCam.SetPosition(0.0f, 2.0f, -15.0f);

	// ���̷�Ʈ �ʱ�ȭ
	if (!cInitD3D::Init())
		return false;

	//���� ��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	Cla_ServerMGR->Init();

	//�÷��̾� ���� �Ŵ��� �ʱ�ȭ
	Player_MGR->Begin();

	//���� ���� �Ŵ��� �ʱ�ȭ
	Monster_MGR->Begin();


	//���� ����
	Cla_ServerMGR->ClientNet()->BeginTcp(SERVER_IP, DEFAULT_PORT);
	//while (Cla_ServerMGR->GetServer_Login() == false)
	//{
	//	//���� ��Ŷ �ѹ� �ޱ�
	//	if (Cla_ServerMGR->ClientNet()->ReadPacket(Protocol, Packet, PacketLength))
	//	{
	//		//	//�Լ������� �迭 ����κ� (�� ���̽��� ���� ���� �Ѵ�.)
	//		if (Protocol != 0x3000000)
	//			ClientProtocol_Func::g_Protocol_Func[Protocol](Packet, PacketLength);
	//	}
	//}

	// �Ŵ��� �ʱ�ȭ
	cLightManager   ::GetInstance()->Init();
	cMaterialManager::GetInstance()->Init();
	cShaderManager  ::GetInstance()->Init();
	cModelManager   ::GetInstance()->Init();
	cDrawManager    ::GetInstance()->Init();

	// ���� ���� �ʱ�ȭ
	cGameStateManager::GetInstance()->Init();
	mGameStateManager = cGameStateManager::GetInstance();

	return true;
}

// ������Ʈ �Ŵ��� �߰�

// ������Ʈ
void cEngine::UpdateScene(float dt)
{
	// �� ������Ʈ ������Ʈ
	mGameStateManager->Update(dt);
}
   
// �׸���
void cEngine::DrawScene()
{
	// ȭ�� Ŭ����
	md3dImmediateContext->ClearRenderTargetView(mSreenRTV, reinterpret_cast<const float*>(&Colors::Blue));
	md3dImmediateContext->ClearDepthStencilView(mMainDSV , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Get ���� ���� ���ٽ� ����
	ID3D11DepthStencilState* pPrevDepthState;
	UINT nPrevStencil;
	md3dImmediateContext->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

	//���� ��Ŷ �ޱ�
	while (Cla_ServerMGR->ClientNet()->ReadPacket(Protocol, Packet, PacketLength))
	{
		//	//�Լ������� �迭 ����κ� (�� ���̽��� ���� ���� �Ѵ�.)
		if (Protocol != 0x3000000)
			ClientProtocol_Func::g_Protocol_Func[Protocol](Packet, PacketLength);
	}

	//-----------------------------------------------------------------------------------------------//
	// �� ������Ʈ �׸���
	mGameStateManager->Draw();
	//-----------------------------------------------------------------------------------------------//

	// Set ���� ���Ľ� �ǵ�����
	md3dImmediateContext->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
	ReleaseCOM(pPrevDepthState);

	HR(mSwapChain->Present(NULL, NULL));
}
