#pragma once
// 쉐도우 맵 클래스
class cShadowMap : public cSingleton<cShadowMap>
{
public:
	InitMetaData* mScreen       = nullptr;	  
	BufferType  * mScreenBuffer = nullptr;
	XMFLOAT4X4    mScreenMtx;

	UINT mWidth;
	UINT mHeight;

	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView  * mDepthMapDSV;

	D3D11_VIEWPORT mViewport;

	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();
public:
	// 쉐도우 맵 크기는 임의 지정 크기
	cShadowMap()
	{
		mWidth = mHeight = 0;
		mDepthMapSRV = nullptr;
		mDepthMapDSV = nullptr;
	}

	~cShadowMap()
	{
		ClearIns();

		SafeDelete(mScreenBuffer);
		SafeDelete(mScreen);
		ReleaseCOM(mDepthMapSRV);
		ReleaseCOM(mDepthMapDSV);
	}

	// 모델 추가하기
	void AddScreen(float _x, float _y, float _z)
	{
		mScreen->AddModel(_x, _y, _z, e_StaticObj);
	}

	// 쉐도우 맵 초기화
	void InitClass(UINT _Width, UINT _Height)
	{
		mWidth  = _Width;
		mHeight = _Height;

		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.Width    = static_cast<float>(mWidth);
		mViewport.Height   = static_cast<float>(mHeight);
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		// DSV가 해석하려고하기 때문에 형식이없는 형식을 사용한다.
		// 비트는 DXGI_FORMAT_D24_UNORM_S8_UINT와 같지만 SRV는 해석 할 것입니다.
		// 비트는 DXGI_FORMAT_R24_UNORM_X8_TYPELESS입니다.
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width              = mWidth;
		texDesc.Height             = mHeight;
		texDesc.MipLevels          = 1;
		texDesc.ArraySize          = 1;
		texDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count   = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage              = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags     = 0;
		texDesc.MiscFlags          = 0;

		ID3D11Texture2D* tDepthMap  = 0;
		HR(mCoreStorage->md3dDevice->CreateTexture2D(&texDesc, 0, &tDepthMap));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags                     = 0;
		dsvDesc.Format                    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension             = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice        = 0;
		HR(mCoreStorage->md3dDevice->CreateDepthStencilView(tDepthMap, &dsvDesc, &mDepthMapDSV));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format                    = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels       = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		HR(mCoreStorage->md3dDevice->CreateShaderResourceView(tDepthMap, &srvDesc, &mDepthMapSRV));

		// View는 텍스처에 대한 참조를 저장하므로 참조를 릴리즈 할 수 있습니다.
		ReleaseCOM(tDepthMap);
	}
	
	void BindDsvAndSetNullRenderTarget()
	{
		mCoreStorage->md3dImmediateContext->RSSetViewports(1, &mViewport);

		// 깊이 버퍼에 드로잉 할 것이기 때문에 널 렌더 타겟을 설정하십시오.
		// null 렌더 타겟을 설정하면 컬러 쓰기가 비활성화됩니다.
		ID3D11RenderTargetView* renderTargets[1] = { 0 };
		mCoreStorage->md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);
		mCoreStorage->md3dImmediateContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void MakeShadowInsBuf()
	{
		// 쉐도우맵 인스턴스 버퍼생성
		mScreenBuffer->MakeShadowInsBuf(mScreen);
	}

	void UpdateIns()
	{
		mScreenBuffer->UpdateScreenIns(mScreen);
	}

	void ClearIns()
	{
		mScreen      ->ClearWdMtx();
		mScreenBuffer->ClearInsBuf();
	}
};