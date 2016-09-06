#include "stdafx.h"
#include "cEngine.h"

extern cCam gCam;

DWORD Protocol; //프로토콜
BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };  //패킷버퍼크기
DWORD PacketLength;             //패킷길이

// 엔진 클래스 생성자
cEngine::cEngine(HINSTANCE hInstance)
: cInitD3D(hInstance)
{
}

// 엔진 클래스 소멸자
cEngine::~cEngine()
{
	//------------------------------------------------------//
	// 싱글톤 클래스들 제거
	//------------------------------------------------------//
	// 스테이트 상태 변수 제거
	cGameStateSave::DestroyInstance();

	// 게임 상태 매니저 제거
	cGameStateManager::GetInstance()->ClearClass();
	cGameStateManager::DestroyInstance();

	// 매니저 클래스들 삭제
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

	//플레이어매니저 삭제
	Player_MGR->Destroy();

	//몬스터 매니저 삭제
	Monster_MGR->Destroy();

	//서버매니저 삭제
	Cla_ServerMGR->Release();

}

// 화면 다시 만들기
void cEngine::OnResize()
{
	// 다이렉트 버퍼들 만들기
	cInitD3D::OnResize();
	
	// 카메라 다시 만들기
	gCam.SetLens(0.25f * cMathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	// 카메라 뷰 프리스텀 업데이트
	//cCam::GetInstance()->FrustumProjection();

}

// 엔진 클래스 초기화
bool cEngine::Init()
{
	// 카메라 초기화
	gCam.SetPosition(0.0f, 2.0f, -15.0f);

	// 다이렉트 초기화
	if (!cInitD3D::Init())
		return false;

	//서버 네트워크 매니저 초기화
	Cla_ServerMGR->Init();

	//플레이어 관리 매니저 초기화
	Player_MGR->Begin();

	//몬스터 관리 매니저 초기화
	Monster_MGR->Begin();


	//서버 접속
	Cla_ServerMGR->ClientNet()->BeginTcp(SERVER_IP, DEFAULT_PORT);
	//while (Cla_ServerMGR->GetServer_Login() == false)
	//{
	//	//서버 패킷 한번 받기
	//	if (Cla_ServerMGR->ClientNet()->ReadPacket(Protocol, Packet, PacketLength))
	//	{
	//		//	//함수포인터 배열 실행부분 (즉 케이스와 같은 역할 한다.)
	//		if (Protocol != 0x3000000)
	//			ClientProtocol_Func::g_Protocol_Func[Protocol](Packet, PacketLength);
	//	}
	//}

	// 매니저 초기화
	cLightManager   ::GetInstance()->Init();
	cMaterialManager::GetInstance()->Init();
	cShaderManager  ::GetInstance()->Init();
	cModelManager   ::GetInstance()->Init();
	cDrawManager    ::GetInstance()->Init();

	// 게임 상태 초기화
	cGameStateManager::GetInstance()->Init();
	mGameStateManager = cGameStateManager::GetInstance();

	return true;
}

// 스테이트 매니저 추가

// 업데이트
void cEngine::UpdateScene(float dt)
{
	// 각 스테이트 업데이트
	mGameStateManager->Update(dt);
}
   
// 그리기
void cEngine::DrawScene()
{
	// 화면 클리어
	md3dImmediateContext->ClearRenderTargetView(mSreenRTV, reinterpret_cast<const float*>(&Colors::Blue));
	md3dImmediateContext->ClearDepthStencilView(mMainDSV , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Get 현재 깊이 스텐실 저장
	ID3D11DepthStencilState* pPrevDepthState;
	UINT nPrevStencil;
	md3dImmediateContext->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

	//서버 패킷 받기
	while (Cla_ServerMGR->ClientNet()->ReadPacket(Protocol, Packet, PacketLength))
	{
		//	//함수포인터 배열 실행부분 (즉 케이스와 같은 역할 한다.)
		if (Protocol != 0x3000000)
			ClientProtocol_Func::g_Protocol_Func[Protocol](Packet, PacketLength);
	}

	//-----------------------------------------------------------------------------------------------//
	// 각 스테이트 그리기
	mGameStateManager->Draw();
	//-----------------------------------------------------------------------------------------------//

	// Set 깊이 스탠실 되돌리기
	md3dImmediateContext->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
	ReleaseCOM(pPrevDepthState);

	HR(mSwapChain->Present(NULL, NULL));
}
