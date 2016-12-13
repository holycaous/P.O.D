#include "stdafx.h"
#include "cEngine.h"

extern cCam gCam;

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
	cHDRManager     ::GetInstance()->ClearClass();
	cHDRManager     ::DestroyInstance();
	cMapManager     ::DestroyInstance();
	cShadowMap      ::DestroyInstance();

	cCoreStorage::GetInstance()->ClearClass();
	cCoreStorage::DestroyInstance();
}

// 화면 다시 만들기
void cEngine::OnResize()
{
	// 다이렉트 버퍼들 만들기
	cInitD3D::OnResize();
	
	// 카메라 다시 만들기
	gCam.SetLens(0.25f * cMathHelper::Pi, AspectRatio());

	// 카메라 뷰 프리스텀 업데이트
	//gCam.FrustumProjection();

}

// 엔진 클래스 초기화
bool cEngine::Init()
{
	// 다이렉트 초기화
	if (!cInitD3D::Init())
		return false;

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
#ifdef POSTEFFECT_ON
	md3dImmediateContext->ClearRenderTargetView(mHDRRTV, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
#else
	md3dImmediateContext->ClearRenderTargetView(mScreenRTV, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
#endif
	md3dImmediateContext->ClearDepthStencilView(mMainDSV , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Get 현재 깊이 스텐실 저장
	ID3D11DepthStencilState* pPrevDepthState;
	UINT nPrevStencil;
	md3dImmediateContext->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

	//-----------------------------------------------------------------------------------------------//
	// 각 스테이트 그리기
	mGameStateManager->Draw();
	//-----------------------------------------------------------------------------------------------//

	// Set 깊이 스탠실 되돌리기
	md3dImmediateContext->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
	ReleaseCOM(pPrevDepthState);

	HR(mSwapChain->Present(NULL, NULL));
}
