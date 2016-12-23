#include "stdafx.h"
#include "cEngine.h"

extern cCam gCam;

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
	cHDRManager     ::GetInstance()->ClearClass();
	cHDRManager     ::DestroyInstance();
	cMapManager     ::DestroyInstance();
	cShadowMap      ::DestroyInstance();

	cCoreStorage::GetInstance()->ClearClass();
	cCoreStorage::DestroyInstance();
}

// ȭ�� �ٽ� �����
void cEngine::OnResize()
{
	// ���̷�Ʈ ���۵� �����
	cInitD3D::OnResize();
	
	// ī�޶� �ٽ� �����
	gCam.SetLens(0.25f * cMathHelper::Pi, AspectRatio());

	// ī�޶� �� �������� ������Ʈ
	//gCam.FrustumProjection();

}

// ���� Ŭ���� �ʱ�ȭ
bool cEngine::Init()
{
	// ���̷�Ʈ �ʱ�ȭ
	if (!cInitD3D::Init())
		return false;

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
#ifdef POSTEFFECT_ON
	md3dImmediateContext->ClearRenderTargetView(mHDRRTV, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
#else
	md3dImmediateContext->ClearRenderTargetView(mScreenRTV, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
#endif
	md3dImmediateContext->ClearDepthStencilView(mMainDSV , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Get ���� ���� ���ٽ� ����
	ID3D11DepthStencilState* pPrevDepthState;
	UINT nPrevStencil;
	md3dImmediateContext->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

	//-----------------------------------------------------------------------------------------------//
	// �� ������Ʈ �׸���
	mGameStateManager->Draw();
	//-----------------------------------------------------------------------------------------------//

	// Set ���� ���Ľ� �ǵ�����
	md3dImmediateContext->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
	ReleaseCOM(pPrevDepthState);

	HR(mSwapChain->Present(NULL, NULL));
}
