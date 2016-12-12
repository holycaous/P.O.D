#pragma once
// ������ �� Ŭ����
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
	// ������ �� ũ��� ���� ���� ũ��
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

	// �� �߰��ϱ�
	void SetmSceneBoundsRadius(float _Radius)
	{
		mSceneBounds.Radius = _Radius;
	}

	// ������ �� �ʱ�ȭ
	void InitClass(UINT _Width, UINT _Height)
	{
		mWidth  = _Width;
		mHeight = _Height;

		// DSV�� �ؼ��Ϸ����ϱ� ������ �����̾��� ������ ����Ѵ�.
		// ��Ʈ�� DXGI_FORMAT_D24_UNORM_S8_UINT�� ������ SRV�� �ؼ� �� ���Դϴ�.
		// ��Ʈ�� DXGI_FORMAT_R24_UNORM_X8_TYPELESS�Դϴ�.
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
		HR(mCoreStorage->md3dDevice->CreateShaderResourceView(tDepthMap, &srvDesc, &mDepthMapSRV)); // ���� �ؽ�ó�� ���ε� �Ǿ��־, �����

		// View�� �ؽ�ó�� ���� ������ �����ϹǷ� ������ ������ �� �� �ֽ��ϴ�.
		ReleaseCOM(tDepthMap);
	}
	
	void BindDsvAndSetNullRenderTarget()
	{
		// ���� ���������� �������ϱ� ������ �׸��� ���� ���̴� �Է����� ���ε� �����մϴ�.
		// �׸��ڴ� ��� ���Կ����� �� �����Ƿ� ��� ������ ����ϴ�.
		ID3D11ShaderResourceView* nullSRV[16] = { 0 };
		mCoreStorage->md3dImmediateContext->PSSetShaderResources(0, 16, nullSRV);

		// ���� ���ۿ� ����� �� ���̱� ������ �� ���� Ÿ���� �����Ͻʽÿ�.
		// null ���� Ÿ���� �����ϸ� �÷� ���Ⱑ ��Ȱ��ȭ�˴ϴ�.
		ID3D11RenderTargetView* renderTargets = NULL;

		mCoreStorage->md3dImmediateContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		mCoreStorage->md3dImmediateContext->OMSetRenderTargets(1, &renderTargets, mDepthMapDSV);
	}

	void BuildShadowTransform()
	{
		// �� �𷺼ų� ����Ʈ������ ������ ����
		XMVECTOR lightDir  = XMLoadFloat3(&mLightManager->mSunDirLight.Direction);
		XMVECTOR lightPos  = -2.0f * mSceneBounds.Radius * lightDir; // �� ���� * ���� (mSceneBounds.Radius * -lightDir �ص� ����� ����..-_-) 
		XMVECTOR targetPos = XMLoadFloat3(&mSceneBounds.Center);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMMATRIX V = XMMatrixLookAtLH(lightPos, targetPos, up);

		// ��豸�� ����Ʈ �� �������� �ѱ�
		XMFLOAT3 sphereCenterLS;
		XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, V));

		// ����Ʈ �������� �ٿ�� ���Ǿ� ���������� ����� �ѷ� �Դϴ�.
		float l = sphereCenterLS.x - mSceneBounds.Radius;
		float b = sphereCenterLS.y - mSceneBounds.Radius;
		float n = sphereCenterLS.z - mSceneBounds.Radius;
		float r = sphereCenterLS.x + mSceneBounds.Radius;
		float t = sphereCenterLS.y + mSceneBounds.Radius;
		float f = sphereCenterLS.z + mSceneBounds.Radius;
		XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

		// NDC ���� [-1, + 1] ^ 2�� �ؽ�ó ���� [0,1] ^ 2�� ��ȯ�մϴ�.
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