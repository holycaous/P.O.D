#include "stdafx.h"
#include "cInitD3D.h"
#include <WindowsX.h>
#include <sstream>

// 다이렉트 초기화 변수
namespace
{
	cInitD3D* gcInitD3D = 0;
}

// 다이렉트 클래스 초기화 (생성자)
// - 클래스 내 맴버변수들을 초기화 하고, 객체를 할당한다. (프로그램 이름) 
cInitD3D::cInitD3D(HINSTANCE hInstance)
: mhAppInst(hInstance),
mMainWndCaption(L"POD_D3DX11 (ver1.0)"),
md3dDriverType(D3D_DRIVER_TYPE_HARDWARE),
mClientWidth(WIN_X),
mClientHeight(WIN_Y),
mEnable4xMsaa(false),
mhMainWnd(0),
mAppPaused(false),
mMinimized(false),
mMaximized(false),
mResizing(false),
m4xMsaaQuality(0),

md3dDevice(0),
md3dImmediateContext(0),
mSwapChain(0),
mDepthStencilTexture(0),
mNomalSRV(0),
mDepthSRV(0),
mPositionSRV(0),
mColorSRV(0),
mSpecularSRV(0),
mShadowSRV(0),
mSreenRTV(0),
mNomalRTV(0),
mSpecularRTV(0),
mPositionRTV(0),
mDepthRTV(0),
mShadowRTV(0),
mMainDSV(0),
mOnlyReadDSV(0),
mDepthStencilState(0),
mColorRTV(0)
{
	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));
	gcInitD3D = this;
}

// 소멸자
cInitD3D::~cInitD3D()
{
	// 리소스 뷰 클리어
	ClearResourceView();

	// 디바이스 클리어
	ClearDevice();
}

// 리소스 뷰 클리어
void cInitD3D::ClearResourceView()
{
	// 렌더 타겟
	ReleaseCOM(mNomalRTV);
	ReleaseCOM(mPositionRTV);
	ReleaseCOM(mSpecularRTV);
	ReleaseCOM(mColorRTV);
	ReleaseCOM(mDepthRTV);
	ReleaseCOM(mShadowRTV);

	// 버퍼
	ReleaseCOM(mNomalSRV);
	ReleaseCOM(mPositionSRV);
	ReleaseCOM(mSpecularSRV);
	ReleaseCOM(mColorSRV);
	ReleaseCOM(mDepthSRV);
	ReleaseCOM(mShadowSRV);

	ReleaseCOM(mDepthStencilState);
	ReleaseCOM(mDepthStencilTexture);

	ReleaseCOM(mHDRTexture);
	ReleaseCOM(mHDRRTV);
	ReleaseCOM(mHDRSRV);

	ReleaseCOM(mSreenRTV);
	ReleaseCOM(mOnlyReadDSV);
	ReleaseCOM(mMainDSV);
}

// 디바이스 클리어
void cInitD3D::ClearDevice()
{
	// Restore all default settings.
	if (md3dImmediateContext)
		md3dImmediateContext->ClearState();

	ReleaseCOM(mSwapChain);
	ReleaseCOM(md3dImmediateContext);
	ReleaseCOM(md3dDevice);

	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));
}

// 디바이스 재 생성
bool cInitD3D::ReCreateDevice()
{
	// 리소스 뷰 클리어
	ClearResourceView();

	// 디바이스 클리어
	ClearDevice();

	// 디바이스 생성
	if (!InitDirect3D())
		return false;

	// 화면 재구성
	OnResize();

	return true;
}

