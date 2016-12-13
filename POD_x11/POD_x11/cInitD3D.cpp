#include "stdafx.h"
#include "cInitD3D.h"
#include <WindowsX.h>
#include <sstream>

// ���̷�Ʈ �ʱ�ȭ ����
namespace
{
	cInitD3D* gcInitD3D = 0;
}

// ���̷�Ʈ Ŭ���� �ʱ�ȭ (������)
// - Ŭ���� �� �ɹ��������� �ʱ�ȭ �ϰ�, ��ü�� �Ҵ��Ѵ�. (���α׷� �̸�) 
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

// �Ҹ���
cInitD3D::~cInitD3D()
{
	// ���ҽ� �� Ŭ����
	ClearResourceView();

	// ����̽� Ŭ����
	ClearDevice();
}

// ���ҽ� �� Ŭ����
void cInitD3D::ClearResourceView()
{
	// ���� Ÿ��
	ReleaseCOM(mNomalRTV);
	ReleaseCOM(mPositionRTV);
	ReleaseCOM(mSpecularRTV);
	ReleaseCOM(mColorRTV);
	ReleaseCOM(mDepthRTV);
	ReleaseCOM(mShadowRTV);

	// ����
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

// ����̽� Ŭ����
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

// ����̽� �� ����
bool cInitD3D::ReCreateDevice()
{
	// ���ҽ� �� Ŭ����
	ClearResourceView();

	// ����̽� Ŭ����
	ClearDevice();

	// ����̽� ����
	if (!InitDirect3D())
		return false;

	// ȭ�� �籸��
	OnResize();

	return true;
}

// D3D ���� (�޼��� ����)
int cInitD3D::Run()
{
	MSG msg = { 0 };
	mTimer.Reset();

	// �޼��� ����
	while (msg.message != WM_QUIT)
	{
		// ������ �޼����� ���Դ°�?
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// �ƴ϶��, ������
		else
		{
			// Ÿ�̸�
			mTimer.Tick();

			if (!mAppPaused)
			{
				CalculateFrameStats();
				UpdateScene(mTimer.DeltaTime());
				DrawScene();
			}
			else
			{
				// ������ �����ϴ� �ð�
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

// ���̷�Ʈ Ŭ���� �ʱ�ȭ
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
	// �ھ� ����ҿ� �� ����
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
	cCoreStorage::GetInstance()->mSreenRTV	  = mSreenRTV; // ȭ��� �����
	cCoreStorage::GetInstance()->mHDRRTV      = mHDRRTV;   // ȭ��� �����

	// DSV
	cCoreStorage::GetInstance()->mMainDSV	  = mMainDSV;
	cCoreStorage::GetInstance()->mOnlyReadDSV = mOnlyReadDSV;

	// DSV ����
	cCoreStorage::GetInstance()->mDepthStencilState = mDepthStencilState;

	// ViewPort
	cCoreStorage::GetInstance()->mScreenViewport = &mScreenViewport;
}

// ȭ��ũ�� ��ȯ �� �籸�� 
void cInitD3D::OnResize()
{
	assert(md3dImmediateContext);
	assert(md3dDevice);
	assert(mSwapChain);

	// ȭ��ũ�Ⱑ ���Կ� ���� ����Ҽ� ���� ������ü�� �ı��Ѵ�.
	// ���ҽ� �� Ŭ����
	ClearResourceView();

	// ���� ü���� ũ�⸦ �����ϰ� ���� Ÿ�� �並 �ٽ� �����.
	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// �����۶�� �ؽ�ó�� �����, ���� Ÿ�ٺ�� �����Ѵ�. ���� Ÿ�ٺ並 ��������� �����Ѵ�.
	ID3D11Texture2D* backBuffer = nullptr;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mSreenRTV));
	ReleaseCOM(backBuffer);

	// �ھ ����
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

	// �ؽ�ó ���� ����
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

	// DSV ���� ����
	D3D11_DEPTH_STENCIL_VIEW_DESC DsvD;
	DsvD.Format                        = depthStencilRenderViewFormat;
	DsvD.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
	DsvD.Flags		                   = 0;
	DsvD.Texture2D.MipSlice            = 0;

	// SRV ���� ����
	D3D11_SHADER_RESOURCE_VIEW_DESC SRV_Desc;
	SRV_Desc.Format                    = basicColorResourceViewFormat;
	SRV_Desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRV_Desc.Texture2D.MipLevels       = TextureDesc.MipLevels;
	SRV_Desc.Texture2D.MostDetailedMip = 0;

	// RTV ���� ����
	D3D11_RENDER_TARGET_VIEW_DESC RTV_Desc;
	RTV_Desc.Format                    = basicColorResourceViewFormat;
	RTV_Desc.ViewDimension             = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTV_Desc.Texture2D.MipSlice        = 0;

	// ���� ���� ����
	TextureDesc.Format = depthStencilTextureFormat;
	HR(md3dDevice->CreateTexture2D       (&TextureDesc        , 0    , &mDepthStencilTexture));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilTexture, &DsvD, &mMainDSV));

	// �б����� ���̹���
	DsvD.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilTexture, &DsvD, &mOnlyReadDSV));

	//// ���� ���� �ؽ�ó ����� & ���� ���� �����ϱ�
	//SRV_Desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	//HR(md3dDevice->CreateShaderResourceView(mDepthStencilTexture, &SRV_Desc, &mDepthSRV));

	// �ؽ�ó ���� �ٲٱ�
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	//--------------------------------------------------------------------------------------//
	// ���� ���� �ؽ�ó ����� & ���� ���� �����ϱ�
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = basicColorTextureFormat;
	SRV_Desc.Format    = basicColorRenderViewFormat;
	RTV_Desc.Format    = basicColorRenderViewFormat;

	ID3D11Texture2D* mDepthTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mDepthTex));
	HR(md3dDevice->CreateRenderTargetView  (mDepthTex   , &RTV_Desc, &mDepthRTV));
	HR(md3dDevice->CreateShaderResourceView(mDepthTex   , &SRV_Desc, &mDepthSRV));

	//--------------------------------------------------------------------------------------//
	// Į�� ���� �ؽ�ó ����� & ������ ���� �����ϱ�
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = basicColorTextureFormat;
	SRV_Desc.Format    = basicColorRenderViewFormat;
	RTV_Desc.Format    = basicColorRenderViewFormat;

	ID3D11Texture2D* mColorTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mColorTex));
	HR(md3dDevice->CreateRenderTargetView  (mColorTex   , &RTV_Desc, &mColorRTV));
	HR(md3dDevice->CreateShaderResourceView(mColorTex   , &SRV_Desc, &mColorSRV));

	//--------------------------------------------------------------------------------------//
	// ��� ���� �ؽ�ó ����� & ��� ���� �����ϱ�
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = normalTextureFormat;
	SRV_Desc.Format    = normalRenderViewFormat;
	RTV_Desc.Format    = normalRenderViewFormat;
	
	ID3D11Texture2D* mNomalTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0        , &mNomalTex));
	HR(md3dDevice->CreateRenderTargetView  (mNomalTex   , &RTV_Desc, &mNomalRTV));
	HR(md3dDevice->CreateShaderResourceView(mNomalTex   , &SRV_Desc, &mNomalSRV));

	//--------------------------------------------------------------------------------------//
	// ������ ���� �ؽ�ó ����� & ������ ���� �����ϱ�
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = positionTextureFormat;
	SRV_Desc.Format    = positionRenderViewFormat;
	RTV_Desc.Format    = positionRenderViewFormat;

	ID3D11Texture2D* mPositionTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mPositionTex));
	HR(md3dDevice->CreateRenderTargetView  (mPositionTex, &RTV_Desc, &mPositionRTV));
	HR(md3dDevice->CreateShaderResourceView(mPositionTex, &SRV_Desc, &mPositionSRV));

	//--------------------------------------------------------------------------------------//
	// ������ ���� �ؽ�ó ����� & ������ ���� �����ϱ�
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = positionTextureFormat;
	SRV_Desc.Format    = positionRenderViewFormat;
	RTV_Desc.Format    = positionRenderViewFormat;

	ID3D11Texture2D* mShadowTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mShadowTex));
	HR(md3dDevice->CreateRenderTargetView  (mShadowTex  , &RTV_Desc, &mShadowRTV));
	HR(md3dDevice->CreateShaderResourceView(mShadowTex  , &SRV_Desc, &mShadowSRV));

	//--------------------------------------------------------------------------------------//
	// ����ŧ�� ���� �ؽ�ó ����� & ����ŧ�� ���� �����ϱ�
	//--------------------------------------------------------------------------------------//
	TextureDesc.Format = SpecularTextureFormat;
	SRV_Desc.Format    = SpecularRenderViewFormat;
	RTV_Desc.Format    = SpecularRenderViewFormat;

	ID3D11Texture2D* mSpecTex = 0;
	HR(md3dDevice->CreateTexture2D		   (&TextureDesc, 0		   , &mSpecTex));
	HR(md3dDevice->CreateRenderTargetView  (mSpecTex	, &RTV_Desc, &mSpecularRTV));
	HR(md3dDevice->CreateShaderResourceView(mSpecTex	, &SRV_Desc, &mSpecularSRV));

	//--------------------------------------------------------------------------------------//
	// �ؽ�ó ����
	//--------------------------------------------------------------------------------------//
	ReleaseCOM(mDepthTex);
	ReleaseCOM(mColorTex);
	ReleaseCOM(mNomalTex);
	ReleaseCOM(mPositionTex);
	ReleaseCOM(mSpecTex);
	ReleaseCOM(mShadowTex);

	// ������ ���ο� ��� ����, ���Ľǹ��۸� ���ε� �Ѵ�. (1���� ���� ����)
	// CreateDepthStencilView������ ����Ÿ�� ��� �Բ� ����/���ٽ� ���۵� ���ÿ� ���� �����մϴ�
	md3dImmediateContext->OMSetRenderTargets(1, &mSreenRTV, mMainDSV);

	// ����Ʈ ��ȯ�� �Ѵ�.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width    = static_cast<float>(mClientWidth);
	mScreenViewport.Height   = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);

	//--------------------------------------------------------------------------------------//
	// ����
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

	// �ھ ����
	SetCoreStorage();

	// ������ �� �ʱ�ȭ
	cShadowMap::GetInstance()->ClearClass();
	cShadowMap::GetInstance()->InitClass(mClientWidth, mClientHeight);

	//-----------------------------------------------------------------------------//
	// HDR ���� ����� ����ϴ�.
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

	// HDR �ʱ�ȭ
	cHDRManager::GetInstance()->ClearClass();
	cHDRManager::GetInstance()->InitClass(mClientWidth, mClientHeight);

}

