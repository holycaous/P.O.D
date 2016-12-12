#pragma once
// 쉐도우 맵 클래스
class cShadowMap : public cSingleton<cShadowMap>
{
public:
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView  * mDepthMapDSV;

	UINT mWidth;
	UINT mHeight;

	XMFLOAT4X4    mShadowTransform;
	XMFLOAT4X4    mLightViewProj;
	XMFLOAT4X4    mLightView;
	XMFLOAT4X4    mLightProj;

	//D3D11_VIEWPORT mViewport;
	BoundingSphere mSceneBounds;

	cLightManager* mLightManager = cLightManager::GetInstance();
	cCoreStorage*  mCoreStorage  = cCoreStorage ::GetInstance();
public:
	// 쉐도우 맵 크기는 임의 지정 크기
	cShadowMap()
	{
		mSceneBounds.Radius = 0.0f;
		mSceneBounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mWidth = mHeight = 0;
		mDepthMapSRV = nullptr;
		mDepthMapDSV = nullptr;

		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mShadowTransform, I);
		XMStoreFloat4x4(&mLightViewProj, I);
		XMStoreFloat4x4(&mLightView, I);
		XMStoreFloat4x4(&mLightProj, I);
	}

	~cShadowMap()
	{
		ClearClass();
	}

	void ClearClass()
	{
		ReleaseCOM(mDepthMapSRV);
		ReleaseCOM(mDepthMapDSV);
	}

	// 모델 추가하기
	void SetmSceneBoundsRadius(float _Radius)
	{
		mSceneBounds.Radius = _Radius;
	}

	// 쉐도우 맵 초기화
	void InitClass(UINT _Width, UINT _Height)
	{
		mWidth  = _Width;
		mHeight = _Height;

		// DSV가 해석하려고하기 때문에 형식이없는 형식을 사용한다.
		// 비트는 DXGI_FORMAT_D24_UNORM_S8_UINT와 같지만 SRV는 해석 할 것입니다.
		// 비트는 DXGI_FORMAT_R24_UNORM_X8_TYPELESS입니다.
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width              = mWidth;
		texDesc.Height             = mHeight;
		texDesc.MipLevels          = 1;
		texDesc.ArraySize          = 1;
		texDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS; //DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS
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
		dsvDesc.Format                    = DXGI_FORMAT_D24_UNORM_S8_UINT; //DXGI_FORMAT_D32_FLOAT_S8X24_UINT
		dsvDesc.ViewDimension             = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice        = 0;
		HR(mCoreStorage->md3dDevice->CreateDepthStencilView(tDepthMap, &dsvDesc, &mDepthMapDSV));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format                    = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels       = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		HR(mCoreStorage->md3dDevice->CreateShaderResourceView(tDepthMap, &srvDesc, &mDepthMapSRV)); // 같은 텍스처로 바인딩 되어있어서, 연결됨

		// View는 텍스처에 대한 참조를 저장하므로 참조를 릴리즈 할 수 있습니다.
		ReleaseCOM(tDepthMap);
	}
	
	void BindDsvAndSetNullRenderTarget()
	{
		// 다음 프레임으로 렌더링하기 때문에 그림자 맵을 셰이더 입력으로 바인딩 해제합니다.
		// 그림자는 모든 슬롯에있을 수 있으므로 모든 슬롯을 지웁니다.
		ID3D11ShaderResourceView* nullSRV[16] = { 0 };
		mCoreStorage->md3dImmediateContext->PSSetShaderResources(0, 16, nullSRV);

		// 깊이 버퍼에 드로잉 할 것이기 때문에 널 렌더 타겟을 설정하십시오.
		// null 렌더 타겟을 설정하면 컬러 쓰기가 비활성화됩니다.
		ID3D11RenderTargetView* renderTargets = NULL;

		mCoreStorage->md3dImmediateContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		mCoreStorage->md3dImmediateContext->OMSetRenderTargets(1, &renderTargets, mDepthMapDSV);
	}

	void BuildShadowTransform()
	{
		// 주 디렉셔널 라이트에서만 쉐도우 적용
		XMVECTOR lightDir  = XMLoadFloat3(&mLightManager->mSunDirLight.Direction);
		XMVECTOR lightPos  = -2.0f * mSceneBounds.Radius * lightDir; // 빛 방향 * 지름 (mSceneBounds.Radius * -lightDir 해도 결과가 같음..-_-) 
		XMVECTOR targetPos = XMLoadFloat3(&mSceneBounds.Center);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMMATRIX V = XMMatrixLookAtLH(lightPos, targetPos, up);

		// 경계구를 라이트 뷰 공간으로 넘김
		XMFLOAT3 sphereCenterLS;
		XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, V));

		// 라이트 공간에서 바운드 스피어 직교투영이 장면을 둘러 쌉니다.
		float l = sphereCenterLS.x - mSceneBounds.Radius;
		float b = sphereCenterLS.y - mSceneBounds.Radius;
		float n = sphereCenterLS.z - mSceneBounds.Radius;
		float r = sphereCenterLS.x + mSceneBounds.Radius;
		float t = sphereCenterLS.y + mSceneBounds.Radius;
		float f = sphereCenterLS.z + mSceneBounds.Radius;
		XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

		// NDC 공간 [-1, + 1] ^ 2을 텍스처 공간 [0,1] ^ 2로 변환합니다.
		XMMATRIX T(
			0.5f,  0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f,  0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f);

		XMMATRIX VP = XMMatrixMultiply(V, P);
		XMMATRIX S  = V*P*T;

		XMStoreFloat4x4(&mLightView, V);
		XMStoreFloat4x4(&mLightProj, P);
		XMStoreFloat4x4(&mShadowTransform, S);
		XMStoreFloat4x4(&mLightViewProj, VP);
	}
};