// D3D 실행 (메세지 루프)
int cInitD3D::Run()
{
	MSG msg = { 0 };
	mTimer.Reset();

	// 메세지 루프
	while (msg.message != WM_QUIT)
	{
		// 윈도우 메세지가 들어왔는가?
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 아니라면, 렌더링
		else
		{
			// 타이머
			mTimer.Tick();

			if (!mAppPaused)
			{
				CalculateFrameStats();
				UpdateScene(mTimer.DeltaTime());
				DrawScene();
			}
			else
			{
				// 프레임 재계산하는 시간
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

// 다이렉트 클래스 초기화
bool cInitD3D::Init()
{
	if (!InitMainWindow())
		return false;

	if (!InitDirect3D())
		return false;

	return true;
}

void cInitD3D::SetCoreStorage()
{
	// 코어 저장소에 값 저장
	cCoreStorage::GetInstance()->mInst = &mhAppInst;
	cCoreStorage::GetInstance()->mhWnd = &mhMainWnd;

	cCoreStorage::GetInstance()->mTimer = &mTimer;

	cCoreStorage::GetInstance()->md3dDevice  		  = md3dDevice;
	cCoreStorage::GetInstance()->md3dImmediateContext = md3dImmediateContext;
	cCoreStorage::GetInstance()->mSwapChain		      = mSwapChain;
	cCoreStorage::GetInstance()->mDepthStencilTexture = mDepthStencilTexture;
	cCoreStorage::GetInstance()->mHDRTexture          = mHDRTexture;
	
	// SRV
	cCoreStorage::GetInstance()->mNomalSRV    = mNomalSRV;
	cCoreStorage::GetInstance()->mColorSRV    = mColorSRV;
	cCoreStorage::GetInstance()->mDepthSRV    = mDepthSRV;
	cCoreStorage::GetInstance()->mPositionSRV = mPositionSRV;
	cCoreStorage::GetInstance()->mSpecularSRV = mSpecularSRV;
	cCoreStorage::GetInstance()->mShadowSRV   = mShadowSRV;
	cCoreStorage::GetInstance()->mHDRSRV      = mHDRSRV;

	// RTV
	cCoreStorage::GetInstance()->mNomalRTV    = mNomalRTV;
	cCoreStorage::GetInstance()->mPositionRTV = mPositionRTV;
	cCoreStorage::GetInstance()->mSpecularRTV = mSpecularRTV;
	cCoreStorage::GetInstance()->mColorRTV    = mColorRTV;
	cCoreStorage::GetInstance()->mDepthRTV    = mDepthRTV;
	cCoreStorage::GetInstance()->mShadowRTV   = mShadowRTV;
	cCoreStorage::GetInstance()->mSreenRTV	  = mSreenRTV; // 화면과 연결된
	cCoreStorage::GetInstance()->mHDRRTV      = mHDRRTV;   // 화면과 연결된

	// DSV
	cCoreStorage::GetInstance()->mMainDSV	  = mMainDSV;
	cCoreStorage::GetInstance()->mOnlyReadDSV = mOnlyReadDSV;

	// DSV 상태
	cCoreStorage::GetInstance()->mDepthStencilState = mDepthStencilState;

	// ViewPort
	cCoreStorage::GetInstance()->mScreenViewport = &mScreenViewport;
}

// 화면크기 변환 시 재구성 
void cInitD3D::OnResize()
{
	assert(md3dImmediateContext);
	assert(md3dDevice);
	assert(mSwapChain);

	// 화면크기가 변함에 따라 사용할수 없는 기존객체를 파괴한다.
	// 리소스 뷰 클리어
	ClearResourceView();

	// 스왑 체인의 크기를 조정하고 렌더 타겟 뷰를 다시 만든다.
	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// 벡버퍼라는 텍스처를 만들고, 렌더 타겟뷰로 설정한다. 렌더 타겟뷰를 만들었으면 제거한다.
	ID3D11Texture2D* backBuffer = nullptr;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mSreenRTV));
	ReleaseCOM(backBuffer);

	// 코어에 저장
	cCoreStorage::GetInstance()->mClientWidth  = static_cast<float>(mClientWidth);
	cCoreStorage::GetInstance()->mClientHeight = static_cast<float>(mClientHeight);

	// Texture formats
	static const DXGI_FORMAT depthStencilTextureFormat      = DXGI_FORMAT_R24G8_TYPELESS;
	static const DXGI_FORMAT positionTextureFormat          = DXGI_FORMAT_R32G32B32A32_FLOAT;
	static const DXGI_FORMAT basicColorTextureFormat        = DXGI_FORMAT_R8G8B8A8_UNORM;     // DXGI_FORMAT_R8G8B8A8_UNORM
	static const DXGI_FORMAT normalTextureFormat            = DXGI_FORMAT_R11G11B10_FLOAT;    
	static const DXGI_FORMAT SpecularTextureFormat          = DXGI_FORMAT_R8G8B8A8_UNORM;    // DXGI_FORMAT_R8G8B8A8_UNORM

	// Render view formats
	static const DXGI_FORMAT depthStencilRenderViewFormat   = DXGI_FORMAT_D24_UNORM_S8_UINT;
	static const DXGI_FORMAT positionRenderViewFormat       = DXGI_FORMAT_R32G32B32A32_FLOAT; 
	static const DXGI_FORMAT basicColorRenderViewFormat     = DXGI_FORMAT_R8G8B8A8_UNORM;     // DXGI_FORMAT_R8G8B8A8_UNORM
	static const DXGI_FORMAT normalRenderViewFormat         = DXGI_FORMAT_R11G11B10_FLOAT;    
	static const DXGI_FORMAT SpecularRenderViewFormat       = DXGI_FORMAT_R8G8B8A8_UNORM;    // DXGI_FORMAT_R8G8B8A8_UNORM

	// Resource view formats
	static const DXGI_FORMAT depthStencilResourceViewFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	static const DXGI_FORMAT positionResourceViewFormat     = DXGI_FORMAT_R32G32B32A32_FLOAT;
	static const DXGI_FORMAT basicColorResourceViewFormat   = DXGI_FORMAT_R8G8B8A8_UNORM;     // DXGI_FORMAT_R8G8B8A8_UNORM
	static const DXGI_FORMAT normalResourceViewFormat       = DXGI_FORMAT_R11G11B10_FLOAT;
	static const DXGI_FORMAT SpecularResourceViewFormat     = DXGI_FORMAT_R8G8B8A8_UNORM;    // DXGI_FORMAT_R8G8B8A8_UNORM

	// 텍스처 뼈대 설정
	D3D11_TEXTURE2D_DESC TextureDesc;
	TextureDesc.Width              = mClientWidth;
	TextureDesc.Height             = mClientHeight;
	TextureDesc.MipLevels          = 1;
	TextureDesc.ArraySize          = 1;
	TextureDesc.Format             = DXGI_FORMAT_UNKNOWN; 
	TextureDesc.SampleDesc.Count   = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage              = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags     = 0;
	TextureDesc.MiscFlags          = 0;

	// DSV 뼈대 설정
	D3D11_DEPTH_STENCIL_VIEW_DESC DsvD;
	DsvD.Format                        = depthStencilRenderViewFormat;
	DsvD.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
	DsvD.Flags		                   = 0;
	DsvD.Texture2D.MipSlice            = 0;

	// SRV 뼈대 설정
	D3D11_SHADER_RESOURCE_VIEW_DESC SRV_Desc;
	SRV_Desc.Format                    = basicColorResourceViewFormat;
	SRV_Desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRV_Desc.Texture2D.MipLevels       = TextureDesc.MipLevels;
	SRV_Desc.Texture2D.MostDetailedMip = 0;

	// RTV 뼈대 설정
	D3D11_RENDER_TARGET_VIEW_DESC RTV_Desc;
	RTV_Desc.Format                    = basicColorResourceViewFormat;
	RTV_Desc.ViewDimension             = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTV_Desc.Texture2D.MipSlice        = 0;

	// 깊이 버퍼 설정
	TextureDesc.Format = depthStencilTextureFormat;
	HR(md3dDevice->CreateTexture2D       (&TextureDesc        , 0    , &mDepthStencilTexture));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilTexture, &DsvD, &mMainDSV));

	// 읽기전용 깊이버퍼
	DsvD.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilTexture, &DsvD, &mOnlyReadDSV));

	//// 깊이 버퍼 텍스처 만들기 & 깊이 버퍼 연결하기
	//SRV_Desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	//HR(md3dDevice->CreateShaderResourceView(mDepthStencilTexture, &SRV_Desc, &mDepthSRV));

	// 텍스처 형식 바꾸기
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	//--------------------------------------------------------------------------------------//
	// 깊이 버퍼 텍스처 만들기 & 깊이 버퍼 연결하기
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = basicColorTextureFormat;
	SRV_Desc.Format    = basicColorRenderViewFormat;
	RTV_Desc.Format    = basicColorRenderViewFormat;

	ID3D11Texture2D* mDepthTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mDepthTex));
	HR(md3dDevice->CreateRenderTargetView  (mDepthTex   , &RTV_Desc, &mDepthRTV));
	HR(md3dDevice->CreateShaderResourceView(mDepthTex   , &SRV_Desc, &mDepthSRV));

	//--------------------------------------------------------------------------------------//
	// 칼라 버퍼 텍스처 만들기 & 포지션 버퍼 연결하기
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = basicColorTextureFormat;
	SRV_Desc.Format    = basicColorRenderViewFormat;
	RTV_Desc.Format    = basicColorRenderViewFormat;

	ID3D11Texture2D* mColorTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mColorTex));
	HR(md3dDevice->CreateRenderTargetView  (mColorTex   , &RTV_Desc, &mColorRTV));
	HR(md3dDevice->CreateShaderResourceView(mColorTex   , &SRV_Desc, &mColorSRV));

	//--------------------------------------------------------------------------------------//
	// 노멀 버퍼 텍스처 만들기 & 노멀 버퍼 연결하기
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = normalTextureFormat;
	SRV_Desc.Format    = normalRenderViewFormat;
	RTV_Desc.Format    = normalRenderViewFormat;
	
	ID3D11Texture2D* mNomalTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0        , &mNomalTex));
	HR(md3dDevice->CreateRenderTargetView  (mNomalTex   , &RTV_Desc, &mNomalRTV));
	HR(md3dDevice->CreateShaderResourceView(mNomalTex   , &SRV_Desc, &mNomalSRV));

	//--------------------------------------------------------------------------------------//
	// 포지션 버퍼 텍스처 만들기 & 포지션 버퍼 연결하기
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = positionTextureFormat;
	SRV_Desc.Format    = positionRenderViewFormat;
	RTV_Desc.Format    = positionRenderViewFormat;

	ID3D11Texture2D* mPositionTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mPositionTex));
	HR(md3dDevice->CreateRenderTargetView  (mPositionTex, &RTV_Desc, &mPositionRTV));
	HR(md3dDevice->CreateShaderResourceView(mPositionTex, &SRV_Desc, &mPositionSRV));

	//--------------------------------------------------------------------------------------//
	// 쉐도우 버퍼 텍스처 만들기 & 쉐도우 버퍼 연결하기
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = positionTextureFormat;
	SRV_Desc.Format    = positionRenderViewFormat;
	RTV_Desc.Format    = positionRenderViewFormat;

	ID3D11Texture2D* mShadowTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mShadowTex));
	HR(md3dDevice->CreateRenderTargetView  (mShadowTex  , &RTV_Desc, &mShadowRTV));
	HR(md3dDevice->CreateShaderResourceView(mShadowTex  , &SRV_Desc, &mShadowSRV));

	//--------------------------------------------------------------------------------------//
	// 스팩큘러 버퍼 텍스처 만들기 & 스팩큘러 버퍼 연결하기
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = SpecularTextureFormat;
	SRV_Desc.Format    = SpecularRenderViewFormat;
	RTV_Desc.Format    = SpecularRenderViewFormat;

	ID3D11Texture2D* mSpecTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mSpecTex));
	HR(md3dDevice->CreateRenderTargetView  (mSpecTex	, &RTV_Desc, &mSpecularRTV));
	HR(md3dDevice->CreateShaderResourceView(mSpecTex	, &SRV_Desc, &mSpecularSRV));

	//--------------------------------------------------------------------------------------//
	// 텍스처 해제
	//--------------------------------------------------------------------------------------//
	ReleaseCOM(mDepthTex);
	ReleaseCOM(mColorTex);
	ReleaseCOM(mNomalTex);
	ReleaseCOM(mPositionTex);
	ReleaseCOM(mSpecTex);
	ReleaseCOM(mShadowTex);

	// 파이프 라인에 뷰와 깊이, 스탠실버퍼를 바인딩 한다. (1개를 묶고 있음)
	// CreateDepthStencilView에서는 렌더타겟 뷰와 함께 깊이/스텐실 버퍼도 동시에 같이 설정합니다
	md3dImmediateContext->OMSetRenderTargets(1, &mSreenRTV, mMainDSV);

	// 뷰포트 변환을 한다.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width    = static_cast<float>(mClientWidth);
	mScreenViewport.Height   = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);

	//--------------------------------------------------------------------------------------//
	// 상태
	//--------------------------------------------------------------------------------------//
	D3D11_DEPTH_STENCIL_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.DepthEnable                          = TRUE;
	descDepth.DepthWriteMask                       = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc                            = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable                        = TRUE;
	descDepth.StencilReadMask                      = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask                     = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC stencilMarkOp = { D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS };
	descDepth.FrontFace                            = stencilMarkOp;
	descDepth.BackFace                             = stencilMarkOp;

	// Create the depth stencil state.
	md3dDevice->CreateDepthStencilState(&descDepth, &mDepthStencilState);

	// 코어에 저장
	SetCoreStorage();

	// 쉐도우 맵 초기화
	cShadowMap::GetInstance()->ClearClass();
	cShadowMap::GetInstance()->InitClass(mClientWidth, mClientHeight);

	//-----------------------------------------------------------------------------//
	// HDR 렌더 대상을 만듭니다.
	//-----------------------------------------------------------------------------//
	// Texture
	D3D11_TEXTURE2D_DESC texDesc = {
		mClientWidth,						  				     //UINT Width;
		mClientHeight,											 //UINT Height;
		1,														 //UINT MipLevels;
		1,														 //UINT ArraySize;
		DXGI_FORMAT_R16G16B16A16_TYPELESS,						 //DXGI_FORMAT Format;
		1,														 //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,									 //D3D11_USAGE Usage;
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,	 //UINT BindFlags;
		0,														 //UINT CPUAccessFlags;
		0														 //UINT MiscFlags;    
	};
	HR(md3dDevice->CreateTexture2D(&texDesc, 0, &mHDRTexture));

	// RTV
	D3D11_RENDER_TARGET_VIEW_DESC rtsvd =
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_RTV_DIMENSION_TEXTURE2D
	};
	HR(md3dDevice->CreateRenderTargetView(mHDRTexture, &rtsvd, &mHDRRTV));

	// SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd =
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0,
		0
	};
	dsrvd.Texture2D.MipLevels = 1;
	HR(md3dDevice->CreateShaderResourceView(mHDRTexture, &dsrvd, &mHDRSRV));

	// HDR 초기화
	cHDRManager::GetInstance()->ClearClass();
	cHDRManager::GetInstance()->InitClass(mClientWidth, mClientHeight);

}