// ���� ���ν��� (�ż��� ���ν��� ����)
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gcInitD3D->MsgProc(hwnd, msg, wParam, lParam);
}

// ����̽� üũ
void cInitD3D::CheckDeviceLost()
{
	HRESULT hr;

	// ����̽� �ν�Ʈ�� �Ͼ����
	hr = md3dDevice->GetDeviceRemovedReason();

	switch (hr)
	{
	// ����
	case S_OK:
		break;

	// �� �ƿ�, ����
	case DXGI_ERROR_DEVICE_HUNG:
	case DXGI_ERROR_DEVICE_RESET:

		// ���� �� ���ø����̼� ����
		if(ReCreateDevice())
			PostQuitMessage(0);

		// ���� �� ���ø����̼� ����
		if (FAILED(hr))
			PostQuitMessage(0);

		break;

	// ���� �� ���ø����̼� ����
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
	case DXGI_ERROR_INVALID_CALL:
	default:
		cout << "����̽� �ν�Ʈ" << endl;
		PostQuitMessage(0);
		break;
	}
}

// �޼��� ���ν���
LRESULT cInitD3D::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ����̽� �ν�Ʈ �˻�
	if (md3dDevice)
		CheckDeviceLost();

	switch (msg)
	{
	// Ű����� ���콺���õ� �͵��� ������Ʈ �Ŵ����� �ѱ��,
	// �̰��� �ֻ��� ��ɾ� �ڵ�� ����Ѵ�.

	//---------------------------------------------------------------------------------------
	// Ű����
	//---------------------------------------------------------------------------------------
	// �̰Ŷ����� 3ȸ�� ����.
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
		// ���콺
		//---------------------------------------------------------------------------------------
		// ���콺 Ŭ��
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		cGameStateManager::GetInstance()->MouseEvent(msg, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
		return 0;

		// ���콺 �̵�
	case WM_MOUSEMOVE:
		//cout << GET_X_LPARAM(lParam) << ", " << GET_Y_LPARAM(lParam) << endl;

		cGameStateManager::GetInstance()->MouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	
		//---------------------------------------------------------------------------------------
		// ������ ����
		//---------------------------------------------------------------------------------------
#ifdef WINDOW_DISABLE_NO_RENDER
		case WM_ACTIVATE:
			// ������ Ȱ��, ��Ȱ��
			// ������ Ȱ��, ��Ȱ��ȭ�� ����, Ÿ�̸� ���� Ȱ��, ��Ȱ��ȭ�� �Ѵ�.
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
		// ������ ����� �����°�?
		// ���ο� Ŭ���̾�Ʈ ������ ũ�⸦ ����
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
				// �ּ�ȭ ���¿��� ������°�?
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}
				// �ִ�ȭ ���¿��� ������°�?
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				// ����ڰ� ũ�� ���� ���븦 �巡���ϴ°�?
				// �巡�� �߿��� �ƹ��۾��� ���� �ʴ´�.
				else if (mResizing)
				{

				}
				// �������� ������Ʈ ��û
				else 
				{
					OnResize();
				}
			}
		}
		return 0;

		//WM_EXITSIZEMOVE�� ����ڰ� ũ�� ���� ���븦 ������� �� ���۵ȴ�.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing  = true;
		mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE�� ����ڰ� ũ�� ���� ���븦 ���� �� ���۵ȴ�.
		// ���⿡�� ���ο� â ũ�⿡ ���� ��� ���� �ٽ� ����.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing  = false;
		mTimer.Start();
		OnResize();
		return 0;

		// �����찡 �ı� �� ��, WM_DESTROY�� ���۵˴ϴ�.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// �޴��� ����ڰ� Ȱ������ �� WM_MENUCHAR�� �޽����� ���۵ȴ�.
		// � �ϸ�� �Ǵ� �׼��������� Ű�� �ش����� �ʴ� Ű�� �����ϴ�.
	case WM_MENUCHAR:
		//�츮�� ALTŰ�� �Է� �� ��, �� �Ҹ����� ���ʽÿ�.
		return MAKELRESULT(0, MNC_CLOSE);

		// �ʹ� â�� �۾����� ���� �����ϱ� ����.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


