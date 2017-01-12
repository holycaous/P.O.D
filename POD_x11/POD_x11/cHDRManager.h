#pragma once
class cHDRManager : public cSingleton<cHDRManager>
{
	UINT mWidth;
	UINT mHeight;

	float m_fMiddleGrey;
	float m_fWhite;

	cCoreStorage*  mCoreStorage = cCoreStorage::GetInstance();
public:
	// �ٿ� ������ ���������� 1D �߰� ���� ����
	ID3D11Buffer			 * mDownScale1DBuffer;
	ID3D11UnorderedAccessView* mDownScale1DUAV;
	ID3D11ShaderResourceView * mDownScale1DSRV;

	// ��� �ֵ�
	ID3D11Buffer             * mAvgLumBuffer;
	ID3D11UnorderedAccessView* mAvgLumUAV;
	ID3D11ShaderResourceView * mAvgLumSRV;

	// HDR ����
	TDownScaleCB mTDownScaleCB;
	TFinalPassCB mTFinalPassCB;

	UINT mDownScaleGroups;
public:
	void Init(UINT _Width, UINT _Height)
	{
		mWidth  = _Width;
		mHeight = _Height;

		m_fMiddleGrey = 0.0025f;
		m_fWhite      = 1.5f;

		//-----------------------------------------------------------------------------//
		// HDR ���� - ��� ���̴� 
		//-----------------------------------------------------------------------------//
		mDownScaleGroups = (UINT)ceil((float)(mWidth * mHeight / 16) / 1024.0f); // ceil <-- �Ǽ��� �ø��ϴ� �Լ�


		//-----------------------------------------------------------------------------------//
		// ������ �� �ֵ� ���۸� �Ҵ��մϴ�.
		//-----------------------------------------------------------------------------------//
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags           = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.StructureByteStride = sizeof(float);
		bufferDesc.ByteWidth           = mDownScaleGroups * bufferDesc.StructureByteStride;
		bufferDesc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		HR(mCoreStorage->md3dDevice->CreateBuffer(&bufferDesc, NULL, &mDownScale1DBuffer));


		D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
		ZeroMemory(&DescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

		DescUAV.Format             = DXGI_FORMAT_UNKNOWN;
		DescUAV.ViewDimension      = D3D11_UAV_DIMENSION_BUFFER;
		DescUAV.Buffer.NumElements = mDownScaleGroups;

		HR(mCoreStorage->md3dDevice->CreateUnorderedAccessView(mDownScale1DBuffer, &DescUAV, &mDownScale1DUAV));

		//-----------------------------------------------------------------------------------//
		// ��� �ֵ� ���۸� �Ҵ��մϴ�.
		//-----------------------------------------------------------------------------------//
		D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd;
		ZeroMemory(&dsrvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		dsrvd.Format             = DXGI_FORMAT_UNKNOWN;
		dsrvd.ViewDimension      = D3D11_SRV_DIMENSION_BUFFER;
		dsrvd.Buffer.NumElements = mDownScaleGroups;

		HR(mCoreStorage->md3dDevice->CreateShaderResourceView(mDownScale1DBuffer, &dsrvd, &mDownScale1DSRV));

		bufferDesc.ByteWidth = sizeof(float);
		HR(mCoreStorage->md3dDevice->CreateBuffer(&bufferDesc, NULL, &mAvgLumBuffer));

		DescUAV.Buffer.NumElements = 1;
		HR(mCoreStorage->md3dDevice->CreateUnorderedAccessView(mAvgLumBuffer, &DescUAV, &mAvgLumUAV));

		dsrvd.Buffer.NumElements = 1;
		HR(mCoreStorage->md3dDevice->CreateShaderResourceView(mAvgLumBuffer, &dsrvd, &mAvgLumSRV));
	}

	// ��ǻƮ ���̴� ����
	void ClearComputeShader()
	{
		// ���� ������ ���� CS���� �Է� �ؽ�ó�� ���ε��� �����մϴ�.
		ID3D11ShaderResourceView* nullSRV[1] = { 0 };
		mCoreStorage->md3dImmediateContext->CSSetShaderResources(0, 1, nullSRV);

		// ��� ���̴��� ����� �� ���ε��մϴ� (�� �н��� ���� �н��� �Է����� ����մϴ�.)
		// ���ÿ� ���ҽ��� ��°� �Է��� �� �� �����ϴ�.
		ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
		mCoreStorage->md3dImmediateContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	}

	void Update()
	{
		mTDownScaleCB.nWidth       = mWidth  / 4;
		mTDownScaleCB.nHeight      = mHeight / 4;
		mTDownScaleCB.nTotalPixels = mTDownScaleCB.nWidth * mTDownScaleCB.nHeight;
		mTDownScaleCB.nGroupSize   = mDownScaleGroups;

		mTFinalPassCB.fMiddleGrey  = m_fMiddleGrey;
		mTFinalPassCB.fLumWhiteSqr = m_fWhite;
		mTFinalPassCB.fLumWhiteSqr *= mTFinalPassCB.fMiddleGrey;  // �߰� ȸ�� ������ ũ�� ����
		mTFinalPassCB.fLumWhiteSqr *= mTFinalPassCB.fLumWhiteSqr; // Squre
	}

	void ClearClass()
	{
		ClearComputeShader();

		ReleaseCOM(mDownScale1DBuffer);
		ReleaseCOM(mDownScale1DUAV);
		ReleaseCOM(mDownScale1DSRV);
		ReleaseCOM(mAvgLumBuffer);
		ReleaseCOM(mAvgLumUAV);
		ReleaseCOM(mAvgLumSRV);
	}
};