// 메인 프로시져 (매세지 프로시져 실행)
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gcInitD3D->MsgProc(hwnd, msg, wParam, lParam);
}

// 디바이스 체크
void cInitD3D::CheckDeviceLost()
{
	HRESULT hr;

	// 디바이스 로스트가 일어났는지
	hr = md3dDevice->GetDeviceRemovedReason();

	switch (hr)
	{
	// 성공
	case S_OK:
		break;

	// 행 아웃, 리셋
	case DXGI_ERROR_DEVICE_HUNG:
	case DXGI_ERROR_DEVICE_RESET:

		// 실패 시 어플리케이션 종료
		if(ReCreateDevice())
			PostQuitMessage(0);

		// 실패 시 어플리케이션 종료
		if (FAILED(hr))
			PostQuitMessage(0);

		break;

	// 실패 시 어플리케이션 종료
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
	case DXGI_ERROR_INVALID_CALL:
	default:
		cout << "디바이스 로스트" << endl;
		PostQuitMessage(0);
		break;
	}
}

// 메세지 프로시져
LRESULT cInitD3D::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 디바이스 로스트 검사
	if (md3dDevice)
		CheckDeviceLost();

	switch (msg)
	{
	// 키보드와 마우스관련된 것들을 스테이트 매니저로 넘기며,
	// 이곳은 최상위 명령어 코드로 사용한다.

	//---------------------------------------------------------------------------------------
	// 키보드
	//---------------------------------------------------------------------------------------
	// 이거때문에 3회가 눌림.
	//case WM_CHAR:
	//case WM_KEYUP:
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;

		case '1':
			cGameStateSave::GetInstance()->m_StateChaneNumber = e_MainState;
			cGameStateSave::GetInstance()->m_StateChangeCheck = true;
			return 0;

		case '2':
			cGameStateSave::GetInstance()->m_StateChaneNumber = e_PlayState;
			cGameStateSave::GetInstance()->m_StateChangeCheck = true;
			return 0;

		case '3':
			cGameStateSave::GetInstance()->m_StateChaneNumber = e_MapTool;
			cGameStateSave::GetInstance()->m_StateChangeCheck = true;
			return 0;

		default:
			cGameStateManager::GetInstance()->KeyEvent(wParam);
			return 0;
		}
		return 0;

		//---------------------------------------------------------------------------------------
		// 마우스
		//---------------------------------------------------------------------------------------
		// 마우스 클릭
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		cGameStateManager::GetInstance()->MouseEvent(msg, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		return 0;

		// 마우스 이동
	case WM_MOUSEMOVE:
		//cout << GET_X_LPARAM(lParam) << ", " << GET_Y_LPARAM(lParam) << endl;

		cGameStateManager::GetInstance()->MouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	
		//---------------------------------------------------------------------------------------
		// 윈도우 보조
		//---------------------------------------------------------------------------------------
#ifdef WINDOW_DISABLE_NO_RENDER
		case WM_ACTIVATE:
			// 윈도우 활성, 비활성
			// 윈도우 활성, 비활성화에 따라, 타이머 또한 활성, 비활성화를 한다.
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				mAppPaused = true;
				mTimer.Stop();
			}
			else
			{
				mAppPaused = false;
				mTimer.Start();
			}
			return 0;