// ������ �ʱ�ȭ
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

	// Ŭ���̾�Ʈ ũ�� ���
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	// ������ ���� �� �˻�
	mhMainWnd = CreateWindow(L"D3DWndClassName", mMainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0);
	if (!mhMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// ������ ���
	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
}

// ���̷�Ʈ ��ü �ʱ�ȭ
// - ��������� �����ϴ°� üũ�ϰ�(���� ���ø� ��), ����ü��, �� ��ü ���� ����, �����Ѵ�.
bool cInitD3D::InitDirect3D()
{
	// ����̽�, ����̽� ���ؽ�Ʈ ����
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

	// ����ۿ� 4X MSAA ǰ�� ������ Ȯ��
	HR(md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	assert(m4xMsaaQuality > 0);

	// ����ü�� ����
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width                   = mClientWidth;
	sd.BufferDesc.Height                  = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator   = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

	// ���� ���ø� ���� (Use 4X MSAA?) 
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

	// ���� ����
	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = mhMainWnd; // ���⼭ ������ �ڵ� ���� (��� DC ����)
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags		= 0;

	// ����̽� ����
	IDXGIDevice* dxgiDevice = 0;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// ����ü�� ���� �� Ȯ��
	HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));

	// �� ��ü ����
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// â ũ�� ���� (�ڵ� �ߺ� ���ϱ� ���� ���)
	OnResize();

	return true;
}

// ������ ���¸� ����Ѵ�. (FPS)
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

// Get �Լ���
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