#else
#endif

	case WM_SIZE:
		// 윈도우 사이즈가 변경됬는가?
		// 새로운 클라이언트 영역의 크기를 저장
		mClientWidth  = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if (md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				// 최소화 상태에서 복원됬는가?
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}
				// 최대화 상태에서 복원됬는가?
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				// 사용자가 크기 조정 막대를 드래그하는가?
				// 드래그 중에는 아무작업도 하지 않는다.
				else if (mResizing)
				{

				}
				// 끝났으면 업데이트 요청
				else 
				{
					OnResize();
				}
			}
		}
		return 0;

		//WM_EXITSIZEMOVE는 사용자가 크기 조정 막대를 잡고있을 때 전송된다.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing  = true;
		mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE는 사용자가 크기 조정 막대를 놓을 때 전송된다.
		// 여기에서 새로운 창 크기에 따라 모든 것을 다시 설정.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing  = false;
		mTimer.Start();
		OnResize();
		return 0;

		// 윈도우가 파괴 될 때, WM_DESTROY가 전송됩니다.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// 메뉴는 사용자가 활성했을 때 WM_MENUCHAR의 메시지가 전송된다.
		// 어떤 니모닉 또는 액셀러레이터 키에 해당하지 않는 키를 누릅니다.
	case WM_MENUCHAR:
		//우리가 ALT키를 입력 할 때, 삐 소리하지 마십시오.
		return MAKELRESULT(0, MNC_CLOSE);

		// 너무 창이 작아지는 것을 방지하기 위해.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


// 윈도우 초기화
bool cInitD3D::InitMainWindow()
{
	WNDCLASS wc;
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhAppInst;
	wc.hIcon		 = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// 클라이언트 크기 계산
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	// 윈도우 생성 및 검사
	mhMainWnd = CreateWindow(L"D3DWndClassName", mMainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0);
	if (!mhMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// 윈도우 출력
	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
}

// 다이렉트 객체 초기화
// - 각종기능을 지원하는가 체크하고(다중 샘플링 등), 스왑체인, 컴 객체 등을 생성, 제거한다.
bool cInitD3D::InitDirect3D()
{
	// 디바이스, 디바이스 컨텍스트 생성
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		md3dDriverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// 백버퍼에 4X MSAA 품질 지원을 확인
	HR(md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	assert(m4xMsaaQuality > 0);

	// 스왑체인 생성
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width                   = mClientWidth;
	sd.BufferDesc.Height                  = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator   = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

	// 다중 샘플링 유무 (Use 4X MSAA?) 
	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	// 버퍼 생성
	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = mhMainWnd; // 여기서 윈도우 핸들 지정 (출력 DC 지정)
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags		= 0;

	// 디바이스 생성
	IDXGIDevice* dxgiDevice = 0;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// 스왑체인 생성 및 확인
	HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));

	// 컴 객체 삭제
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// 창 크기 조절 (코드 중복 피하기 위해 사용)
	OnResize();

	return true;
}

// 프레임 상태를 계산한다. (FPS)
void cInitD3D::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

// Get 함수들
HINSTANCE cInitD3D::AppInst()const
{
	return mhAppInst;
}
HWND cInitD3D::MainWnd()const
{
	return mhMainWnd;
}
float cInitD3D::AspectRatio()const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}
